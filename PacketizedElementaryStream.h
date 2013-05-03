#ifndef PACKETIZEDELEMENTARYSTREAM_H
#define PACKETIZEDELEMENTARYSTREAM_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_PACKETIZEDELEMENTARYSTREAM
static const char *rcsid_PacketizedElementaryStream = "@(#)$Id$";
#endif /* IMPLEMENTING_PACKETIZEDELEMENTARYSTREAM */
#include <iostream>
#include "TSTypes.h"
#include "ByteArrayBuffer.h"
#include <list>
#include "MPEGHeaders.h"

typedef std::list<ByteArrayBuffer *> ByteArrayBufferList;

/*
 * PacketizedElementaryStream
 */
class PacketizedElementaryStream {
 public:
   PacketizedElementaryStream();
   ~PacketizedElementaryStream();
   virtual void put(const ByteArray *packet);
   virtual MPEGHeader *readObject() = 0; // ByteArray returned have to be deleted by caller side
   int length() const;
   void remove(int bytes);
 protected:
   ByteArray *read(int off, int n) const;
   ByteArrayBufferList buffer;
};


#endif /* PACKETIZEDELEMENTARYSTREAM_H */
