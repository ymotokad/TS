/*
 * PacketizedElementaryStream - 
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
#define IMPLEMENTING_PACKETIZEDELEMENTARYSTREAM
#include "ByteArrayBuffer.h"
#include "PacketizedElementaryStream.h"

/*
 * PacketizedElementaryStream class
 */

PacketizedElementaryStream::PacketizedElementaryStream() {
}

PacketizedElementaryStream::~PacketizedElementaryStream() {
   ByteArrayBufferList::iterator it;
   for (it = buffer.begin(); it != buffer.end(); it++) {
      ByteArray *p = *it;
      delete p;
   }
}

void PacketizedElementaryStream::put(const ByteArray *packet) {
#if 1
   buffer.push_back(new ByteArrayBuffer(*packet));
#else
   ByteArray *p = new ByteArray(*packet);
   buffer.push_back(p);

   printf("DBG: PES::put(), ");
   packet->hexdump(0, &std::cout, 0, 8);

   printf("DBG:            --");
   p->hexdump(0, &std::cout, 0, 8);

   p = buffer.front();
   printf("DBG:            --");
   p->hexdump(0, &std::cout, 0, 8);
#endif
}

void PacketizedElementaryStream::remove(int bytes) {
   int bytesToBeRemoved = bytes;
   ByteArray *p = buffer.front();
   buffer.pop_front();
   while (bytesToBeRemoved > 0) {
      if (p->length() > bytesToBeRemoved) {
	 ByteArray *sub = p->subarray(bytesToBeRemoved);
	 delete p;
	 buffer.push_front(new ByteArrayBuffer(*sub));
	 delete sub;
	 bytesToBeRemoved = 0;
      } else {
	 bytesToBeRemoved -= p->length();
	 delete p;
	 p = buffer.front();
	 buffer.pop_front();
      }
   }
}

int PacketizedElementaryStream::length() const {
   int len = 0;
   for (ByteArrayBufferList::const_iterator it = buffer.begin(); it != buffer.end(); it++) {
      ByteArray *p = *it;
      len += p->length();
   }

   return len;
}

ByteArray *PacketizedElementaryStream::read(int off, int bytes) const {
   if (off + bytes > length()) return NULL;

   int bytesToBeRead = bytes;
   ByteArrayBuffer *buf = new ByteArrayBuffer(1024, 1024);
   if (buf == NULL) return NULL;
   for (ByteArrayBufferList::const_iterator it = buffer.begin(); it != buffer.end(); it++) {
      if (bytesToBeRead <= 0) break;
      ByteArray *p = *it;
      if (p->length() <= off) {
	 off -= p->length();
	 continue;
      }
      if (p->length() > off + bytesToBeRead) {
	 buf->append(p->part(off, bytesToBeRead), bytesToBeRead);
	 bytesToBeRead = 0;
	 off = 0;
	 break;
      } else {
	 buf->append(p->part(off, bytesToBeRead), bytesToBeRead);
	 bytesToBeRead -= p->length() - off;
	 off = 0;
      }
   }

   assert(bytesToBeRead == 0);
   //printf("DBG: PES::read(%d), ", bytes);
   //buf->hexdump(0, &std::cout, 0, -1);
   return buf;
}
