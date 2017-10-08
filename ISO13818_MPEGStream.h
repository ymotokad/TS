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

#ifndef ISO13818_MPEGSTREAM_H
#define ISO13818_MPEGSTREAM_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_ISO13818_MPEGSTREAM
static const char *rcsid_ISO13818_MPEGStream = "@(#)$Id$";
#endif /* IMPLEMENTING_ISO13818_MPEGSTREAM */
#include <iostream>
#include "TSTypes.h"
#include "ByteArray.h"
#include "ISO13818_PacketizedElementaryStream.h"
#include <list>
#include "ISO13818_MPEGHeaders.h"

#define MPEG_STARTCODE_PREFIX0	((uint8)0x00)
#define MPEG_STARTCODE_PREFIX1	((uint8)0x00)
#define MPEG_STARTCODE_PREFIX2	((uint8)0x01)

/*
 * ISO13818_MPEGStream
 */
class ISO13818_MPEGStream : public ISO13818_PacketizedElementaryStream {
 public:
   ISO13818_MPEGStream();
   virtual ~ISO13818_MPEGStream();
   ElementaryStream *readObject(); // ISO13818_MPEGHeader returned have to be deleted by caller side
   static void dumpHeader(ISO13818_MPEGHeader *h);

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



#endif /* ISO13818_MPEGSTREAM_H */
