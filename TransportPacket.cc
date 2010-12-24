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

static TransportPacket nullobj;
static const int the_field_width[] = {
   8,	// TransportPacket_sync_byte			0
   1,	// TransportPacket_trasnport_error_indicator	1
   1,	// TransportPacket_payload_unit_start_indicator	2
   1,	// TransportPacket_transport_priority		3
   13,	// TransportPacket_PID				4
   2,	// TransportPacket_transport_scrambling_control	5
   1,	// TransportPacket_has_adaptation_field		6
   1,	// TransportPacket_has_payload			7
   4,	// TransportPacket_continuity_counter		8
   0	// TransportPacket_StartOfData			9
};

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

TransportPacket::TransportPacket() {
   initBitDistance();
   adaptationField = NULL;
   payload = NULL;
}

TransportPacket::TransportPacket(std::istream *isp) {
   initBitDistance();
   payload = NULL;

   // Read into buffer
   uint8 buff[SIZEOF_PACKET];
   isp->read((char *)buff, SIZEOF_PACKET);
   ByteArrayBuffer *buffer = new ByteArrayBuffer(buff, SIZEOF_PACKET);
   setBuffer(buffer);

   // Check if it has adaptation field and payload
   int offset = sizeofBufferBefore(TransportPacket_StartOfData);
   if (has_adaptation_field()) {
      ByteArray *dt = buffer->subarray(offset);
      AdaptationField *af = new AdaptationField();
      af->setBuffer(dt);
      adaptationField = af;
      offset += byteAt(offset);
   }
   if (has_payload()) {
      payload = buffer->subarray(offset);
   }
}

void TransportPacket::initBitDistance() {
   static int *the_bit_distance = NULL;
   
   if (this == &nullobj) {
      int len = sizeof(the_field_width) / sizeof(the_field_width[0]);
      the_bit_distance = new int[len + 1];
      initializeBitDistance(the_field_width, len, the_bit_distance);
   }
   assert(the_bit_distance != NULL);
   setBitDistance(the_bit_distance);
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


/*
int TransportPacket::load(TSContext *tsc, std::istream *isp) {
   ByteArrayBuffer *buff = new ByteArrayBuffer();
   setBuffer(buff);

   // Packet Header
   int sizeOfHeader = sizeofBufferBefore(TransportPacket_StartOfData);
   if (readin(isp, buff, sizeOfHeader) != sizeOfHeader) return 0;
   if (PID() == PID_ProgramAssociationTable) {
      assert(!has_adaptation_field());
      assert(has_payload());
   }
   // Adaptation Field
   int sizeAdaptationField = 0;
   if (has_adaptation_field()) {
      adaptationField = new AdaptationField();
      sizeAdaptationField = adaptationField->load(tsc, isp);
      if (sizeAdaptationField == 0) return 0;
   }

   // Payload
   int sizePayload = 0;
   if (has_payload()) {
      sizePayload = 184 - sizeAdaptationField;
      assert(sizePayload > 0);
      ByteArrayBuffer *ba = new ByteArrayBuffer();
      if (readin(isp, ba, sizePayload) != sizePayload) return 0;
      payload = ba;
   }

   setFullLength(sizeOfHeader);
   return sizeOfHeader + sizeAdaptationField + sizePayload;
}

void TransportPacket::process(TSContext *tsc) {

   uint16 pid = PID();
   if (pid == PID_ProgramAssociationTable) {
      programAssociationSection = new ProgramAssociationSection();
      if (programAssociationSection == NULL) return;
      int pointer_field = payload->at(0);
      assert(pointer_field == 0);  // for now
      programAssociationSection->setBuffer(payload->subarray(1 + pointer_field));
      programAssociationSection->process(tsc);
   } else if (tsc->isProgramMapTablePID(pid)) {
      programMapSection = new ProgramMapSection();
      if (programMapSection == NULL) return;
      if (payload_unit_start_indicator()) {
	 int pointer_field = payload->at(0);
	 programMapSection->load(payload->subarray(1 + pointer_field));
	 if (!programMapSection->isCompleted()) {
	    tsc->setIncompletePacketSection(pid, programMapSection);
	    programMapSection = NULL;
	 }
      } else {
	 if (tsc->isIncompletePacketSection(pid)) {
	    PacketSection *fullPacket =
	       tsc->appendPacketSectionData(pid, payload);
	    if (fullPacket != NULL) {
	       programMapSection->load(fullPacket->getBuffer());
	       delete fullPacket;
	       programMapSection->process(tsc);
	    }
	 }
      }
   }
}
*/

