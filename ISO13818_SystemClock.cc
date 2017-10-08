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

static char rcsid[] = "@(#)$Id$";
#ifdef __GNUG__
#pragma implementation
#endif
#include <assert.h>
#include <stdio.h>
#include <time.h>
#define IMPLEMENTING_ISO13818_SYSTEMCLOCK
#include "ISO13818_SystemClock.h"


/*
 * ProgramClock class
 */

void ProgramClock::sync(uint64 base, uint16 ext) {
   assert(initialized);
   if (base < (clock_base & TS_CLOCK_BASE_MASK)) {
     // Meaning wrap around occured
     clock_base = clock_base >> TS_CLOCK_BASE_BITLEN;
     clock_base++;
     clock_base = clock_base << TS_CLOCK_BASE_BITLEN;
   }
   clock_base = (clock_base & (~TS_CLOCK_BASE_MASK)) | (base & TS_CLOCK_BASE_MASK);
   clock_ext = ext;
}

bool ProgramClock::isGreaterThan(const ProgramClock &right) const {
   assert(initialized);
   if (clock_base > right.clock_base) return true;
   if (clock_base < right.clock_base) return false;
   if (clock_ext > right.clock_ext) return true;
   return false;
}

bool ProgramClock::isGreaterThanOrEqualTo(const ProgramClock &right) const {
   assert(initialized);
   if (clock_base > right.clock_base) return true;
   if (clock_base < right.clock_base) return false;
   if (clock_ext < right.clock_ext) return false;
   return true;
}

const ProgramClock &ProgramClock::subtract(const ProgramClock &right)  {
   clock_base -= right.clock_base;
   int diff_ext = clock_ext - right.clock_ext;
   if (diff_ext < 0) {
      clock_base--;
      clock_ext = (TS_CLOCK_EXTENSION_MAX + 1) + diff_ext;
   } else {
      clock_ext = diff_ext;
   }
   return *this;
}

char *ProgramClock::toString(char *buf) const {
   int hour, min, sec, msec;
   sec = toSeconds();
   msec = getMilliSeconds();
   min = sec / 60;
   sec -= min * 60;
   hour = min / 60;
   min -= hour * 60;
   sprintf(buf, "%02d:%02d:%02d.%03d", hour, min, sec, msec);
   return buf;
}

char *ProgramClock::toHexString(char *buf) const {
   sprintf(buf, "%09llx.%03x", clock_base, clock_ext);
   return buf;
}



/*
 * ISO13818_SystemClock class
 */

ISO13818_SystemClock::ISO13818_SystemClock() {
   abstime_initialized = false;
   stream_time_pid = 0;
   stream_time_pid_counter = 0;
}

void ISO13818_SystemClock::setAbsoluteTime(const std::time_t &src) {
   abstime = src;
   if (stream_time.isInitialized()) {
      stream_time_last_abstime.set(stream_time);
      if (!abstime_initialized) abstime_initialized = true;
   }
}

std::time_t ISO13818_SystemClock::getAbsoluteTime() const {
   if (!abstime_initialized) return 0;
   assert(stream_time.isInitialized());

   // Adjust absolute time with system clock
   assert(stream_time.isGreaterThanOrEqualTo(stream_time_last_abstime));
   ProgramClock diff(stream_time);
   diff.subtract(stream_time_last_abstime);
   return abstime + diff.toSeconds(); // For now. time_t shouldn't be used for any direct arithmetic operation.
}

ProgramClock ISO13818_SystemClock::getRelativeTime() const {
   if (!stream_time.isInitialized()) {
      return stream_time;
   }
   assert(stream_time.isGreaterThanOrEqualTo(stream_time0));
   ProgramClock result(stream_time);
   result.subtract(stream_time0);
   return result;
}

ProgramClock *ISO13818_SystemClock::getStreamTimeRoundUpBy(int seconds) const {
   if (!abstime_initialized) return 0;
   assert(stream_time.isInitialized());

   // Adjust absolute time with system clock
   assert(stream_time.isGreaterThanOrEqualTo(stream_time_last_abstime));
   ProgramClock diff(stream_time);
   diff.subtract(stream_time_last_abstime);

   // Round it up by 'seconds'
   std::time_t t = abstime;
   t += diff.toSeconds();
   struct tm *now = localtime(&t);
   int aseconds = now->tm_hour * 60 * 60;
   aseconds += now->tm_min * 60;
   aseconds += now->tm_sec;
   int diffsec;
   if (aseconds == 0) {
      diffsec = 0;
   } else {
      diffsec = (((aseconds - 1) / seconds) + 1) * seconds - aseconds;
   }

   // Convert to ProgramClock
   ProgramClock *result = new ProgramClock(stream_time_last_abstime);
   result->append(diffsec);
#if 0
   {
      char buf1[64], buf2[64];
      std::time_t t = getAbsoluteTime();
      struct tm *abs = localtime(&t);
      strftime(buf1, 64, "%Y/%m/%d %H:%M:%S", abs);
      printf("getStreamTimeRoundUpBy(%d): diffsec=%d\n", seconds, diffsec);
      printf("  now      abs=[%s], ProgramClock=[%s]\n", buf1, getStreamTime()->toString(buf2));
      t += diffsec;
      abs = localtime(&t);
      strftime(buf1, 64, "%Y/%m/%d %H:%M:%S", abs);
      printf("  roundup  abs=[%s], ProgramClock=[%s]\n", buf1, result->toString(buf2));
   }
#endif
   return result;
}

void ISO13818_SystemClock::sync(uint16 pid, uint64 base, uint16 ext) {
   // Use PCR in 'frequently appearing' stream so we keep more acurate
   // clock value. It is decided as 'frequently appearing' stream if
   // previous sync was done with the same pid with this sync.
   if (stream_time_pid == pid) {
      stream_time_pid_counter++;
      stream_time.sync(base, ext);
   } else if (stream_time_pid_counter == 0) {
      stream_time_pid = pid;
      stream_time_pid_counter = 0;
      stream_time.set(base, ext);
      stream_time0.set(stream_time);
   } else {
      return;
   }
}
