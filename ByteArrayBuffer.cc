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
   //std::cout << "DBG0: ByteArrayBuffer()_1" << std::endl;
   capacity = initialCapacity;
   if (capacity < 1) capacity = 1;
   tail = 0;
   offset = 0;
   room = new Room(capacity);
}

ByteArrayBuffer::ByteArrayBuffer(const ByteArrayBuffer& src) {
   //std::cout << "DBG0: ByteArrayBuffer()_2" << std::endl;
   capacity = src.capacity;
   capacityIncrement = src.capacityIncrement;
   tail = src.tail;
   offset = 0;
   room = new Room(capacity);
   memcpy(room->buffer(), src.part(0, tail), tail);
}

ByteArrayBuffer::ByteArrayBuffer(const ByteArray& src) {
   //std::cout << "DBG0: ByteArrayBuffer()_3" << std::endl;
   capacity = ((src.length() / BYTEARRAY_INITIALCAPACITY_DEFAULT) + 1) * BYTEARRAY_INITIALCAPACITY_DEFAULT;
   capacityIncrement = BYTEARRAY_CAPACITYINCREMENT_DEFAULT;
   tail = src.length();
   offset = 0;
   room = new Room(capacity);
   memcpy(room->buffer(), src.part(0, tail), tail);
}

ByteArrayBuffer::ByteArrayBuffer(const uint8 *src, int length) {
   //std::cout << "DBG0: ByteArrayBuffer()_4, length=" << length << std::endl;
   capacityIncrement = BYTEARRAY_CAPACITYINCREMENT_DEFAULT;
   capacity = suitableCapacity(BYTEARRAY_INITIALCAPACITY_DEFAULT,
			       capacityIncrement, length);
   tail = length;
   offset = 0;
   room = new Room(capacity);
   memcpy(room->buffer(), src, tail);
}


/*
 * destructor
 */
ByteArrayBuffer::~ByteArrayBuffer() {
   //std::cout << "DBG0: ~ByteArrayBuffer()" << std::endl;
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
   return copyin(target, strlen((const char *)target));
}


/*
 * other methods
 */

void
ByteArrayBuffer::extendRoom(int lengthNeeded) {
   int cap = suitableCapacity(capacity, capacityIncrement, lengthNeeded);
   room->extend(length(), cap);
   capacity = cap;
}

ByteArrayBuffer&
ByteArrayBuffer::append(const ByteArray& src) {
   if (capacity < (length() + src.length())) {
      extendRoom(length() + src.length());
   }
   memcpy(room->buffer() + offset + tail,
	  src.part(0, src.length()),
	  src.length());
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
   memcpy(room->buffer() + offset + tail, src, len);
   tail += len;
   return *this;
}

ByteArrayBuffer&
ByteArrayBuffer::append(uint8 src) {
   if (capacity < (length() + 1)) {
      extendRoom(length() + 1);
   }
   uint8 *p = room->buffer() + offset;
   p[tail] = src;
   tail ++;
   return *this;
}

ByteArrayBuffer&
ByteArrayBuffer::copyin(const uint8 *src, int length) {
   if (capacity < length) {
      int cap = suitableCapacity(capacity, capacityIncrement, length);
      room->extend(capacity, cap);
      capacity = cap;
   }
   tail = length;
   memcpy(room->buffer() + offset, src, length);
   return *this;
}


/*
 * Room Class
 */

Room::Room(int size) {
   //std::cout << "Room::Room(" << std::dec << size << ")" << std::endl;
   data = allocateRoom(size);
   refcnt = 1;
}

Room::~Room() {
   //std::cout << "Room::~Room() ... refcnt=" << std::dec << refcnt << " " << std::endl;
   assert(refcnt == 0);
   delete data;
}

uint8 *Room::buffer() {
   return data;
}

void Room::extend(int current_size, int total_size) {
   //std::cout << "Room::extend()" << std::dec << current_size << ", " << total_size << ")" << std::endl;
   uint8 *r = allocateRoom(total_size);
   memcpy(r, data, current_size);
   delete data;
   data = r;
}

int Room::incr_refcnt() {
   //std::cout << "Room::incr_refcnt() ...refcnt=" << std::dec << refcnt + 1<< "" << std::endl;
   return ++refcnt;
}

int Room::decr_refcnt() {
   //std::cout << "Room::decr_refcnt() ...refcnt=" << std::dec << refcnt - 1<< "" << std::endl;
   return --refcnt;
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

