// This may look like C code, but it is really -*- C++ -*-
/*
 * 
 * 
 * @(#)$Id$
 */
#ifndef	LOGGER_H
#define	LOGGER_H
#ifdef IMPLEMENTING_LOGGER
static const char Logger_rcsid[] = "@(#)$Id$";
#endif

class Logger {
public:
   virtual void error(const char* format, ...) = 0;
   virtual void debug(const char* format, ...) = 0;
};

#endif	/* LOGGER_H */
