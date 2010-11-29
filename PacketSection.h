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

typedef unsigned char uint8;	// 8bit unsigned integer
typedef unsigned short uint16;	// 16bit unsigned integer

class PacketSection {
public:
   virtual int load(uint8 *buffer) = 0;
   virtual void dump(std::ostream *outputstream) const = 0;
protected:
   uint8 *bytes;
};

void hexdump(std::ostream *osp, const uint8 *buf, int len, int indent);

#endif /* PACKETSECTION_H */
