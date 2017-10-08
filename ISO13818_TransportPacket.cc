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

static char rcsid[] = "@(#)$Id$";
#ifdef __GNUG__
#pragma implementation
#endif
#include <assert.h>
#define IMPLEMENTING_ISO13818_TRANSPORTPACKET
#include "ISO13818_TransportPacket.h"
#include "ISO13818_AdaptationField.h"
#include "ByteArrayBuffer.h"

FIELDWIDTH_PREAMBLE(ISO13818_TransportPacket)
FIELDWIDTH_CONTENT(ISO13818_TransportPacket_sync_byte,				8)
FIELDWIDTH_CONTENT(ISO13818_TransportPacket_trasnport_error_indicator,		1)
FIELDWIDTH_CONTENT(ISO13818_TransportPacket_payload_unit_start_indicator,	1)
FIELDWIDTH_CONTENT(ISO13818_TransportPacket_transport_priority,			1)
FIELDWIDTH_CONTENT(ISO13818_TransportPacket_PID,				13)
FIELDWIDTH_CONTENT(ISO13818_TransportPacket_transport_scrambling_control,	2)
FIELDWIDTH_CONTENT(ISO13818_TransportPacket_has_adaptation_field,		1)
FIELDWIDTH_CONTENT(ISO13818_TransportPacket_has_payload,			1)
FIELDWIDTH_CONTENT(ISO13818_TransportPacket_continuity_counter,			4)
FIELDWIDTH_CONTENT(ISO13818_TransportPacket_StartOfData,			0)
FIELDWIDTH_POSTAMBLE(ISO13818_TransportPacket, initvars())

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

void ISO13818_TransportPacket::initvars() {
   adaptationField = NULL;
   payload = NULL;
}

ISO13818_TransportPacket::ISO13818_TransportPacket(ByteArray *buffer) {
   initobj();
   initvars();

   setBuffer(buffer);

   // Check if it has adaptation field and payload
   int offset = sizeofBufferBefore(ISO13818_TransportPacket_StartOfData);
   if (has_adaptation_field()) {
      int len = 1 + byteAt(offset);
      if (buffer->length() >= offset + len) {
	 ISO13818_AdaptationField *af = new ISO13818_AdaptationField();
	 af->setBuffer(buffer->subarray(offset, len));
	 af->load();
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

ISO13818_TransportPacket::~ISO13818_TransportPacket() {
   if (adaptationField != NULL) delete adaptationField;
   if (payload != NULL) delete payload;
}


/*
 * other methods
 */

int ISO13818_TransportPacket::write(std::ostream *osp) const {
   assert(mydata->length() == SIZEOF_PACKET);
   osp->write((const char *)mydata->part(0, SIZEOF_PACKET), SIZEOF_PACKET);
   return SIZEOF_PACKET;
}

const ByteArray *ISO13818_TransportPacket::getRawdata() const {
   assert(mydata->length() == SIZEOF_PACKET);
   return mydata;
}
