/*
 * TransportPacket - 
 *
 * 
 *
 */
static char rcsid[] = "@(#)$Id$";
#ifdef __GNUG__
#pragma implementation
#endif
#include <assert.h>
#define IMPLEMENTING_TRANSPORTPACKET
#include "TransportPacket.h"
#include "AdaptationField.h"
#include "ProgramMapSection.h"
#include "ByteArrayBuffer.h"

FIELDWIDTH_PREAMBLE(TransportPacket)
FIELDWIDTH_CONTENT(TransportPacket_sync_byte,				8)
FIELDWIDTH_CONTENT(TransportPacket_trasnport_error_indicator,		1)
FIELDWIDTH_CONTENT(TransportPacket_payload_unit_start_indicator,	1)
FIELDWIDTH_CONTENT(TransportPacket_transport_priority,			1)
FIELDWIDTH_CONTENT(TransportPacket_PID,					13)
FIELDWIDTH_CONTENT(TransportPacket_transport_scrambling_control,	2)
FIELDWIDTH_CONTENT(TransportPacket_has_adaptation_field,		1)
FIELDWIDTH_CONTENT(TransportPacket_has_payload,				1)
FIELDWIDTH_CONTENT(TransportPacket_continuity_counter,			4)
FIELDWIDTH_CONTENT(TransportPacket_StartOfData,				0)
FIELDWIDTH_POSTAMBLE(TransportPacket, initvars())

static int readin(std::istream *isp, ByteArrayBuffer *buffer, int length) {
   uint8 buff[length];

   //std::cout << "DBG: readin(isp, packet, ength=" << length << ")";
   isp->read((char *)buff, length);
   buffer->append(buff, length);
   //std::cout << "...read " << std::dec << isp->gcount() << " bytes" << std::endl;
   return isp->gcount();
}
   

/*
 * constructors 
 */

void TransportPacket::initvars() {
   adaptationField = NULL;
   payload = NULL;
}

TransportPacket::TransportPacket(ByteArrayBuffer *buffer) {
   initobj();
   initvars();

   setBuffer(buffer);

   // Check if it has adaptation field and payload
   int offset = sizeofBufferBefore(TransportPacket_StartOfData);
   if (has_adaptation_field()) {
      int len = 1 + byteAt(offset);
      if (buffer->length() >= offset + len) {
	 AdaptationField *af = new AdaptationField();
	 af->setBuffer(buffer->subarray(offset, len));
	 adaptationField = af;
	 offset += len;
      } else {
	 logger->warning("TrasnportPacket: Found an adaptation field, but its size %d in the header is larger than actual size %d. Ignoring the adaptation field.", len, buffer->length() - offset);
      }
   }
   if (has_payload()) {
      payload = buffer->subarray(offset);
   }
}


/*
 * destructors 
 */

TransportPacket::~TransportPacket() {
   if (adaptationField != NULL) delete adaptationField;
   if (payload != NULL) delete payload;
}


/*
 * other methods
 */

int TransportPacket::write(std::ostream *osp) const {
   assert(mydata->length() == SIZEOF_PACKET);
   osp->write((const char *)mydata->part(0, SIZEOF_PACKET), SIZEOF_PACKET);
   return SIZEOF_PACKET;
}

const ByteArray *TransportPacket::getRawdata() const {
   assert(mydata->length() == SIZEOF_PACKET);
   return mydata;
}
