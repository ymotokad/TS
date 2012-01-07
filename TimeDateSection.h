#ifndef TIMEDATESECTION_H
#define TIMEDATESECTION_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_TIMEDATESECTION
static const char *rcsid_TimeDateSection = "@(#)$Id$";
#endif /* IMPLEMENTING_TIMEDATESECTION */
#include <ctime>
#include "Table.h"
#include "MJD.h"

class TimeDateSection : public Section {
 public:
   TimeDateSection();
   TimeDateSection(uint8 continuity_counter);
   void dump(std::ostream *osp) const;
   std::time_t convert() const;

 protected:
   // Order of bit stream data
   static const int pos_JST_time_ymd			= 4;
   static const int pos_JST_time_hms			= 5;
   
 protected:
   void initobj();
};

class TimeOffsetSection : public Section {
 public:
   TimeOffsetSection();
   TimeOffsetSection(uint8 continuity_counter);
   void dump(std::ostream *osp) const;
   std::time_t convert() const;

 protected:
   // Order of bit stream data
   static const int pos_JST_time_ymd			= 4;
   static const int pos_JST_time_hms			= 5;
   static const int pos_reserved			= 6;
   static const int pos_descriptors_loop_length		= 7;
   static const int pos_start_of_descriptors		= 8;
   
 protected:
   void initobj();
};


#endif /* TIMEDATESECTION_H */
