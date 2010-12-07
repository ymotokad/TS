/*
 * BitStreamSyntax - 
 *
 * 
 *
 */
static char rcsid[] = "@(#)$Id$";
#ifdef __GNUG__
#pragma implementation
#endif
#include <assert.h>
#define IMPLEMENTING_TRANSPORTPACKET
#include "BitStreamSyntax.h"

const uint8 BitStreamSyntax::start_mask[] = {
   0xff,
   0x7f,
   0x3f,
   0x1f,
   0x0f,
   0x07,
   0x03,
   0x01
};

const uint8 BitStreamSyntax::end_mask[] = {
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
BitStreamSyntax::BitStreamSyntax() {
   mydata = NULL;
   fullLength = 0;
}


/*
 * destructors 
 */
BitStreamSyntax::~BitStreamSyntax() {
   if (mydata != NULL) {
      delete mydata;
   }
}

/*
 * Other Methods
 */
void BitStreamSyntax::initializeBitDistance(const int *field_width, int length, int *the_bit_distance) {
   int distance = 0;
   for (int i = 0; i < (length + 1); i++) {
      the_bit_distance[i] = distance;
      distance += field_width[i];
   }
}

void BitStreamSyntax::setBitDistance(const int *the_bit_distance) {
   bit_distance = the_bit_distance;
}

void BitStreamSyntax::setBuffer(const ByteArray *data) {
   mydata = data;
}

const ByteArray *BitStreamSyntax::getBuffer() const {
   return mydata;
}

void BitStreamSyntax::setFullLength(int len) {
   fullLength = len;
}

bool BitStreamSyntax::isCompleted() const {
   assert(fullLength != 0);
   return fullLength <= mydata->length();
}


int BitStreamSyntax::sizeofBufferBefore(int pos) const {
   int distance = bit_distance[pos];
   return (distance + 7) / 8;
}

bool BitStreamSyntax::bit_field1(int pos) const {
   int start = bit_distance[pos];
   int end = bit_distance[pos + 1] - 1;
   assert(start == end);
   return (mydata->at(start / 8) & start_mask[start % 8] & end_mask[start % 8]) != 0;
}

uint8 BitStreamSyntax::bit_field8(int pos) const {
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

uint16 BitStreamSyntax::bit_field16(int pos) const {
   int start = bit_distance[pos];
   int end = bit_distance[pos + 1] - 1;
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


int BitStreamSyntax::readin(std::istream *isp, ByteArrayBuffer *buffer, int length) {
   uint8 buff[length];

   //std::cout << "DBG: readin(isp, packet, ength=" << length << ")";
   isp->read((char *)buff, length);
   buffer->append(buff, length);
   //std::cout << "...read " << std::dec << isp->gcount() << " bytes" << std::endl;
   return isp->gcount();
}

int BitStreamSyntax::append(const ByteArray &src) {
   ByteArrayBuffer *newdata = new ByteArrayBuffer(*mydata);
   if (newdata == NULL) return 0;
   const ByteArray *olddata = mydata;
   newdata->append(src);
   mydata = newdata;
   delete olddata;
   return src.length();
}
