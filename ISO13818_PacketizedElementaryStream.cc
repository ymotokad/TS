/*
 * ISO13818_PacketizedElementaryStream - 
 *
 * 
 *
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
 * PES_Packet class
 */
class PES_Packet : public BitStream {
public:
   PES_Packet();
   uint32 packet_start_code_prefix() const;
   uint8 stream_id() const;
   uint32 PES_packet_length() const;
   int headerSize() const;
protected:
   void initobj();
   void initvars();
};

/*
 * Order of data
 */
#define PES_Packet_packet_start_code_prefix		0
#define PES_Packet_stream_id				1
#define PES_Packet_PES_packet_length			2
#define PES_Packet_PES_packet_data_byte			3

FIELDWIDTH_PREAMBLE(PES_Packet)
FIELDWIDTH_CONTENT(PES_Packet_packet_start_code_prefix,	24)
FIELDWIDTH_CONTENT(PES_Packet_stream_id,		8)
FIELDWIDTH_CONTENT(PES_Packet_PES_packet_length,	16)
FIELDWIDTH_CONTENT(PES_Packet_PES_packet_data_byte,	8)
FIELDWIDTH_POSTAMBLE(PES_Packet, initvars())

void PES_Packet::initvars() {
}

inline int PES_Packet::headerSize() const {
   return sizeofBufferBefore(PES_Packet_PES_packet_data_byte);
}

inline uint32 PES_Packet::packet_start_code_prefix() const {
   return bit_field32(PES_Packet_packet_start_code_prefix);
};

inline uint8 PES_Packet::stream_id() const {
   return bit_field8(PES_Packet_stream_id);
};

inline uint32 PES_Packet::PES_packet_length() const {
   return bit_field32(PES_Packet_PES_packet_length);
};


/*
 * ISO13818_PacketizedElementaryStream class
 */

ISO13818_PacketizedElementaryStream::ISO13818_PacketizedElementaryStream() : data_completed(false) {
}

ISO13818_PacketizedElementaryStream::~ISO13818_PacketizedElementaryStream() {
   PESPacketList::iterator it;
   for (it = blist.begin(); it != blist.end(); it++) {
      ByteArray *p = *it;
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

void ISO13818_PacketizedElementaryStream::put(const ProgramClock &clk, const ByteArray *packet) {

   if (blist.empty()) packet_start_time.set(clk);

   // append to blist
   blist.push_back(new ByteArray(*packet));

   // Map to PES
   PES_Packet pes;
   ByteArray *p = blist.front();
   assert(p->length() >= pes.headerSize());
   pes.setBuffer(new ByteArray(*p));
   //p->hexdump(0,&std::cout);
       
   if (pes.packet_start_code_prefix() != 0x000001) {
      clear();
   } else {
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
	 int len = length() - pes.headerSize();
	 int pes_packet_length = pes.PES_packet_length();
	 if (len >= pes_packet_length) {
	    data_completed = true;
	    remove(pes_header_size);
	 } else {
	    data_completed = false;
	 }
      } catch(ByteArrayOverflowException &e) {
	 data_completed = false;
      }
   }
}

int ISO13818_PacketizedElementaryStream::dataLength() const {
   assert(data_completed == true);
   return length();
}

void ISO13818_PacketizedElementaryStream::remove(int bytes) {
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

void ISO13818_PacketizedElementaryStream::clear() {
   while (!blist.empty()) {
      ByteArray *p = blist.front();
      blist.pop_front();
      delete p;
   }
}

int ISO13818_PacketizedElementaryStream::length() const {
   int len = 0;
   for (PESPacketList::const_iterator it = blist.begin(); it != blist.end(); it++) {
      ByteArray *p = *it;
      len += p->length();
   }

   return len;
}

ByteArray *ISO13818_PacketizedElementaryStream::read(int off, int bytes) const {
   if (off + bytes > length()) return NULL;

   int bytesToBeRead = bytes;
   ByteArrayBuffer *buf = new ByteArrayBuffer(1024, 1024);
   if (buf == NULL) return NULL;
   for (PESPacketList::const_iterator it = blist.begin(); it != blist.end(); it++) {
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
   return buf;
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
