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
#define IMPLEMENTING_BITSTREAM
#include "BitStream.h"

const uint8 BitStream::start_mask[] = {
   0xff,
   0x7f,
   0x3f,
   0x1f,
   0x0f,
   0x07,
   0x03,
   0x01
};

const uint8 BitStream::end_mask[] = {
   0x80,
   0xc0,
   0xe0,
   0xf0,
   0xf8,
   0xfc,
   0xfe,
   0xff
};

/*
 * constructors 
 */
BitStream::BitStream() {
   mydata = NULL;
}


/*
 * destructors 
 */
BitStream::~BitStream() {
   if (mydata != NULL) {
      //logger->debug("BitStream::~BitStream(): deleting mydata=0x%x", mydata);
      delete mydata;
   }
}

/*
 * Other Methods
 */
void BitStream::initializeBitDistance(const int *field_width, int length, int *the_bit_distance) {
   int distance = 0;
   for (int i = 0; i < (length + 1); i++) {
      the_bit_distance[i] = distance;
      distance += field_width[i];
   }
}

void BitStream::dumpBitDistance(int len) {
   int distance = 0;
   std::cout << "DEBUG: dumpBitDistance" << std::endl;
   for (int i = 0; i < (len + 1); i++) {
      std::cout << "  " << i << ": " << bit_distance[i] << std::endl;
   }
}

void BitStream::setBitDistance(const int *the_bit_distance) {
   bit_distance = the_bit_distance;
}

void BitStream::setBuffer(const ByteArray *data) {
   assert(mydata != data);
   if (mydata != NULL) delete mydata;
   mydata = data;
   //logger->debug("BitStream::setBuffer(): registered mydata=0x%x", mydata);
}

void BitStream::setBuffer(const BitStream &src, int off, int len) {
   assert(mydata != src.mydata);
   if (mydata != NULL) delete mydata;
   mydata = src.mydata->subarray(off, len);
}

int BitStream::sizeofBufferBefore(int pos) const {
   int distance = bit_distance[pos];
   return (distance + 7) / 8;
}

bool BitStream::bit_field1(int pos) const {
   int start = bit_distance[pos];
   int end = bit_distance[pos + 1] - 1;
   assert(start == end);
   return (mydata->at(start / 8) & start_mask[start % 8] & end_mask[start % 8]) != 0;
}

uint8 BitStream::bit_field8(int pos) const {
   int start = bit_distance[pos];
   int end = bit_distance[pos + 1] - 1;
   assert(start <= end);
   assert(end - start <= 8);
   //std::cout << "DEBUG - bit_hield8: start=" << std::dec << start << ", end=" << end << std::endl;
   uint8 msb, lsb;
   msb = mydata->at(start / 8) & start_mask[start % 8];
   lsb = mydata->at(end / 8) & end_mask[end % 8];
   //std::cout << "DEBUG - bit_hield8: msb=0x" << std::hex << (int)msb << ", lsb=0x" << (int)lsb << std::endl;
   if (start / 8 == end / 8) {
      return (msb & lsb) >> (7 - (end % 8));
   }
   int result = msb << 8 | lsb;
   //std::cout << "DEBUG - bit_hield8: result=0x" << std::hex << (int)(result >> (7 - (end % 8))) << std::endl;
   return (uint8)(result >> (7 - (end % 8)));
}

uint16 BitStream::bit_field16(int pos) const {
   int start = bit_distance[pos];
   int end = bit_distance[pos + 1] - 1;
   //std::cout << "DBG1: start=" << start << ", end=" << end << std::endl;
   assert(start <= end);
   assert(end - start <= 16);
   int result = 0;
   result = 0;
   for (int i = start / 8; i <= end / 8; i++) {
      result <<= 8;
      uint8 dt = mydata->at(i);
      if (i == start / 8) {
	 dt &= start_mask[start % 8];
      }
      if (i == end / 8) {
	 dt &= end_mask[end % 8];
      }
      result |= dt;
   }
   return (uint16)(result >> (7 - (end % 8)));
}

uint32 BitStream::bit_field32(int pos) const {
   int start = bit_distance[pos];
   int end = bit_distance[pos + 1] - 1;
   //std::cout << "DBG1: start=" << start << ", end=" << end << std::endl;
   assert(start <= end);
   assert(end - start <= 32);
   long result = 0;
   assert(sizeof(long) >= sizeof(uint32));
   result = 0;
   for (int i = start / 8; i <= end / 8; i++) {
      result <<= 8;
      uint8 dt = mydata->at(i);
      if (i == start / 8) {
	 dt &= start_mask[start % 8];
      }
      if (i == end / 8) {
	 dt &= end_mask[end % 8];
      }
      result |= dt;
   }
   return (uint32)(result >> (7 - (end % 8)));
}

uint64 BitStream::bit_field64(int pos) const {
   int start = bit_distance[pos];
   int end = bit_distance[pos + 1] - 1;
   //std::cout << "DBG1: start=" << start << ", end=" << end << std::endl;
   assert(start <= end);
   assert(end - start <= 64);
   uint64 result = 0;
   for (int i = start / 8; i <= end / 8; i++) {
      result <<= 8;
      uint8 dt = mydata->at(i);
      if (i == start / 8) {
	 dt &= start_mask[start % 8];
      }
      if (i == end / 8) {
	 dt &= end_mask[end % 8];
      }
      result |= dt;
   }
   return (uint64)(result >> (7 - (end % 8)));
}

int BitStream::append(const ByteArray &src, int off, int len) {
   ByteArrayBuffer *newdata = new ByteArrayBuffer(*mydata);
   if (newdata == NULL) return 0;
   const ByteArray *olddata = mydata;
   if (off == 0 && len == -1) {
      newdata->append(src);
   } else {
      ByteArray *tmp = src.subarray(off, len);
      newdata->append(*tmp);
      delete tmp;
   }
   mydata = newdata;
   delete olddata;
   //logger->debug("BitStream::append(): deleted mydata=0x%x", olddata);
   //logger->debug("BitStream::append(): registered mydata=0x%x", mydata);
   return src.length();
}

void BitStream::dump(std::ostream *osp) const {
   mydata->hexdump(2, osp, -1);
}

