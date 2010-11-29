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


/*
 * constructors 
 */

TransportPacket::TransportPacket() {
   adaptationField = NULL;
}


/*
 * destructors 
 */

TransportPacket::~TransportPacket() {
   if (adaptationField != NULL) {
      delete adaptationField;
   }
}


/*
 * other methods
 */
int TransportPacket::load(std::istream *isp) {
   uint8 *ptr = bytes;
   // Packet Header
   isp->read((char *)ptr, sizeOfHeader);
   if (isp->gcount() != sizeOfHeader) return 0;
   assert(bytes[0] == SYNC_BYTE_VALUE);
   ptr += sizeOfHeader;
   
   if (PID() == PID_ProgramAssociationTable) {
      assert(!hasAdaptationField());
      assert(hasPayload());
   }
   // Adaptation Field
   int sizeAdaptationField;
   if (hasAdaptationField()) {
      adaptationField = new AdaptationField();
      if (adaptationField == NULL) return -1;
      // length
      isp->read((char *)ptr, 1);
      if (isp->gcount() != 1) return 0;
      sizeAdaptationField = adaptationField->load(ptr);
      ptr++;
      isp->read((char *)ptr, sizeAdaptationField - 1);
      if (isp->gcount() != sizeAdaptationField - 1) return 0;
      ptr += sizeAdaptationField - 1;
   } else {
      sizeAdaptationField = 0;
   }
   // Payload
   sizePayload = 0;
   if (hasPayload()) {
      sizePayload = 184 - sizeAdaptationField;
      assert(sizePayload > 0);
      isp->read((char *)ptr, sizePayload);
      if (isp->gcount() != sizePayload) return 0;
      payload = ptr;
      ptr += sizePayload;
   }
   assert(sizeOfHeader + sizeAdaptationField + sizePayload <= sizeof bytes);
   return sizeOfHeader + sizeAdaptationField + sizePayload;
}

void TransportPacket::dump(std::ostream *osp) const {
   *osp << "-- Packet";
   char *pid;
   switch (PID()) {
   case PID_ProgramAssociationTable:
      pid = (char *)"Program Association Table";
      break;
   case PID_ConditionalAccessTable:
      pid = (char *)"Conditional Access Table";
      break;
   case PID_TransportStreamDescriptionTable:
      pid = (char *)"Transport Stream Description Table";
      break;
   case PID_NullPacket:
      pid = (char *)"Null Packet";
      break;
   default:
      pid = NULL;
   }
   if (pid) {
      *osp << " id=" << pid;
   } else {
      *osp << " id=" << std::hex << std::showbase << (int)PID();
   }
   *osp << " cc=" << std::dec << (int)continuity_counter();
   *osp << std::endl;

   if (PID() == PID_ProgramAssociationTable) {
      hexdump(osp, bytes, sizeOfHeader+ sizePayload, 2);
   } else {
      //*osp << "  byte0: " << std::hex << std::showbase << (unsigned int)bytes[0] << std::endl;
      //*osp << "  byte1: " << std::hex << std::showbase << (unsigned int)bytes[1] << std::endl;
      //*osp << "  byte2: " << std::hex << std::showbase << (unsigned int)bytes[2] << std::endl;
      //*osp << "  byte3: " << std::hex << std::showbase << (unsigned int)bytes[3] << std::endl;
      //*osp << std::dec;
      //*osp << "  hasAdaptationField: " << (bool)hasAdaptationField() << std::endl;
      //*osp << "  hasPayload: " << (bool)hasPayload() << std::endl;

      if (hasAdaptationField()) {
	 //adaptationField->dump(osp);
      }

      if (hasPayload()) {
	 //*osp << "  -- Payoad";
	 //*osp << std::endl;
	 //hexdump(osp, payload, sizePayload, 4);
      }
   }
}

void TransportPacket::dump_hex(std::ostream *osp) const {
   *osp << "-- Packet" << std::endl;
   hexdump(osp, bytes, sizeOfHeader+ sizePayload, 2);
}
