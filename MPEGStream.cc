/*
 * MPEGStream - 
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
#define IMPLEMENTING_MPEGSTREAM
#include "MPEGStream.h"

/*
 * MPEGStream class
 */

MPEGStream::MPEGStream() : PacketizedElementaryStream(4) {
}

MPEGStream::~MPEGStream() {
}

bool MPEGStream::isStartSignature(const ByteArray *bp, int off) const {
   return bp->at(off + 0) == MPEG_STARTCODE_0 && bp->at(off + 1) == MPEG_STARTCODE_1 && bp->at(off + 2) == MPEG_STARTCODE_2;
}


ElementaryStream *MPEGStream::readObject() {
   if (buffer.empty()) return NULL;
   ByteArray *buf = buffer.front();
   buffer.pop_front();

   MPEGHeader *ret = NULL;
   assert(isStartSignature(buf, 0));
   switch (buf->at(3)) {
   case StartCode_Picture:
      ret = new MPEGPictureHeader();
      break;
   case StartCode_GroupOfPictures:
      ret = new MPEGGroupOfPictures();
      break;
   default:
      ret = new MPEGHeader();
      break;
   }
   ret->setBuffer(*buf);
   delete buf;

   return ret;
}

void MPEGStream::dumpHeader(MPEGHeader *obj) {
   if (obj != NULL) {
      const char *tag = NULL;
      uint8 c = obj->start_code();
      if(c == MPEGStream::StartCode_Picture) {
	 std::cout << " --- PES Picture --- " << std::endl;
	 MPEGPictureHeader *hp = (MPEGPictureHeader *)obj;
	 printf("    temporal seq num = %d\n", hp->temporal_sequence_number());
	 printf("    frame type       = 0x%02x\n", hp->frame_type());
	 printf("    VBV delay        = %d\n", hp->VBV_delay());
      } else if(c == MPEGStream::StartCode_GroupOfPictures) {
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
	 if(c == MPEGStream::StartCode_SequenceHeader) {
	    tag = "Sequence Header";
	 } else if(c == MPEGStream::StartCode_SequenceEnd) {
	    tag = "Sequence End";
	 } else if(c == MPEGStream::StartCode_Picture) {
	    tag = "Picture";
	 } else if(MPEGStream::StartCode_Slice_Min <= c && c <= MPEGStream::StartCode_Slice_Max) {
	    //tag = "Slice";
	 } else if(c == MPEGStream::StartCode_SequenceHeader) {
	    tag = "Sequence Header";
	 } else if(c == MPEGStream::StartCode_Extension) {
	    tag = "Extension";
	 } else if(MPEGStream::StartCode_AudioStream_Min <= c && c <= MPEGStream::StartCode_AudioStream_Max) {
	    tag = "Audio Stream";
	 } else if(MPEGStream::StartCode_VideoStream_Min <= c && c <= MPEGStream::StartCode_VideoStream_Max) {
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
