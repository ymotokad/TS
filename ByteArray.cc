/*
 * ByteArray - 
 *
 *
 */
static char rcsid[] = "@(#)$Id$";
#ifdef __GNUG__
#pragma implementation
#endif
#define IMPLEMENTING_BYTEARRAY
#include <string>
#include "ByteArray.h"
#include "ByteArrayBuffer.h"

/*
 * constructors 
 */

ByteArray::ByteArray(const ByteArray& src) : room(src.room) {
   src.room->incr_refcnt();
   tail = src.length();
   offset = 0;
}

ByteArray::ByteArray(const uint8 *src, int length) {
   room = new Room(length);
   tail = length;
   offset = 0;
}

ByteArray::ByteArray(const ByteArray& src, int off, int length) {
   src.room->incr_refcnt();
   room = src.room;
   if (length == -1) {
      tail = src.tail;
   } else {
      assert(src.length() >= off + length);
      tail = off + length;
   }
   offset = off;
}


/*
 * destructor
 */
ByteArray::~ByteArray() {
   if (room->decr_refcnt() == 0) {
      delete room;
   }
}


/*
 * other methods
 */

uint8
ByteArray::at(int idx) const {
   idx += offset;
   if (idx >= tail) {
      ByteArrayOverflowException e;
      throw e;
   }
   uint8 *p = room->buffer();
   return p[idx];
}

ByteArray *
ByteArray::subarray(int idx, int len) const {
   idx += offset;
   if (idx >= tail) {
      return new ByteArray(NULL, 0);
   }
   return new ByteArray(*this, idx, len);
}

const uint8 *
ByteArray::part(int idx, int len) const {
   idx += offset;
   if ((idx + len) > tail) {
      ByteArrayOverflowException e;
      throw e;
   }
   return room->buffer() + idx;
}
