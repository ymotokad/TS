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

#ifndef	LOGGER_H
#define	LOGGER_H
#ifdef IMPLEMENTING_LOGGER
static const char Logger_rcsid[] = "@(#)$Id$";
#endif

class Logger {
public:
   virtual void error(const char* format, ...) = 0;
   virtual void warning(const char* format, ...) = 0;
   virtual void debug(const char* format, ...) = 0;
};

#endif	/* LOGGER_H */
