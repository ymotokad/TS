/*
  This file is part of TS software suite.

  TS software suite is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  TS software suite is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with TS software suite.  If not, see <http://www.gnu.org/licenses/>.

*/

static char rcsid[] = "@(#)$Id$";
#ifdef __GNUG__
#pragma implementation
#endif
#include <assert.h>
#include <stdio.h>
#define IMPLEMENTING_ISO13818_PACKETIZEDELEMENTARYSTREAM
#include "ByteArrayBuffer.h"
#include "ISO13818_PacketizedElementaryStream.h"

/*
 * ISO13818_PES_Packet class
 */
FIELDWIDTH_PREAMBLE(ISO13818_PES_Packet)
FIELDWIDTH_CONTENT(ISO13818_PES_Packet_packet_start_code_prefix,	24)
FIELDWIDTH_CONTENT(ISO13818_PES_Packet_stream_id,			8)
FIELDWIDTH_CONTENT(ISO13818_PES_Packet_PES_packet_length,		16)
FIELDWIDTH_CONTENT(ISO13818_PES_Packet_PES_packet_data_byte,		8)
FIELDWIDTH_POSTAMBLE(ISO13818_PES_Packet, initvars())

void ISO13818_PES_Packet::initvars() {
}

ISO13818_PES_Packet::ISO13818_PES_Packet(const ProgramClock &src) : data_completed(false), packet_start_time(src), packet_data_pointer(0), header_parsed_p(false)  {
   initobj();
}

ISO13818_PES_Packet::~ISO13818_PES_Packet() {
   PESDataList::iterator it;
   for (it = blist.begin(); it != blist.end(); it++) {
      ByteArray *p = *it;
      delete p;
   }
}

inline int charAt(PESDataList::const_iterator it, int idx, PESDataList::const_iterator it_end) {
   if (idx < (*it)->length()) return (*it)->at(idx);
   idx -= (*it)->length();
   it++;
   if (it == it_end) return -1;
   return (*it)->at(idx);
}
int ISO13818_PES_Packet::find3(int off, uint8 c1, uint8 c2, uint8 c3) const {
   int ret = off;
   off += packet_data_pointer;
   try {
      for (PESDataList::const_iterator it = blist.begin(); it != blist.end(); it++) {
	 ByteArray *p = *it;
	 int idx;
	 if (off < p->length()) {
	    idx = off;
	    off = 0;
	 } else {
	    off -= p->length();
	    continue;
	 }
	 while (idx < p->length()) {
	    if (p->at(idx) == c1) {
	       if (charAt(it, idx + 1, blist.end()) == c2) {
		  if (charAt(it, idx + 2, blist.end()) == c3) {
		     return ret;
		  }
	       }
	    }
	    idx++;
	    ret++;
	 }
      }
   } catch(ByteArrayOverflowException &e) {
   }
   return -1;
}


void ISO13818_PES_Packet::put(const ByteArray& data) {
   if (blist.empty()) {
      setBuffer(new ByteArray(data));
      assert(data.length() >= headerSize());
   }
   blist.push_back(new ByteArray(data));
}


ByteArray *ISO13818_PES_Packet::read(int bytes) {
   int off = packet_data_pointer;
   if (off + bytes > length()) return NULL;

   int bytesToBeRead = bytes;
   ByteArrayBuffer *buf = new ByteArrayBuffer(1024, 1024);
   if (buf == NULL) return NULL;
   for (PESDataList::const_iterator it = blist.begin(); it != blist.end(); it++) {
      if (bytesToBeRead <= 0) break;
      ByteArray *p = *it;
      if (p->length() <= off) {
	 off -= p->length();
	 continue;
      }
      if (p->length() > off + bytesToBeRead) {
	 buf->append(p->part(off, bytesToBeRead), bytesToBeRead);
	 bytesToBeRead = 0;
	 off = 0;
	 break;
      } else {
	 int n = p->length() - off;
	 buf->append(p->part(off, n), n);
	 bytesToBeRead -= n;
	 off = 0;
      }
   }

   assert(bytesToBeRead == 0);

   packet_data_pointer += bytes;
   shrink_blist();
   return buf;
}

void ISO13818_PES_Packet::shrink_blist() {
   ByteArray *p = blist.front();
   while (packet_data_pointer > 0) {
      if (p->length() > packet_data_pointer) return;
      packet_data_pointer -= p->length();
      delete p;
      blist.pop_front();
      if (blist.empty()) return;
      p = blist.front();
   }
}

