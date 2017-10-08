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

#ifndef BITSTREAM_H
#define BITSTREAM_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_BITSTREAM
static const char *rcsid_BitStream = "@(#)$Id$";
#endif /* IMPLEMENTING_BITSTREAM */
#include <iostream>
#include <stdio.h>
#include "TSTypes.h"
#include "ByteArrayBuffer.h"

class BitStream;

class BitStream {
 public:
   BitStream();
   virtual ~BitStream();
   int append(const ByteArray &src, int off = 0, int len = -1);
   void setBuffer(const ByteArray *data);
   void setBuffer(const BitStream &src, int offset = 0, int length = -1);
   uint8 byteAt(int idx) const; // ByteArrayOverflowException
   virtual void dump(std::ostream *osp) const;
   int bufferLength() const;
   bool bufferAllocated() const; // primary for debugging
   void dumpBitDistance(int len); // for debugging
   void hexdump(int indent, std::ostream *osp, int offset = 0, int len = -1) const;
   //static void hexdump(int indent, std::ostream *osp, const ByteArray &buff, int offset = 0, int len = -1);
   
 protected:
   const ByteArray *mydata;
   //const ByteArray *getBuffer() const; // use byteAt() instead
   
   // Bitfield methods. start starts eith 0. 
   bool bit_field1(int position) const;
   uint8 bit_field8(int position) const;
   uint16 bit_field16(int position) const;
   uint32 bit_field32(int position) const;
   uint64 bit_field64(int position) const;

   static void initializeBitDistance(const int *the_field_width, int length, int *bit_distance);
   void setBitDistance(const int *bit_distance);
   int sizeofBufferBefore(int position) const;

   const int *bit_distance;

 private:
   static const uint8 start_mask[];
   static const uint8 end_mask[];
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
   mydata->hexdump(indent, osp, offset, len);
}

/*
 * Bit distance table initializations
 */
#define FIELDWIDTH_PREAMBLE(cname) \
   static cname cname##_nullobj;\
   cname::cname() {\
      initobj();\
   }\
   void cname::initobj() {\
      static const int the_field_width[] = {
#define FIELDWIDTH_CONTENT(fieldname, width) \
         width, /* fieldname */
#define FIELDWIDTH_POSTAMBLE(cname, initfunc) \
      }; \
      static int *the_bit_distance = NULL;				\
   /* printf("%s::initobj(): _nullobj=0x%lx, this=0x%lx\n", #cname, (unsigned long)&cname##_nullobj, (unsigned long)this); */\
      if (this == &cname##_nullobj) {\
         int len = sizeof(the_field_width) / sizeof(the_field_width[0]);\
         the_bit_distance = new int[len + 1];\
	 /* printf("%s::initobj(): len=%d\n", #cname, len); */\
         initializeBitDistance(the_field_width, len, the_bit_distance);\
      }\
      /* assert(the_bit_distance != NULL);*/	\
      setBitDistance(the_bit_distance);\
      initfunc;\
   }

#endif /* BITSTREAM_H */
