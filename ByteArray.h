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

#ifndef	BYTEARRAY_H
#define	BYTEARRAY_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_BYTEARRAY
static char ByteArray_rcsid[] = "@(#)$Id$";
#endif

#include <iostream>
#include <assert.h>
#include "TSTypes.h"

class Room;
class ByteArray;

class ByteArray {
public:
   ByteArray(const ByteArray& src);
   ByteArray(const uint8 *src, int length);
   ByteArray(const ByteArray& src, int offset, int length);
   virtual ~ByteArray();

   uint8 at(int idx) const; // ByteArrayOverflowException
   ByteArray *subarray(int idx, int len = -1) const; // Caller is responsible to delete returned object
   const uint8 *part(int idx = 0, int len = -1) const; // ByteArrayOverflowException

   bool isEmpty() const;
   int length() const;
   void hexdump(int indent, std::ostream *osp, int offset = 0, int len = -1) const;

protected:
   ByteArray();
   Room *room;
   int tail;
   int offset;
private:
   ByteArray& operator = (const ByteArray& src); // detect implicit copy operator usage
};

inline ByteArray::ByteArray() {};
inline bool ByteArray::isEmpty() const { return tail == offset; }
inline int ByteArray::length() const { return tail - offset; }

class ByteArrayOverflowException {
public:
   ByteArrayOverflowException();
};

inline ByteArrayOverflowException::ByteArrayOverflowException() {}

#endif /* BYTEARRAY_H */
