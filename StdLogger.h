// This may look like C code, but it is really -*- C++ -*-
/*
 * 
 * 
 * @(#)$Id$
 */
#ifndef	STDLOGGER_H
#define	STDLOGGER_H
#ifdef IMPLEMENTING_STDLOGGER
static const char StdLogger_rcsid[] = "@(#)$Id$";
#endif
#include <String.h>
#include "Logger.h"

#define LOGGER_ERROR	(1<<0)
#define LOGGER_DEBUG	(1<<1)

class StdLogger : public Logger {
public:
   StdLogger( int flag = LOGGER_ERROR );
   void error( const char* format, ... );
   void debug( const char* format, ... );
private:
   int flag;
};

#endif	/* STDLOGGER_H */
