// This may look like C code, but it is really -*- C++ -*-
/*
 * ByteArrayBuffer - Modifiable byte array class
 *
 * 
 *
 */
#ifndef	BYTEARRAYBUFFER_H
#define	BYTEARRAYBUFFER_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_BYTEARRAY
static char ByteArrayBuffer_rcsid[] = "@(#)$Id$";
#endif

#include "ByteArray.h"

#define BYTEARRAY_INITIALCAPACITY_DEFAULT	192
#define BYTEARRAY_CAPACITYINCREMENT_DEFAULT	192

class ByteArrayBuffer : public ByteArray {
public:
   ByteArrayBuffer(int initialCapacity = BYTEARRAY_INITIALCAPACITY_DEFAULT,
	     int capacityIncrement = BYTEARRAY_CAPACITYINCREMENT_DEFAULT);
   ByteArrayBuffer(const ByteArrayBuffer& src);
   ByteArrayBuffer(const ByteArray& src);
   ByteArrayBuffer(const uint8 *src, int length);
   ~ByteArrayBuffer();

   ByteArrayBuffer& operator = (const ByteArray& src);
   ByteArrayBuffer& operator = (const uint8 *src);

   ByteArrayBuffer& append(const ByteArray& src);
   ByteArrayBuffer& append(const uint8 *src);
   ByteArrayBuffer& append(const uint8 *src, int length);
   ByteArrayBuffer& append(uint8 src);
   ByteArrayBuffer& operator += (const ByteArray& src);
   ByteArrayBuffer& operator += (const uint8 *src);
   ByteArrayBuffer& operator += (uint8 src);
private:
   int capacity;
   int capacityIncrement;
   void extendRoom(int lengthNeeded);
   ByteArrayBuffer& copyin(const uint8 *src, int length);
};

inline ByteArrayBuffer& ByteArrayBuffer::operator += (const ByteArray& src) {
   return append(src);
}
inline ByteArrayBuffer& ByteArrayBuffer::operator += (const uint8 *src) {
   return append(src);
}
inline ByteArrayBuffer& ByteArrayBuffer::operator += (uint8 src) {
   return append(src);
}

#endif /* BYTEARRAYBUFFER_H */
