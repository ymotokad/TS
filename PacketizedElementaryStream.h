#ifndef PACKETIZEDELEMENTARYSTREAM_H
#define PACKETIZEDELEMENTARYSTREAM_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_PACKETIZEDELEMENTARYSTREAM
static const char *rcsid_PacketizedElementaryStream = "@(#)$Id$";
#endif /* IMPLEMENTING_PACKETIZEDELEMENTARYSTREAM */
#include <assert.h>
#include <iostream>
#include "TSTypes.h"
#include "ByteArrayBuffer.h"
#include <list>
#include "ElementaryStream.h"

typedef std::list<ByteArrayBuffer *> ByteArrayBufferList;

/*
 * PacketizedElementaryStream
 */
class PacketizedElementaryStream {
 public:
   PacketizedElementaryStream(int startSignatureLength);
   virtual ~PacketizedElementaryStream();
   virtual void put(const ByteArray *packet);
   virtual ElementaryStream *readObject() = 0; // ByteArray returned have to be deleted by caller side
   int length() const;
   void remove(int bytes);
 protected:
   virtual bool isStartSignature(const ByteArray *bp, int off) const = 0;
   int nStartSignature() const;
   ByteArray *read(int off, int n) const;

   ByteArrayBufferList buffer;
   ByteArrayBuffer *carryover;
   int startSignatureLength;
};

inline int PacketizedElementaryStream::nStartSignature() const {
   return startSignatureLength;
}


#endif /* PACKETIZEDELEMENTARYSTREAM_H */
