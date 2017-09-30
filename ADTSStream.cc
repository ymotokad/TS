/*
 * ADTSStream - 
 *
 * 
 *
 */
static char rcsid[] = "@(#)$Id$";
#ifdef __GNUG__
#pragma implementation
#endif
#include <assert.h>
#include <stdio.h>
#define IMPLEMENTING_ADTSSTREAM
#include "ADTSStream.h"

/*
 * ADTSStream class
 */

ADTSStream::ADTSStream() : PacketizedElementaryStream(2) {
}

ADTSStream::~ADTSStream() {
}

bool ADTSStream::isStartSignature(const ByteArray *bp, int off) const {
  return bp->at(off + 0) == 0xff && (bp->at(off + 1) & 0xfe) == 0xf8;
}


ElementaryStream *ADTSStream::readObject() {
   if (buffer.empty()) return NULL;
   ByteArray *buf = buffer.front();
   buffer.pop_front();

   assert(isStartSignature(buf, 0));
   ADTSHeader *ret = new ADTSHeader();
   ret->setBuffer(*buf);
   delete buf;

   return ret;
}

void ADTSStream::dumpHeader(ADTSHeader *obj) {
   if (obj != NULL) {
   }
}
