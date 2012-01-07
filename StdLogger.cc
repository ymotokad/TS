/*
 *
 *
 * @(#)$Id$
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
