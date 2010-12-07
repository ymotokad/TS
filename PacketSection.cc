/*
 *
 */
static char rcsid[] = "@(#)$Id$";
#define IMPLEMENTING_PACKETSECTION
#include "PacketSection.h"

PacketSection::~PacketSection() {}

void PacketSection::hexdump(std::ostream *osp, const ByteArray *buf, int indent, int len) {
   static const int bpl = 16;
   bool needEndl = false;

   *osp << std::hex;
   osp->unsetf(std::ios::showbase);
   if (len == -1 || len > buf->length()) len = buf->length();
   for (int cnt = 0; cnt < len; cnt++) {
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
      *osp << (unsigned int)buf->at(cnt);
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
