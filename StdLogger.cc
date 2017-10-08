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
#define IMPLEMENTING_STDLOGGER
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "StdLogger.h"

StdLogger::StdLogger(int flag) : flag(flag) {
}

void StdLogger::error(const char* format, ...) {
   if (!(flag & LOGGER_ERROR)) return;
   
   fprintf(stderr, "[ERROR] ");
   va_list args;
   va_start(args, format);
   vfprintf(stderr, format, args);
   va_end(args);
   fprintf(stderr, "\n");
   abort();
}

void StdLogger::warning(const char* format, ...) {
   if (!(flag & LOGGER_WARNING)) return;
   
   fprintf(stderr, "[WARNING] ");
   va_list args;
   va_start(args, format);
   vfprintf(stderr, format, args);
   va_end(args);
   fprintf(stderr, "\n");
}

void StdLogger::debug(const char* format, ...) {
   if (!(flag & LOGGER_DEBUG)) return;
   
   fprintf(stderr, "[DEBUG] ");
   va_list args;
   va_start(args, format);
   vfprintf(stderr, format, args);
   va_end(args);
   fprintf(stderr, "\n");
}
