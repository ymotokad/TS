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

#ifndef	B10_MJD_H
#define	B10_MJD_H
#ifdef IMPLEMENTING_B10_MJD
static const char B10_MJD_rcsid[] = "@(#)$Id$";
#endif
#include <ctime>
#include "TSTypes.h"

class B10_MJD {
public:
   B10_MJD(uint16 mjd, uint32 jtc);
   const char *date();
   const char *time();
   const char *datetime();
   std::time_t convert() const;
private:
   uint16 mjd;
   uint32 jtc;
   char mjdbuf[11]; // "yyyy/mm/dd"
   char jtcbuf[9]; // "hh:mm:ss"
   char dntbuf[20]; // "yyyy/mm/dd hh:mm:ss"
};

#endif	/* B10_MJD_H */
