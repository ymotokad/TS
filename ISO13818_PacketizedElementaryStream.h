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

#ifndef ISO13818_PACKETIZEDELEMENTARYSTREAM_H
#define ISO13818_PACKETIZEDELEMENTARYSTREAM_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_ISO13818_PACKETIZEDELEMENTARYSTREAM
static const char *rcsid_ISO13818_PacketizedElementaryStream = "@(#)$Id$";
#endif /* IMPLEMENTING_ISO13818_PACKETIZEDELEMENTARYSTREAM */
#include <assert.h>
#include <iostream>
#include "TSTypes.h"
#include <list>
#include <map>
#include "ElementaryStream.h"
#include "ISO13818_SystemClock.h"

typedef std::list<ByteArray *> PESDataList;

/*
 * ISO13818_PES_Packet class
 */
class ISO13818_PES_Packet : public BitStream {
public:
   ISO13818_PES_Packet();
   ISO13818_PES_Packet(const ProgramClock &src);
   virtual ~ISO13818_PES_Packet();
   uint32 packet_start_code_prefix() const;
   uint8 stream_id() const;
   uint32 PES_packet_length() const;
   int headerSize() const;

   void put(const ByteArray &data);

   void setDataPointer(int pos) {
      packet_data_pointer = pos;
   }
   void moveDataPointer(int rel) {
      packet_data_pointer += rel;
   }
   bool wasHeaderParsed() const {
      return header_parsed_p;
   }
   void headerWasParsed() {
      header_parsed_p = true;
   }
   ByteArray *read(int n);
   int length() const;
   int dataLength() const {
      return length() - packet_data_pointer;
   }
   uint8 at(int idx) const {	// This can access only the first blist
      return blist.front()->at(idx);
   }
   int find3(int off, uint8 c1, uint8 c2, uint8 c3) const;
   //void remove(int bytes);
   //void clear();

   bool data_completed;
   const ProgramClock packet_start_time;
protected:
   int packet_data_pointer;
   bool header_parsed_p;
   
   void shrink_blist();
   void initobj();
   void initvars();
   PESDataList blist;
};

/*
 * Order of data
 */
#define ISO13818_PES_Packet_packet_start_code_prefix		0
#define ISO13818_PES_Packet_stream_id				1
#define ISO13818_PES_Packet_PES_packet_length			2
#define ISO13818_PES_Packet_PES_packet_data_byte		3

inline int ISO13818_PES_Packet::headerSize() const {
   return sizeofBufferBefore(ISO13818_PES_Packet_PES_packet_data_byte);
}

inline uint32 ISO13818_PES_Packet::packet_start_code_prefix() const {
   return bit_field32(ISO13818_PES_Packet_packet_start_code_prefix);
};

inline uint8 ISO13818_PES_Packet::stream_id() const {
   return bit_field8(ISO13818_PES_Packet_stream_id);
};

inline uint32 ISO13818_PES_Packet::PES_packet_length() const {
   return bit_field32(ISO13818_PES_Packet_PES_packet_length);
};



/*
 * ISO13818_PacketizedElementaryStream
 */
typedef std::list<ISO13818_PES_Packet *> PESPacketList;
class ISO13818_PacketizedElementaryStream {
 public:
   ISO13818_PacketizedElementaryStream(int streamtype);
   virtual ~ISO13818_PacketizedElementaryStream();
   void put(const ProgramClock &clk, const ByteArray *packet);
   void startUnit(const ProgramClock &clk, const ByteArray *packet);
   void putUnit(const ByteArray *packet);
   virtual ElementaryStream *readObject() = 0; // ByteArray returned have to be deleted by caller side
   int getStreamType() const {
      return streamtype;
   }
   ProgramClock getPacketStartTime() const {
      return last_packet_start_time;
   }
   static const int StreamType_MPEG	= 1;
   static const int StreamType_ADTS	= 2;
   static const int StreamType_Caption	= 3;
 protected:
   bool isReadable();
   PESPacketList plist;
   int packet_data_pointer;
   ProgramClock last_packet_start_time;
   static const uint8 stream_id_program_stream_map	= 0xbc;
   static const uint8 stream_id_private_stream_1	= 0xbd;
   static const uint8 stream_id_padding_stream		= 0xbe;
   static const uint8 stream_id_private_stream_2	= 0xbf;
   static const uint8 stream_id_audio_stream_mask	= 0xe0;
   static const uint8 stream_id_audio_stream_bits	= 0xc0;
   static const uint8 stream_id_video_stream_mask	= 0xf0;
   static const uint8 stream_id_video_stream_bits	= 0xe0;
   static const uint8 stream_id_ECM_stream		= 0xf0;
   static const uint8 stream_id_EMM_stream		= 0xf1;
   static const uint8 stream_id_DSMCC_stream		= 0xf2;
   static const uint8 stream_id_IEC13522_stream		= 0xf3;
   static const uint8 stream_id_H222_1_type_A		= 0xf4;
   static const uint8 stream_id_H222_1_type_B		= 0xf5;
   static const uint8 stream_id_H222_1_type_C		= 0xf6;
   static const uint8 stream_id_H222_1_type_D		= 0xf7;
   static const uint8 stream_id_H222_1_type_E		= 0xf8;
   static const uint8 stream_id_ancillary_stream	= 0xf9;
   static const uint8 stream_id_SL_packetized_stream	= 0xfa;
   static const uint8 stream_id_FlexMux_stream		= 0xfb;
   static const uint8 stream_id_program_stream_directory = 0xff;
   const int streamtype;
 private:
   //int data_length;
};



/*
 * PESManager class
 */
typedef std::map<uint16, ISO13818_PacketizedElementaryStream *, std::less<int> > PID2PESMap;
class PESManager {
public:
   ~PESManager();
   void clear();
   void setPES(uint16 pid, ISO13818_PacketizedElementaryStream *pes);
   ISO13818_PacketizedElementaryStream *getPES(uint16 pid);
private:
   PID2PESMap pess;
};



#endif /* ISO13818_PACKETIZEDELEMENTARYSTREAM_H */
