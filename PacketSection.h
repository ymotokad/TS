/*
 *
 * 
 * 
 *
 */

#ifndef	PACKETSECTION_H
#define	PACKETSECTION_H
#ifdef IMPLEMENTING_PACKETSECTION
static char PacketSection_rcsid[] = "@(#)$Id$";
#endif
#include <iostream>
#include "TSTypes.h"
#include "TSContext.h"
#include "BitStreamSyntax.h"
#include "ByteArrayBuffer.h"

class PacketSection : public BitStreamSyntax {
 public:
   virtual ~PacketSection();
   virtual int load(TSContext *tsc, std::istream *inputstream) = 0;
   virtual int load(const ByteArray *data) = 0;
   virtual void process(TSContext *tsc) = 0;
   virtual void dump(std::ostream *outputstream) const = 0;
   static void hexdump(std::ostream *osp, const ByteArray *buff, int indent, int len);
   void hexdump(std::ostream *osp, int indent, int len) const;
   void hexdump(std::ostream *osp, int indent) const;
};

inline void PacketSection::hexdump(std::ostream *osp, int indent) const {
   hexdump(osp, indent, -1);
}

inline void PacketSection::hexdump(std::ostream *osp, int indent, int len) const {
   hexdump(osp, getBuffer(), indent, len);
}

#endif /* PACKETSECTION_H */
