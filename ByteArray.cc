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
   tail = src.tail;
   offset = src.offset;
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
      tail = src.offset + off + length;
   }
   offset = src.offset + off;
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
   if (idx + offset >= tail) {
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

void ByteArray::hexdump(int indent, std::ostream *osp, int offset, int len) const {
   static const int bpl = 16;
   bool needEndl = false;

   *osp << std::hex;
   osp->unsetf(std::ios::showbase);
   if (len == -1 || offset + len > this->length()) len = this->length() - offset;
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
      *osp << (unsigned int)this->at(offset + cnt);
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
