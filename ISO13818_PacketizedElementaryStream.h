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
#include "SystemClock.h"

typedef std::list<ByteArray *> PESPacketList;

/*
 * ISO13818_PacketizedElementaryStream
 */
class ISO13818_PacketizedElementaryStream {
 public:
   ISO13818_PacketizedElementaryStream();
   virtual ~ISO13818_PacketizedElementaryStream();
   virtual void put(const ProgramClock &clk, const ByteArray *packet);
   virtual ElementaryStream *readObject() = 0; // ByteArray returned have to be deleted by caller side
   ProgramClock getPacketStartTime() const;
 protected:
   int length() const;
   void remove(int bytes);
   void clear();
   ByteArray *read(int off, int n) const;
   int dataLength() const;
   bool data_completed;
   ProgramClock packet_start_time;
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
 private:
   PESPacketList blist;
   int data_length;
};

inline ProgramClock ISO13818_PacketizedElementaryStream::getPacketStartTime() const {
   return packet_start_time;
}



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
