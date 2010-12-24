// This may look like C code, but it is really -*- C++ -*-
/*
 * ByteArray - Byte array class
 *
 * 
 *
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
   ByteArray *subarray(int idx, int len = -1) const;
   const uint8 *part(int idx, int len) const; // ByteArrayOverflowException

   bool isEmpty() const;
   int length() const;

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
