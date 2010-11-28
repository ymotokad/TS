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

static void hexdump(std::ostream *osp, uint8 *buf, int len, int indent) {
   int bpl = 16;
   int cnt;
   bool needEndl = false;
   *osp << std::hex;
   osp->unsetf(std::ios::showbase);
   for (cnt = 0; cnt < len; cnt++) {
      if ((cnt % bpl) == 0) {
	 for (int i = 0; i < indent; i++) {
	    *osp << " ";
	 }
	 osp->width(2);
	 osp->fill('0');
	 *osp << cnt << ":";
      }
      if ((cnt % (bpl / 2)) == 0 && (cnt % bpl) != 0 ) {
	 *osp << " - ";
      } else {
	 *osp << " ";
      }
      osp->width(2);
      osp->fill('0');
      *osp << (unsigned int)buf[cnt];
      if ((cnt % bpl) == bpl - 1) {
	 *osp << std::endl;
	 needEndl = false;
      } else {
	 needEndl = true;
      }
   }
   if (needEndl) {
      *osp << std::endl;
   }
}

/*
 * constructors 
 */

AdaptationField::AdaptationField() {
   buffer = NULL;
}


/*
 * destructors 
 */

AdaptationField::~AdaptationField() {
   if (buffer != NULL) {
      delete buffer;
   }
}

/*
 * other methods
 */
int AdaptationField::load(std::istream *isp) {
   // Header
   isp->read((char *)&byte0, sizeof byte0);
   if (isp->gcount() != sizeof byte0) return 0;
   if (field_length() == 0) {
      return 1;
   }

   isp->read((char *)&byte1, sizeof byte1);
   if (isp->gcount() != sizeof byte1) return 0;
   
   // 
   int len = field_length() - 1;
   buffer = new uint8[len];
   isp->read((char *)buffer, len);
   assert(isp->gcount() == len);
   return 2 + len;
}

void AdaptationField::dump(std::ostream *osp) const {
   *osp << "  -- Adaptation Field";
   *osp << " len=" << std::dec << (int)field_length();
   *osp << std::endl;
   *osp << "    byte0: " << std::hex << std::showbase << (unsigned int)byte0 << std::endl;
   *osp << "    byte1: " << std::hex << std::showbase << (unsigned int)byte1 << std::endl;
   hexdump(osp, buffer, field_length() - 2, 4);
}


/*
 * constructors 
 */

TransportPacket::TransportPacket() {
   payload = NULL;
   adaptationField = NULL;
}


/*
 * destructors 
 */

TransportPacket::~TransportPacket() {
   if (adaptationField != NULL) {
      delete adaptationField;
   }
   if (payload != NULL) {
      delete payload;
   }
}


/*
 * other methods
 */
int TransportPacket::load(std::istream *isp) {
   // Packet Header
   isp->read((char *)&byte0, sizeof byte0);
   assert(byte0 == SYNC_BYTE_VALUE);
   isp->read((char *)&byte1, sizeof byte1);
   isp->read((char *)&byte2, sizeof byte2);
   isp->read((char *)&byte3, sizeof byte3);
   if (isp->gcount() != sizeof byte3) return 0;
   // Adaptation Field
   int sizeAdaptationField;
   if (hasAdaptationField()) {
      adaptationField = new AdaptationField();
      if (adaptationField == NULL) return -1;
      sizeAdaptationField = adaptationField->load(isp);
      if (sizeAdaptationField < 0) return -1;
   } else {
      sizeAdaptationField = 0;
   }
   // Payload
   sizePayload = 0;
   if (hasPayload()) {
      sizePayload = 184 - sizeAdaptationField;
      assert(sizePayload > 0);
      payload = new uint8[sizePayload];
      if (payload == NULL) return -1;
      isp->read((char *)payload, sizePayload);
      assert(isp->gcount() == sizePayload);
   }
   return 4 + sizeAdaptationField + sizePayload;
}

void TransportPacket::dump(std::ostream *osp) const {
   *osp << "-- Packet";
   char *pid;
   switch (PID()) {
   case 0x0000:
      pid = (char *)"Program Association Table";
      break;
   case 0x0001:
      pid = (char *)"Conditional Access Table";
      break;
   case 0x0002:
      pid = (char *)"Transport Stream Description Table";
      break;
   case 0x1fff:
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
   //*osp << "  byte0: " << std::hex << std::showbase << (unsigned int)byte0 << std::endl;
   //*osp << "  byte1: " << std::hex << std::showbase << (unsigned int)byte1 << std::endl;
   //*osp << "  byte2: " << std::hex << std::showbase << (unsigned int)byte2 << std::endl;
   //*osp << "  byte3: " << std::hex << std::showbase << (unsigned int)byte3 << std::endl;
   //*osp << std::dec;
   //*osp << "  hasAdaptationFiled: " << (bool)hasAdaptationField() << std::endl;
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
