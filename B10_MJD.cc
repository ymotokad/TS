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

static const char rcsid[] = "@(#)$Id$";
#define IMPLEMENTING_B10_MJD
#include <stdio.h>
#include <stdarg.h>
#include "B10_MJD.h"

B10_MJD::B10_MJD(uint16 m, uint32 j) {
   mjd = m;
   jtc = j;
}

const char *B10_MJD::date() {
   // Defined in Appendix C of ARIB-STD-B10 Part II
   int y, m, d;
   y = (int)(((double)mjd - 15078.2) / 365.25);
   m = (int)(((double)mjd - 14956.1 - (int)(y * 365.25)) / 30.6001);
   d = mjd - 14956 - (int)(y * 365.25) - (int)(m * 30.6001);
   int k = (m == 14 || m == 15)? 1 : 0;
   y = y + k;
   m = m - 1 - k * 12;

   sprintf(mjdbuf, "%04d/%02d/%02d", y + 1900, m, d);
   return mjdbuf;
}
   

const char *B10_MJD::time() {
   sprintf(jtcbuf, "%02x:%02x:%02x", jtc >> 16, (jtc >> 8) & 0xff, jtc & 0xff);
   return jtcbuf;
}

const char *B10_MJD::datetime() {
   sprintf(dntbuf, "%s %s", date(), time());
   return dntbuf;
}

inline int bcd2bin(uint8 src) {
   return (src / 16) * 10 + (src % 16);
}

std::time_t B10_MJD::convert() const {
   std::tm t;

   // Defined in Appendix C of ARIB-STD-B10 Part II
   int y, m, d;
   y = (int)(((double)mjd - 15078.2) / 365.25);
   m = (int)(((double)mjd - 14956.1 - (int)(y * 365.25)) / 30.6001);
   d = mjd - 14956 - (int)(y * 365.25) - (int)(m * 30.6001);
   int k = (m == 14 || m == 15)? 1 : 0;
   y = y + k;
   m = m - 1 - k * 12;

   t.tm_year = y;
   t.tm_mon = m - 1;
   t.tm_mday = d;
   t.tm_hour = bcd2bin(jtc >> 16);
   t.tm_min = bcd2bin((jtc >> 8) & 0xff);
   t.tm_sec = bcd2bin(jtc & 0xff);
   t.tm_isdst = -1;

   return std::mktime(&t);
}
