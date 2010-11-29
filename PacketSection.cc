/*
 *
 */
static char rcsid[] = "@(#)$Id$";
#define IMPLEMENTING_PACKETSECTION
#include "PacketSection.h"

void hexdump(std::ostream *osp, const uint8 *buf, int len, int indent) {
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
