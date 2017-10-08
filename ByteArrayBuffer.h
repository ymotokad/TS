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

class Room {
public:
   Room(int size);
   ~Room();
   uint8 *buffer();
   void extend(int current_size, int total_size);
   int incr_refcnt();
   int decr_refcnt();
protected:
   uint8 *data;
   int refcnt;
};


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
