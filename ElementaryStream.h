#ifndef ELEMENTARYSTREAM_H
#define ELEMENTARYSTREAM_H
#ifdef IMPLEMENTING_ELEMENTARYSTREAM
static const char *rcsid_ElementaryStream = "@(#)$Id$";
#endif /* IMPLEMENTING_ELEMENTARYSTREAM */
#include <iostream>
#include "TSTypes.h"
#include "BitStream.h"

/*
 * ElementaryStream
 */
class ElementaryStream : public BitStream {
 public:
   int getStreamType() const {
      return streamtype;
   }
   static const int StreamType_MPEG	= 1;
   static const int StreamType_ADTS	= 2;
   static const int StreamType_Caption	= 3;
 protected:
   void setStreamType(int type) {
      streamtype = type; 
   }
   int streamtype;
};


#endif /* ELEMENTARYSTREAM_H */
