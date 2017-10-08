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
#define IMPLEMENTING_ISO13818_MPEGSTREAM
#include "ISO13818_MPEGStream.h"

/*
 * ISO13818_MPEGStream class
 */

ISO13818_MPEGStream::ISO13818_MPEGStream() : ISO13818_PacketizedElementaryStream(StreamType_MPEG) {
}

ISO13818_MPEGStream::~ISO13818_MPEGStream() {
}


ElementaryStream *ISO13818_MPEGStream::readObject() {
   if (!isReadable()) return NULL;

   // Slice the first sequence
   ISO13818_PES_Packet *packet = plist.front();
   int pos = packet->find3(4, MPEG_STARTCODE_PREFIX0, MPEG_STARTCODE_PREFIX1, MPEG_STARTCODE_PREFIX2);
   if (pos < 0) {
      if (packet->data_completed) {
	 pos = packet->dataLength();
	 if (pos < 4) { // Minimum MPEG sequence length
	    plist.pop_front();
	    delete packet;
	    return NULL;
	 }
      } else {
	 return NULL;
      }
   }
   
   ByteArray *buf = packet->read(pos);
   ISO13818_MPEGHeader *ret = NULL;
   assert(buf->at(0) == MPEG_STARTCODE_PREFIX0 && buf->at(1) == MPEG_STARTCODE_PREFIX1 && buf->at(2) == MPEG_STARTCODE_PREFIX2);
   switch (buf->at(3)) {
   case StartCode_Picture:
      ret = new MPEGPictureHeader();
      break;
   case StartCode_GroupOfPictures:
      ret = new MPEGGroupOfPictures();
      break;
   default:
      ret = new ISO13818_MPEGHeader();
      break;
   }
   ret->setBuffer(*buf);
   delete buf;

   if (packet->data_completed && packet->dataLength() == 0) {
      plist.pop_front();
      delete packet;
   }

   return ret;
}

void ISO13818_MPEGStream::dumpHeader(ISO13818_MPEGHeader *obj) {
   if (obj != NULL) {
      const char *tag = NULL;
      uint8 c = obj->start_code();
      if(c == ISO13818_MPEGStream::StartCode_Picture) {
	 std::cout << " --- PES Picture --- " << std::endl;
	 MPEGPictureHeader *hp = (MPEGPictureHeader *)obj;
	 printf("    temporal seq num = %d\n", hp->temporal_sequence_number());
	 printf("    frame type       = 0x%02x\n", hp->frame_type());
	 printf("    VBV delay        = %d\n", hp->VBV_delay());
      } else if(c == ISO13818_MPEGStream::StartCode_GroupOfPictures) {
	 std::cout << " --- PES GOP --- " << std::endl;
	 MPEGGroupOfPictures *hp = (MPEGGroupOfPictures *)obj;
	 printf("    drop_frame_flag = %d\n", hp->drop_frame_flag());
	 printf("    hour           = %d\n", hp->hour());
	 printf("    minute         = %d\n", hp->minute());
	 printf("    second         = %d\n", hp->second());
	 printf("    frame          = %d\n", hp->frame());
	 printf("    closed_GOP     = %d\n", hp->closed_GOP());
	 printf("    broken_GOP     = %d\n", hp->broken_GOP());
      } else {
	 if(c == ISO13818_MPEGStream::StartCode_SequenceHeader) {
	    tag = "Sequence Header";
	 } else if(c == ISO13818_MPEGStream::StartCode_SequenceEnd) {
	    tag = "Sequence End";
	 } else if(c == ISO13818_MPEGStream::StartCode_Picture) {
	    tag = "Picture";
	 } else if(ISO13818_MPEGStream::StartCode_Slice_Min <= c && c <= ISO13818_MPEGStream::StartCode_Slice_Max) {
	    //tag = "Slice";
	 } else if(c == ISO13818_MPEGStream::StartCode_SequenceHeader) {
	    tag = "Sequence Header";
	 } else if(c == ISO13818_MPEGStream::StartCode_Extension) {
	    tag = "Extension";
	 } else if(ISO13818_MPEGStream::StartCode_AudioStream_Min <= c && c <= ISO13818_MPEGStream::StartCode_AudioStream_Max) {
	    tag = "Audio Stream";
	 } else if(ISO13818_MPEGStream::StartCode_VideoStream_Min <= c && c <= ISO13818_MPEGStream::StartCode_VideoStream_Max) {
	    tag = "Video Stream";
	 } else {
	    tag = "Others";
	 }
	 if (tag != NULL && 0) {
	    std::cout << " --- PES " << tag << " --- " << std::endl;
	    int n = obj->bufferLength();
	    if (n > 32) n = 32;
	    obj->hexdump(3, &std::cout, 0, n);
	 }
      }
   }
}
