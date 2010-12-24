/*
 * BitStream - 
 *
 * 
 *
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
   mydata = data;
   //logger->debug("BitStream::setBuffer(): registered mydata=0x%x", mydata);
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
   //std::cout << "DEBUG - bit_hield8: msb=0x" << std::hex << (int)msb << ", lsb=" << (int)lsb << std::endl;
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
   result = mydata->at(start / 8) & start_mask[start % 8];
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

int BitStream::append(const ByteArray &src) {
   ByteArrayBuffer *newdata = new ByteArrayBuffer(*mydata);
   if (newdata == NULL) return 0;
   const ByteArray *olddata = mydata;
   newdata->append(src);
   mydata = newdata;
   delete olddata;
   //logger->debug("BitStream::append(): deleted mydata=0x%x", olddata);
   //logger->debug("BitStream::append(): registered mydata=0x%x", mydata);
   return src.length();
}

void BitStream::dump(std::ostream *osp) const {
   hexdump(2, osp, *mydata, -1);
}


void BitStream::hexdump(int indent, std::ostream *osp, const ByteArray &buf, int offset, int len) {
   static const int bpl = 16;
   bool needEndl = false;

   *osp << std::hex;
   osp->unsetf(std::ios::showbase);
   if (len == -1 || offset + len > buf.length()) len = buf.length() - offset;
   for (int cnt = 0; cnt < len; cnt++) {
      if ((cnt % bpl) == 0) {
	 for (int i = 0; i < indent; i++) {
	    *osp << " ";
	 }
	 osp->width(2);
	 osp->fill('0');
	 *osp << cnt << ":";
      }
      if ((cnt % (bpl / 2)) == 0 && (cnt % bpl) != 0 ) {
	 *osp << " - ";
      } else {
	 *osp << " ";
      }
      osp->width(2);
      osp->fill('0');
      *osp << (unsigned int)buf.at(offset + cnt);
      if ((cnt % bpl) == bpl - 1) {
	 *osp << std::endl;
	 needEndl = false;
      } else {
	 needEndl = true;
      }
   }
   if (needEndl) {
      *osp << std::endl;
   }
}
