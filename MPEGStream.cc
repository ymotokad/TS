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

MPEGStream::MPEGStream() : carryover(NULL) {
}

MPEGStream::~MPEGStream() {
   if (carryover != NULL) delete carryover;
}

void MPEGStream::put(const ByteArray *packet) {
   int head, i;
   bool lastDataValid = true;
   const ByteArray *src;
   if (carryover == NULL) {
      i = 0;
      lastDataValid = false;
      carryover = new ByteArrayBuffer(*packet);
   } else {
      i = carryover->length() - 4;
      carryover->append(*packet);
   }
   src = carryover;
   head = 0;

   while (i < src->length() - 4) {
      if (isStartCode(src, i)) {
	 ByteArrayBuffer *buf;
	 if (lastDataValid) {
	    buf = new ByteArrayBuffer(1024, 1024);
	    buf->append(src->part(head, i - head), i - head);
	    buffer.push_back(buf);
	    //printf("A: head=%d, i=0x%x, src=", head, i); src->hexdump(0, &std::cout, 0, -1);
	    //printf("   ---pushed=");buf->hexdump(0, &std::cout, 0, -1);
	 } else {
	    lastDataValid = true;
	    //printf("B: head=%d, i=0x%x, src=", head, i); src->hexdump(0, &std::cout, 0, -1);
	 }
	 head = i;
	 i += 4;
      } else {
	 i++;
      }
   }

   if (lastDataValid) {
      ByteArray *p = carryover->subarray(head);
      delete carryover;
      carryover = new ByteArrayBuffer(*p);
      //printf("DBG: MPEGStream::put() exit, carryover->length()=0x%x\n", carryover->length());
      delete p;
   } else {
      delete carryover;
      carryover = NULL;
      //printf("DBG: MPEGStream::put() exit, carryover->length()=0\n");
   }
}

MPEGHeader *MPEGStream::readObject() {
   if (buffer.empty()) return NULL;
   ByteArray *buf = buffer.front();
   buffer.pop_front();

   MPEGHeader *ret = NULL;
   assert(isStartCode(buf, 0));
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
