#ifndef SYSTEMCLOCK_H
#define SYSTEMCLOCK_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_SYSTEMCLOCK
static const char *rcsid_SystemClock = "@(#)$Id$";
#endif /* IMPLEMENTING_SYSTEMCLOCK */
#include <ctime>
#include "TSTypes.h"

#undef SYSTEMCLOCK_DEBUG
//#define SYSTEMCLOCK_DEBUG


#define TS_CLOCK_BASE_MASK	(((uint64)1 << TS_CLOCK_BASE_BITLEN) - 1)

class ProgramClock;

class ProgramClock {
 public:
   ProgramClock();
   ProgramClock(uint64 base, uint16 ext);
   ProgramClock(const ProgramClock &src);
   bool isInitialized() const;
   void set(uint64 base, uint16 ext);
   void set(const ProgramClock &src);
   void sync(uint64 base, uint16 ext);
   void append(int seconds);
   void appendMilliSeconds(int mseconds);
   const ProgramClock &subtract(const ProgramClock &right);
   bool isGreaterThan(const ProgramClock &right) const;
   bool isGreaterThanOrEqualTo(const ProgramClock &right) const;
   void tick();
   int toSeconds() const;
   int getMilliSeconds() const;
   char *toString(char *buf) const;
   char *toHexString(char *buf) const;
 protected:
   int64 clock_base;
   uint16 clock_ext;
   bool initialized;
};

class SystemClock {
 public:
   SystemClock();
   void setAbsoluteTime(const std::time_t &src);
   std::time_t getAbsoluteTime() const;
   ProgramClock getRelativeTime() const;
   const ProgramClock *getStreamTime() const;
   const ProgramClock *getStreamTimeRoundUpBy(int seconds) const;
   void tick();
   void sync(uint16 pid, uint64 base, uint16 ext);
 protected:
   uint16 stream_time_pid;
   int stream_time_pid_counter;
   ProgramClock stream_time;
   ProgramClock stream_time_last_abstime;
   ProgramClock stream_time0;
   std::time_t abstime;
   bool abstime_initialized;
};

inline ProgramClock::ProgramClock() {
   initialized = false;
   clock_base = 0;
   clock_ext = 0;
}

inline ProgramClock::ProgramClock(uint64 base, uint16 ext) {
   initialized = true;
   clock_base = base;
   clock_ext = ext;
}

inline ProgramClock::ProgramClock(const ProgramClock &src) {
   initialized = src.initialized;
   clock_base = src.clock_base;
   clock_ext = src.clock_ext;
}

inline bool ProgramClock::isInitialized() const {
   return initialized;
}

inline void ProgramClock::set(uint64 base, uint16 ext) {
   initialized = true;
   clock_base = base;
   clock_ext = ext;
}

inline void ProgramClock::set(const ProgramClock &src) {
   initialized = src.initialized;
   clock_base = src.clock_base;
   clock_ext = src.clock_ext;
}

inline void ProgramClock::sync(uint64 base, uint16 ext) {
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

inline void ProgramClock::append(int seconds) {
   assert(initialized);
   clock_base += ((int64)seconds * TS_CLOCK_BASE_FREQUENCY);
}

inline void ProgramClock::appendMilliSeconds(int mseconds) {
   assert(initialized);
   clock_base += ((int64)mseconds * (TS_CLOCK_BASE_FREQUENCY / 1000));
}

inline bool ProgramClock::isGreaterThan(const ProgramClock &right) const {
   assert(initialized);
   if (clock_base > right.clock_base) return true;
   if (clock_base < right.clock_base) return false;
   if (clock_ext > right.clock_ext) return true;
   return false;
}

inline bool ProgramClock::isGreaterThanOrEqualTo(const ProgramClock &right) const {
   assert(initialized);
   if (clock_base > right.clock_base) return true;
   if (clock_base < right.clock_base) return false;
   if (clock_ext < right.clock_ext) return false;
   return true;
}

inline void ProgramClock::tick() {
   if (clock_ext == TS_CLOCK_EXTENSION_MAX) {
      clock_ext = 0;
      clock_base++;
   } else {
      clock_ext++;
   }
}

inline int ProgramClock::toSeconds() const {
   return (int)(clock_base / TS_CLOCK_BASE_FREQUENCY);
}

inline int ProgramClock::getMilliSeconds() const {
   return (int)(clock_base % TS_CLOCK_BASE_FREQUENCY) / (TS_CLOCK_BASE_FREQUENCY / 1000);
}


inline void SystemClock::tick() {
   stream_time.tick();
}

inline const ProgramClock *SystemClock::getStreamTime() const {
   return &stream_time;
}


#endif /* SYSTEMCLOCK_H */
