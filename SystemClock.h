#ifndef SYSTEMCLOCK_H
#define SYSTEMCLOCK_H
#ifdef IMPLEMENTING_SYSTEMCLOCK
static const char *rcsid_SystemClock = "@(#)$Id$";
#endif /* IMPLEMENTING_SYSTEMCLOCK */
#include <ctime>
#include "TSTypes.h"

#undef SYSTEMCLOCK_DEBUG
//#define SYSTEMCLOCK_DEBUG

inline int SystemClock_base2sec(uint64 base) {
   return (int)(base / TS_CLOCK_BASE_FREQUENCY);
}

inline int SystemClock_base2msec(uint64 base) {
   return (int)(base % TS_CLOCK_BASE_FREQUENCY) / (TS_CLOCK_BASE_FREQUENCY / 1000);
}

inline char *SystemClock_toString(char *buf, uint64 base) {
   int hour, min, sec, msec;
   sec = SystemClock_base2sec(base);
   msec = SystemClock_base2msec(base);
   min = sec / 60;
   sec -= min * 60;
   hour = min / 60;
   min -= hour * 60;
   sprintf(buf, "%02d:%02d:%02d.%03d", hour, min, sec, msec);
   return buf;
}
   

class SystemClock {
 public:
   SystemClock();
   void setAbsoluteTime(const std::time_t &src);
   std::time_t getAbsoluteTime();
   uint64 getRelativeTime() const; // in time base
   void tick();
   void sync(uint16 pid, uint64 base, uint16 ext);
 protected:
   uint16 reltime_pid;
   int reltime_pid_counter;
   uint64 reltime_base;
   uint16 reltime_ext;
   uint64 start_base;
   uint64 abstime_base;
   std::time_t abstime;
   bool reltime_initialized;
   bool abstime_initialized;
};

inline SystemClock::SystemClock() {
   reltime_initialized = abstime_initialized = false;
   reltime_pid = 0;
   reltime_pid_counter = 0;
   start_base = 0;
}

inline void SystemClock::setAbsoluteTime(const std::time_t &src) {
   abstime = src;
   if (reltime_initialized) {
      abstime_base = reltime_base;
      if (!abstime_initialized) abstime_initialized = true;
   }
}

inline std::time_t SystemClock::getAbsoluteTime() {
   if (!abstime_initialized) return 0;
   assert(reltime_initialized);

   // Adjust absolute time with system clock
   assert(reltime_base >= abstime_base);
   uint64 diff_base = reltime_base - abstime_base;
#ifdef SYSTEMCLOCK_DEBUG
   printf("SystemClock::getAbsoluteTime()...diff=%d\n", diff_base);
#endif
   
   return abstime + SystemClock_base2sec(diff_base); // For now. time_t shouldn't be used for any direct arithmetic operation.
}

inline uint64 SystemClock::getRelativeTime() const {
   if (!reltime_initialized) return 0;

   assert(reltime_base >= start_base);
   uint64 diff_base = reltime_base - start_base;

   return diff_base;
}

inline void SystemClock::sync(uint16 pid, uint64 base, uint16 ext) {
   // Use PCR in 'frequently appearing' stream so we keep more acurate
   // clock value. It is decided as 'frequently appearing' stream if
   // previous sync was done with the same pid with this sync.
   if (reltime_pid == pid) {
      reltime_pid_counter++;
   } else if (reltime_pid_counter == 0) {
      reltime_pid = pid;
      reltime_pid_counter = 0;
      start_base = base;
   } else {
      return;
   }
#ifdef SYSTEMCLOCK_DEBUG
   printf("SystemClock::sync(0x%09x, 0x%04x)...0x%09x, 0x%04x\n", base, ext, reltime_base, reltime_ext);
#endif
   reltime_base = base;
   reltime_ext = ext;
   if (reltime_initialized == false) reltime_initialized = true;
}

#define TS_CLOCK_BASE_MASK	(((uint64)1 << TS_CLOCK_BASE_BITLEN) - 1)

inline void SystemClock::tick() {
   if (reltime_ext == TS_CLOCK_EXTENSION_MAX) {
      reltime_ext = 0;
      reltime_base = (reltime_base + 1) & TS_CLOCK_BASE_MASK;
   } else {
      reltime_ext++;
   }
}


#endif /* SYSTEMCLOCK_H */
