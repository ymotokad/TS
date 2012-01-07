/*
 *
 *
 * @(#)$Id$
 */
static const char rcsid[] = "@(#)$Id$";
#define IMPLEMENTING_MJD
#include <stdio.h>
#include <stdarg.h>
#include "MJD.h"

MJD::MJD(uint16 m, uint32 j) {
   mjd = m;
   jtc = j;
}

const char *MJD::date() {
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
   

const char *MJD::time() {
   sprintf(jtcbuf, "%02x:%02x:%02x", jtc >> 16, (jtc >> 8) & 0xff, jtc & 0xff);
   return jtcbuf;
}

const char *MJD::datetime() {
   sprintf(dntbuf, "%s %s", date(), time());
   return dntbuf;
}

inline int bcd2bin(uint8 src) {
   return (src / 16) * 10 + (src % 16);
}

std::time_t MJD::convert() const {
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
