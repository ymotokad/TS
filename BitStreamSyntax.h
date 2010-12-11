#ifndef BITSTREAMSYNTAX_H
#define BITSTREAMSYNTAX_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_BITSTREAMSYNTAX
static const char *rcsid_BitStreamSyntax = "@(#)$Id$";
#endif /* IMPLEMENTING_BITSTREAMSYNTAX */
#include "ByteArrayBuffer.h"

class BitStreamSyntax {
 public:
   BitStreamSyntax();
   virtual ~BitStreamSyntax();

   void setBuffer(const ByteArray *data);
   const ByteArray *getBuffer() const;
   bool isCompleted() const;
   virtual int append(const ByteArray &src);
 protected:
   // Bitfield methods. start starts eith 0. 
   bool bit_field1(int position) const;
   uint8 bit_field8(int position) const;
   uint16 bit_field16(int position) const;
   
   void setFullLength(int len);
   static int readin(std::istream *inputstream, ByteArrayBuffer *buffer, int size);
   static void initializeBitDistance(const int *the_field_width, int length, int *the_bit_distance);
   void setBitDistance(const int *the_bit_distance);
   int sizeofBufferBefore(int position) const;

   const int *bit_distance;

 private:
   static const uint8 start_mask[];
   static const uint8 end_mask[];
   const ByteArray *mydata;
   int fullLength;
};


#endif /* BITSTREAMSYNTAX_H */
