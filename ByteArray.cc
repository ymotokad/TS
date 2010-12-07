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

/*
 * constructors 
 */

ByteArray::ByteArray(const ByteArray& src) : room((uint8 *)src.room) {
   tail = src.length();
}

ByteArray::ByteArray(const uint8 *src, int length) : room((uint8 *)src) {
   tail = length;
}


/*
 * destructor
 */
ByteArray::~ByteArray() {
}


/*
 * other methods
 */

uint8
ByteArray::at(int idx) const {
   if (idx >= tail) {
      ByteArrayOverflowException e;
      throw e;
   }
   return room[idx];
}

ByteArray *
ByteArray::subarray(int idx, int len) const {
   if (idx >= tail) {
      return new ByteArray(NULL, 0);
   }
   int n = tail - idx;
   if (len == -1) {
      len = n;
   } else {
      if (len > n) len = n;
   }
   return new ByteArray(room + idx, len);
}

const uint8 *
ByteArray::part(int idx, int len) const {
   if ((idx + len) > tail) {
      ByteArrayOverflowException e;
      throw e;
   }
   return room + idx;
}
