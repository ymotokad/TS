#ifndef BITSTREAM_H
#define BITSTREAM_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_BITSTREAM
static const char *rcsid_BitStream = "@(#)$Id$";
#endif /* IMPLEMENTING_BITSTREAM */
#include <iostream>
#include "TSTypes.h"
#include "ByteArrayBuffer.h"

class BitStream {
 public:
   BitStream();
   virtual ~BitStream();
   int append(const ByteArray &src);
   void setBuffer(const ByteArray *data);
   uint8 byteAt(int idx) const; // ByteArrayOverflowException
   virtual void dump(std::ostream *osp) const;
   int bufferLength() const;
   bool bufferAllocated() const; // primary for debugging
   void dumpBitDistance(int len); // for debugging
   void hexdump(int indent, std::ostream *osp, int offset = 0, int len = -1) const;
   
 protected:
   //const ByteArray *getBuffer() const; // use byteAt() instead
   
   // Bitfield methods. start starts eith 0. 
   bool bit_field1(int position) const;
   uint8 bit_field8(int position) const;
   uint16 bit_field16(int position) const;

   static void hexdump(int indent, std::ostream *osp, const ByteArray &buff, int offset = 0, int len = -1);

   static void initializeBitDistance(const int *the_field_width, int length, int *the_bit_distance);
   void setBitDistance(const int *the_bit_distance);
   int sizeofBufferBefore(int position) const;

   const int *bit_distance;

 private:
   static const uint8 start_mask[];
   static const uint8 end_mask[];
   const ByteArray *mydata;
};


inline uint8 BitStream::byteAt(int idx) const {
   return mydata->at(idx);
}

inline int BitStream::bufferLength() const {
   return mydata->length();
}

inline bool BitStream::bufferAllocated() const {
   return mydata != NULL;
}

inline void BitStream::hexdump(int indent, std::ostream *osp, int offset, int len) const {
   hexdump(indent, osp, *mydata, offset, len);
}


#endif /* BITSTREAM_H */