#if 0

void ISO13818_PES_Packet::remove(int bytes) {
   int bytesToBeRemoved = bytes;
   ByteArray *p = blist.front();
   blist.pop_front();
   while (bytesToBeRemoved > 0) {
      if (p->length() > bytesToBeRemoved) {
	 ByteArray *sub = p->subarray(bytesToBeRemoved);
	 delete p;
	 blist.push_front(sub);
	 bytesToBeRemoved = 0;
      } else {
	 bytesToBeRemoved -= p->length();
	 delete p;
	 p = blist.front();
	 blist.pop_front();
      }
   }
}

void ISO13818_PES_Packet::clear() {
   while (!blist.empty()) {
      ByteArray *p = blist.front();
      blist.pop_front();
      delete p;
   }
}
#endif

int ISO13818_PES_Packet::length() const {
   int len = 0;
   for (PESDataList::const_iterator it = blist.begin(); it != blist.end(); it++) {
      ByteArray *p = *it;
      len += p->length();
   }

   return len;
}


/*
 * ISO13818_PacketizedElementaryStream class
 */

ISO13818_PacketizedElementaryStream::ISO13818_PacketizedElementaryStream(int type) : streamtype(type) {
}
ISO13818_PacketizedElementaryStream::~ISO13818_PacketizedElementaryStream() {
   PESPacketList::iterator it;
   for (it = plist.begin(); it != plist.end(); it++) {
      ISO13818_PES_Packet *p = *it;
      delete p;
   }
}

inline uint8 decompose_flag(uint8 b, int off, int width) {
   static uint8 start_masks[] = {
      0xff, 0x7f, 0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x01
   };
   static uint8 end_masks[] = {
      0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff
   };
   return (b & start_masks[off] & end_masks[off + width - 1]) >> 8 - (off + width);
}

#if 0
void ISO13818_PacketizedElementaryStream::put(const ProgramClock &clk, const ByteArray *packet) {

   if (blist.empty()) packet_start_time.set(clk);

   // append to blist
   blist.push_back(new ByteArray(*packet));

   // Map to PES
   ISO13818_PES_Packet pes;
   ByteArray *p = blist.front();
   assert(p->length() >= pes.headerSize());
   pes.setBuffer(new ByteArray(*p));
   p->hexdump(0,&std::cout);
   
   if (pes.packet_start_code_prefix() == 0x000001 && pes.stream_id() >= ISO13818_PacketizedElementaryStream::stream_id_program_stream_map) {
      try {
	 // Calculate PES header size
	 int pes_header_size = pes.headerSize();
	 if (pes.stream_id() != stream_id_program_stream_map &&
	     pes.stream_id() != stream_id_padding_stream &&
	     pes.stream_id() != stream_id_private_stream_2 && 
	     pes.stream_id() != stream_id_ECM_stream &&
	     pes.stream_id() != stream_id_EMM_stream && 
	     pes.stream_id() != stream_id_program_stream_directory && 
	     pes.stream_id() != stream_id_DSMCC_stream && 
	     pes.stream_id() != stream_id_H222_1_type_E) {

#if 0
	    uint8 PTS_DTS_flags = decompose_flag(p->at(pes_header_size + 1), 0, 2);
	    uint8 ESCR_flag = decompose_flag(p->at(pes_header_size + 1), 2, 1);
	    uint8 ES_rate_flag = decompose_flag(p->at(pes_header_size + 1), 3, 1);
	    uint8 DSM_trick_mode_flag = decompose_flag(p->at(pes_header_size + 1), 4, 1);
	    uint8 additional_copy_info_flag = decompose_flag(p->at(pes_header_size + 1), 5, 1);
	    uint8 PES_CRC_flag = decompose_flag(p->at(pes_header_size + 1), 6, 1);
	    uint8 PES_extension_flag = decompose_flag(p->at(pes_header_size + 1), 7, 1);
#endif
	    int PES_header_data_length = p->at(pes_header_size + 2);
	    pes_header_size += 2;
	    pes_header_size += PES_header_data_length;
	    pes_header_size += 4;
#if 0
	    if (PTS_DTS_flags == 2) pes_header_size += 5;
	    if (PTS_DTS_flags == 3) pes_header_size += 10;
	    if (ESCR_flag) pes_header_size += 6;
	    if (ES_rate_flag) pes_header_size += 3;
	    if (DSM_trick_mode_flag) pes_header_size += 1;
	    if (additional_copy_info_flag) pes_header_size += 1;
	    if (PES_CRC_flag) pes_header_size += 2;
	    if (PES_extension_flag) {
	       uint8 PES_private_data_flag = decompose_flag(p->at(pes_header_size), 0, 1);
	       uint8 pack_header_field_flag = decompose_flag(p->at(pes_header_size), 1, 1);
	       uint8 program_packet_sequence_counter_flag = decompose_flag(p->at(pes_header_size), 2, 1);
	       uint8 P_STD_buffer_flag = decompose_flag(p->at(pes_header_size), 3, 1);
	       uint8 PES_extension_flag_2 = decompose_flag(p->at(pes_header_size), 7, 1);
	       pes_header_size += 1;

	       if (PES_private_data_flag) pes_header_size += 128/8;
	       if (pack_header_field_flag) {
		  uint8 pack_field_length = p->at(pes_header_size);
		  pes_header_size += 1;
		  pes_header_size += pack_field_length;
	       }
	       if (program_packet_sequence_counter_flag) pes_header_size += 2;
	       if (P_STD_buffer_flag) pes_header_size += 2;
	       if (PES_extension_flag_2) {
		  uint8 PES_extenstion_field_length = p->at(pes_header_size) & 0x7f;
		  pes_header_size += PES_extenstion_field_length;
	       }
	    }
#endif
	 }
	 int told_length = pes.headerSize() + pes.PES_packet_length();
	 int actual_length = length();
	 if (actual_length > told_length) {
	    // PES starts only at TS packet boundary, which means that a single TS packet cannot contain more than
	    // one PES packets, in other words, actual_length should not be greater than told_length.
	    // The code reached here just becuase that the 000001 was not packet_start_code_prefix of PES. So just discard the data.
	    clear();
	 } else if (actual_length == told_length) {
	    data_completed = true;
	    remove(pes_header_size);
	 } else {
	    data_completed = false;
	 }
      } catch(ByteArrayOverflowException &e) {
	 data_completed = false;
      }
   } else {
      clear();
   }
}
#endif

