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

#ifndef	STDLOGGER_H
#define	STDLOGGER_H
#ifdef IMPLEMENTING_STDLOGGER
static const char StdLogger_rcsid[] = "@(#)$Id$";
#endif
#include "Logger.h"

#define LOGGER_ERROR	(1<<0)
#define LOGGER_WARNING	(1<<1)
#define LOGGER_DEBUG	(1<<2)

class StdLogger : public Logger {
public:
   StdLogger( int flag = LOGGER_ERROR );
   void error( const char* format, ... );
   void warning( const char* format, ... );
   void debug( const char* format, ... );
private:
   int flag;
};

#endif	/* STDLOGGER_H */
