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
#include <iostream>
#define IMPLEMENTING_TRANSPORTPACKET
#include "TransportPacket.h"
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

/*
 * constructors 
 */

TransportPacket::TransportPacket() : PacketSection() {
   static int *the_bit_distance = NULL;

   adaptationField = NULL;
   programAssociationSection = NULL;
   programMapSection = NULL;
   payload = NULL;
   
   
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
   if (programAssociationSection != NULL) delete programAssociationSection;
   if (programMapSection != NULL) delete programMapSection;
   if (payload != NULL) delete payload;
}


/*
 * other methods
 */

int TransportPacket::load(const ByteArray *data) {
   return 0;
}

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

void TransportPacket::dump(std::ostream *osp) const {
   *osp << "TransportPacket::dump(std::ostream *osp) const ...not yet implemented!!" << std::endl;
}

void TransportPacket::dump(TSContext *tsc, std::ostream *osp) const {
   *osp << "-- Packet";
   const char *pid;
   switch (PID()) {
   case PID_ProgramAssociationTable:
      pid = "Program Association Table";
      break;
   case PID_ConditionalAccessTable:
      pid = "Conditional Access Table";
      break;
   case PID_TransportStreamDescriptionTable:
      pid = "Transport Stream Description Table";
      break;
   case PID_NullPacket:
      pid = "Null Packet";
      break;
   default:
      pid = NULL;
   }

   if (pid) {
      *osp << " PID=" << pid;
   } else {
      *osp << " PID=" << std::hex << std::showbase << (int)PID();
   }
   if (pid == NULL && tsc->isProgramMapTablePID(PID())) {
      *osp << "(PMT)";
   }
   *osp << " cc=" << std::dec << (int)continuity_counter();
   *osp << std::endl;
   //*osp << "  Packet.length: " << length << std::endl;
   if (payload_unit_start_indicator()) {
      *osp << "  payload_unit_start_indicator: 1" << std::endl;
   }

   if (PID() == PID_ProgramAssociationTable) {
      programAssociationSection->dump(osp);
   } else if (tsc->isProgramMapTablePID(PID())) {
      if (programMapSection != NULL && programMapSection->isCompleted()) {
	 programMapSection->dump(osp);
      }
   } else {
      //*osp << "  byte0: " << std::hex << std::showbase << (unsigned int)bytes[0] << std::endl;
      //*osp << "  byte1: " << std::hex << std::showbase << (unsigned int)bytes[1] << std::endl;
      //*osp << "  byte2: " << std::hex << std::showbase << (unsigned int)bytes[2] << std::endl;
      //*osp << "  byte3: " << std::hex << std::showbase << (unsigned int)bytes[3] << std::endl;
      //*osp << std::dec;
      //*osp << "  hasAdaptationField: " << (bool)hasAdaptationField() << std::endl;
      //*osp << "  hasPayload: " << (bool)hasPayload() << std::endl;
      
      //hexdump(osp, bytes, length, 4);

      if (has_adaptation_field()) {
	 adaptationField->dump(osp);
      }

      if (has_payload()) {
	 //*osp << "  -- Payoad";
	 //*osp << std::endl;
	 //hexdump(osp, payload, sizePayload, 4);
      }
   }
}

uint16 TransportPacket::PID() const {
   return bit_field16(TransportPacket_PID);
}

bool TransportPacket::transport_error_indicator() const {
   return bit_field1(TransportPacket_transport_error_indicator);
}

bool TransportPacket::payload_unit_start_indicator() const {
   return bit_field1(TransportPacket_payload_unit_start_indicator);
}

bool TransportPacket::transport_priority() const {
   return bit_field1(TransportPacket_transport_priority);
}

uint8 TransportPacket::transport_scrambling_control() const {
   return bit_field8(TransportPacket_transport_scrambling_control);
}

bool TransportPacket::has_adaptation_field() const {
   return bit_field1(TransportPacket_has_adaptation_field);
}

bool TransportPacket::has_payload() const {
   return bit_field1(TransportPacket_has_payload);
}

uint8 TransportPacket::continuity_counter() const {
   return bit_field8(TransportPacket_continuity_counter);
}