void ISO13818_PacketizedElementaryStream::startUnit(const ProgramClock &clk, const ByteArray *src) {
   // Complete previous packet
   if (!plist.empty()) {
      ISO13818_PES_Packet *prev_packet = plist.back();
      prev_packet->data_completed = true;
   }

   // Add packet
   ISO13818_PES_Packet *packet = new ISO13818_PES_Packet(clk);
   packet->put(*src);
   packet->setDataPointer(0);
   plist.push_back(packet);

}

void ISO13818_PacketizedElementaryStream::putUnit(const ByteArray *src) {
   if (!plist.empty()) {
      ISO13818_PES_Packet *packet = plist.back();
      packet->put(*src);
   }
}

bool ISO13818_PacketizedElementaryStream::isReadable() {
   if (plist.empty()) return false;
   
   ISO13818_PES_Packet *packet = plist.front();

   if (packet->wasHeaderParsed()) {
      if (packet->PES_packet_length() == 0) return true;
      if (packet->data_completed == true) return true;
      int told_length = packet->headerSize() + packet->PES_packet_length();
      int actual_length = packet->length();
      if (actual_length >= told_length) {
	 packet->data_completed = true;
	 last_packet_start_time.set(packet->packet_start_time);
	 return true;
      }
      return false;
   }

   if (packet->packet_start_code_prefix() != 0x000001 || packet->stream_id() < ISO13818_PacketizedElementaryStream::stream_id_program_stream_map) {
      plist.pop_front();
      delete packet;
      return false;
   }

   // Calculate PES header size
   try {
      int pes_header_size = packet->headerSize();
      if (packet->stream_id() != stream_id_program_stream_map &&
	  packet->stream_id() != stream_id_padding_stream &&
	  packet->stream_id() != stream_id_private_stream_2 && 
	  packet->stream_id() != stream_id_ECM_stream &&
	  packet->stream_id() != stream_id_EMM_stream && 
	  packet->stream_id() != stream_id_program_stream_directory && 
	  packet->stream_id() != stream_id_DSMCC_stream && 
	  packet->stream_id() != stream_id_H222_1_type_E) {
	 
#if 0
	 uint8 PTS_DTS_flags = decompose_flag(packet->at(pes_header_size + 1), 0, 2);
	 uint8 ESCR_flag = decompose_flag(packet->at(pes_header_size + 1), 2, 1);
	 uint8 ES_rate_flag = decompose_flag(packet->at(pes_header_size + 1), 3, 1);
	 uint8 DSM_trick_mode_flag = decompose_flag(packet->at(pes_header_size + 1), 4, 1);
	 uint8 additional_copy_info_flag = decompose_flag(packet->at(pes_header_size + 1), 5, 1);
	 uint8 PES_CRC_flag = decompose_flag(packet->at(pes_header_size + 1), 6, 1);
	 uint8 PES_extension_flag = decompose_flag(packet->at(pes_header_size + 1), 7, 1);
#endif
	 int PES_header_data_length = packet->at(pes_header_size + 2);
	 pes_header_size += 3;
	 pes_header_size += PES_header_data_length;
#if 0
	 if (PTS_DTS_flags == 2) pes_header_size += 5;
	 if (PTS_DTS_flags == 3) pes_header_size += 10;
	 if (ESCR_flag) pes_header_size += 6;
	 if (ES_rate_flag) pes_header_size += 3;
	 if (DSM_trick_mode_flag) pes_header_size += 1;
	 if (additional_copy_info_flag) pes_header_size += 1;
	 if (PES_CRC_flag) pes_header_size += 2;
	 if (PES_extension_flag) {
	    uint8 PES_private_data_flag = decompose_flag(packet->at(pes_header_size), 0, 1);
	    uint8 pack_header_field_flag = decompose_flag(packet->at(pes_header_size), 1, 1);
	    uint8 program_packet_sequence_counter_flag = decompose_flag(packet->at(pes_header_size), 2, 1);
	    uint8 P_STD_buffer_flag = decompose_flag(packet->at(pes_header_size), 3, 1);
	    uint8 PES_extension_flag_2 = decompose_flag(packet->at(pes_header_size), 7, 1);
	    pes_header_size += 1;

	    if (PES_private_data_flag) pes_header_size += 128/8;
	    if (pack_header_field_flag) {
	       uint8 pack_field_length = packet->at(pes_header_size);
	       pes_header_size += 1;
	       pes_header_size += pack_field_length;
	    }
	    if (program_packet_sequence_counter_flag) pes_header_size += 2;
	    if (P_STD_buffer_flag) pes_header_size += 2;
	    if (PES_extension_flag_2) {
	       uint8 PES_extenstion_field_length = packet->at(pes_header_size) & 0x7f;
	       pes_header_size += PES_extenstion_field_length;
	    }
	 }
#endif
      }

      if (packet->PES_packet_length() == 0) {
	 packet->setDataPointer(pes_header_size);
	 last_packet_start_time.set(packet->packet_start_time);
	 packet->headerWasParsed();
	 return true;
      }
      int told_length = packet->headerSize() + packet->PES_packet_length();
      int actual_length = packet->length();
      if (actual_length >= told_length) {
	 if (actual_length > told_length) {
	    ;
	 }
	 packet->data_completed = true;
	 packet->setDataPointer(pes_header_size);
	 last_packet_start_time.set(packet->packet_start_time);
	 packet->headerWasParsed();
	 return true;
      }
   } catch(ByteArrayOverflowException &e) {
   }
   packet->data_completed = false;
   return false;
}


/*
 * PESManager class
 */
PESManager::~PESManager() {
   for (PID2PESMap::iterator itr = pess.begin(); itr != pess.end(); itr++) {
      delete itr->second;
   }
}

void PESManager::clear() {
   for (PID2PESMap::iterator itr = pess.begin(); itr != pess.end(); itr++) {
      delete itr->second;
   }
   pess.clear();
}

void PESManager::setPES(uint16 pid, ISO13818_PacketizedElementaryStream *pes) {
   pess[pid] = pes;
}

ISO13818_PacketizedElementaryStream *PESManager::getPES(uint16 pid) {
   PID2PESMap::const_iterator itr = pess.find(pid);
   if (itr == pess.end()) return NULL;
   return itr->second;
}
