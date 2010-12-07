/*
 * ByteArrayBuffer - 
 *
 *
 */
static char rcsid[] = "@(#)$Id$";
#ifdef __GNUG__
#pragma implementation
#endif
#define IMPLEMENTING_BYTEARRAYBYFFER
#include <string.h>
#include "ByteArrayBuffer.h"

static uint8 *
allocateRoom(int size) {
   uint8 *p;
   if ((p = new uint8[size]) == NULL) {
      ;// Error handling here.
   }
   return p;
}

inline int
suitableCapacity(int capacity, int capacityIncrement, int length) {
   int n = ((length - capacity) / capacityIncrement) + 1;
   return capacity + (capacityIncrement * n);
}

/*
 * constructors 
 */

ByteArrayBuffer::ByteArrayBuffer(int initialCapacity, int capacityIncrement) : capacityIncrement(capacityIncrement) {
   capacity = initialCapacity;
   if (capacity < 1) capacity = 1;
   tail = 0;
   room = allocateRoom(capacity);
}

ByteArrayBuffer::ByteArrayBuffer(const ByteArrayBuffer& src) {
   capacity = src.capacity;
   capacityIncrement = src.capacityIncrement;
   tail = src.tail;
   room = allocateRoom(capacity);
   memcpy(room, src.room, tail);
}

ByteArrayBuffer::ByteArrayBuffer(const ByteArray& src) {
   capacity = ((src.length() / BYTEARRAY_INITIALCAPACITY_DEFAULT) + 1) * BYTEARRAY_INITIALCAPACITY_DEFAULT;
   capacityIncrement = BYTEARRAY_CAPACITYINCREMENT_DEFAULT;
   tail = src.length();
   room = allocateRoom(capacity);
   memcpy(room, src.part(0, tail), tail);
}

ByteArrayBuffer::ByteArrayBuffer(const uint8 *src, int length) {
   capacityIncrement = BYTEARRAY_CAPACITYINCREMENT_DEFAULT;
   capacity = suitableCapacity(BYTEARRAY_INITIALCAPACITY_DEFAULT,
			       capacityIncrement, length);
   room = allocateRoom(capacity);
   memcpy(room, src, length);
   tail = length;
}


/*
 * destructor
 */
ByteArrayBuffer::~ByteArrayBuffer() {
   delete room;
}


/*
 * operators
 */

ByteArrayBuffer&
ByteArrayBuffer::operator = (const ByteArray& target) {
   if (this == &target) return *this;
   return copyin(target.part(0, target.length()), target.length());
}

ByteArrayBuffer&
ByteArrayBuffer::operator = (const uint8 *target) {
   if (this->room == target) return *this;
   return copyin(target, strlen((const char *)target));
}


/*
 * other methods
 */

void
ByteArrayBuffer::extendRoom(int lengthNeeded) {
   int cap = suitableCapacity(capacity, capacityIncrement, lengthNeeded);
   uint8 *r = allocateRoom(cap);
   memcpy(r, room, length());
   delete room;
   room = r;
   capacity = cap;
}

ByteArrayBuffer&
ByteArrayBuffer::append(const ByteArray& src) {
   if (capacity < (length() + src.length())) {
      extendRoom(length() + src.length());
   }
   memcpy(&room[tail], src.part(0, src.length()), src.length());
   tail += src.length();
   return *this;
}

ByteArrayBuffer&
ByteArrayBuffer::append(const uint8 *src) {
   return append(src, strlen((const char *)src));
}

ByteArrayBuffer&
ByteArrayBuffer::append(const uint8 *src, int len) {
   if (capacity < (length() + len)) {
      extendRoom(length() + len);
   }
   memcpy(&room[tail], src, len);
   tail += len;
   return *this;
}

ByteArrayBuffer&
ByteArrayBuffer::append(uint8 src) {
   if (capacity < (length() + 1)) {
      extendRoom(length() + 1);
   }
   room[tail] = src;
   tail ++;
   return *this;
}

ByteArrayBuffer&
ByteArrayBuffer::copyin(const uint8 *src, int length) {
   if (capacity < length) {
      int cap = suitableCapacity(capacity, capacityIncrement, length);
      delete room;
      room = allocateRoom(cap);
      capacity = cap;
   }
   tail = length;
   memcpy(room, src, length);
   return *this;
}

#ifdef LOCAL_MAIN
int main() {
   ByteArrayBuffer line;
   int c;

   while ((c = cin.get()) != EOF) {
      line += c;
      if (c == '\n') {
	 cout << line;
	 line = "";
      }
   }
   if (!line.empty()) {
      cout << line;
   }

   return 0;
}
#endif

