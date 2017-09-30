#ifndef ADTSSTREAM_H
#define ADTSSTREAM_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_ADTSSTREAM
static const char *rcsid_ADTSStream = "@(#)$Id$";
#endif /* IMPLEMENTING_ADTSSTREAM */
#include <iostream>
#include "TSTypes.h"
#include "ByteArray.h"
#include "PacketizedElementaryStream.h"
#include <list>
#include "ADTSHeaders.h"

/*
 * ADTSStream
 */
class ADTSStream : public PacketizedElementaryStream {
 public:
   ADTSStream();
   virtual ~ADTSStream();
   ElementaryStream *readObject(); // ADTSHeader returned have to be deleted by caller side
   static void dumpHeader(ADTSHeader *h);

 protected:
   bool isStartSignature(const ByteArray *bp, int off) const;
};



#endif /* ADTSSTREAM_H */
