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

#ifndef B10_TIMEDATESECTION_H
#define B10_TIMEDATESECTION_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_B10_TIMEDATESECTION
static const char *rcsid_B10_TimeDateSection = "@(#)$Id$";
#endif /* IMPLEMENTING_B10_TIMEDATESECTION */
#include <ctime>
#include "ISO13818_Table.h"
#include "B10_MJD.h"

class B10_TimeDateSection : public ISO13818_Section {
 public:
   B10_TimeDateSection();
   B10_TimeDateSection(uint8 continuity_counter);
   void dump(std::ostream *osp) const;
   std::time_t convert() const;

 protected:
   // Order of bit stream data
   static const int pos_JST_time_ymd			= 4;
   static const int pos_JST_time_hms			= 5;
   
 protected:
   void initobj();
};

class B10_TimeOffsetSection : public ISO13818_Section {
 public:
   B10_TimeOffsetSection();
   B10_TimeOffsetSection(uint8 continuity_counter);
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


#endif /* B10_TIMEDATESECTION_H */
