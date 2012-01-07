// This may look like C code, but it is really -*- C++ -*-
/*
 * 
 * 
 * @(#)$Id$
 */
#ifndef	MJD_H
#define	MJD_H
#ifdef IMPLEMENTING_MJD
static const char MJD_rcsid[] = "@(#)$Id$";
#endif
#include <ctime>
#include "TSTypes.h"

class MJD {
public:
   MJD(uint16 mjd, uint32 jtc);
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

#endif	/* MJD_H */
