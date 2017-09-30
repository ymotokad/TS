#ifndef MPEGSTREAM_H
#define MPEGSTREAM_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_MPEGSTREAM
static const char *rcsid_MPEGStream = "@(#)$Id$";
#endif /* IMPLEMENTING_MPEGSTREAM */
#include <iostream>
#include "TSTypes.h"
#include "ByteArray.h"
#include "PacketizedElementaryStream.h"
#include <list>
#include "MPEGHeaders.h"

#define MPEG_STARTCODE_0	0x00
#define MPEG_STARTCODE_1	0x00
#define MPEG_STARTCODE_2	0x01

/*
 * MPEGStream
 */
class MPEGStream : public PacketizedElementaryStream {
 public:
   MPEGStream();
   virtual ~MPEGStream();
   ElementaryStream *readObject(); // MPEGHeader returned have to be deleted by caller side
   static void dumpHeader(MPEGHeader *h);

   static const int StartCode_Picture		= 0x00;
   static const int StartCode_Slice_Min		= 0x01;
   static const int StartCode_Slice_Max		= 0xaf;
   static const int StartCode_SequenceHeader	= 0xb3;
   static const int StartCode_Extension		= 0xb5;
   static const int StartCode_SequenceEnd	= 0xb7;
   static const int StartCode_GroupOfPictures	= 0xb8;
   static const int StartCode_AudioStream_Min	= 0xc0;
   static const int StartCode_AudioStream_Max	= 0xdf;
   static const int StartCode_VideoStream_Min	= 0xe0;
   static const int StartCode_VideoStream_Max	= 0xef;
 protected:
   bool isStartSignature(const ByteArray *bp, int off) const;
};



#endif /* MPEGSTREAM_H */
