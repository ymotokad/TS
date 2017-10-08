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
#include <iostream>
#include <ctime>
#define IMPLEMENTING_EVENTINFORMATIONTABLE
#include "B10_Descriptors.h"
#include "B10_TimeDateSection.h"

/*
 * B10_TimeDateSection class
 */

FIELDWIDTH_PREAMBLE(B10_TimeDateSection)
FIELDWIDTH_CONTENT(pos_table_id,			8)
FIELDWIDTH_CONTENT(pos_section_syntax_indicator,	1)
FIELDWIDTH_CONTENT(pos_reserved1,			3)
FIELDWIDTH_CONTENT(pos_section_length,			12)
FIELDWIDTH_CONTENT(pos_JST_time_ymd,			16)
FIELDWIDTH_CONTENT(pos_JST_time_hms,			24)
FIELDWIDTH_POSTAMBLE(B10_TimeDateSection, /**/)

B10_TimeDateSection::B10_TimeDateSection(uint8 continuous_counter) : ISO13818_Section(continuous_counter) {
   initobj();
}

std::time_t B10_TimeDateSection::convert() const {
   B10_MJD jst_time(bit_field16(pos_JST_time_ymd), bit_field32(pos_JST_time_hms));
   return jst_time.convert();
}

void B10_TimeDateSection::dump(std::ostream *osp) const {
   assert(TableID_TimeDateSection == table_id());
   printf("  -- Time Date Section ---\n");
   B10_MJD jst_time(bit_field16(pos_JST_time_ymd), bit_field32(pos_JST_time_hms));
   printf("    %s\n", jst_time.datetime());
}

/*
 * B10_TimeOffsetSection class
 */

FIELDWIDTH_PREAMBLE(B10_TimeOffsetSection)
FIELDWIDTH_CONTENT(pos_table_id,			8)
FIELDWIDTH_CONTENT(pos_section_syntax_indicator,	1)
FIELDWIDTH_CONTENT(pos_reserved1,			3)
FIELDWIDTH_CONTENT(pos_section_length,			12)
FIELDWIDTH_CONTENT(pos_JST_time_ymd,			16)
FIELDWIDTH_CONTENT(pos_JST_time_hms,			24)
FIELDWIDTH_CONTENT(pos_reserved,			4)
FIELDWIDTH_CONTENT(pos_descriptors_loop_length,		12)
FIELDWIDTH_CONTENT(pos_start_of_descriptors,		0)
FIELDWIDTH_POSTAMBLE(B10_TimeOffsetSection, /**/)

B10_TimeOffsetSection::B10_TimeOffsetSection(uint8 continuous_counter) : ISO13818_Section(continuous_counter) {
   initobj();
}

std::time_t B10_TimeOffsetSection::convert() const {
   B10_MJD jst_time(bit_field16(pos_JST_time_ymd), bit_field32(pos_JST_time_hms));
   return jst_time.convert();
}

void B10_TimeOffsetSection::dump(std::ostream *osp) const {
   assert(TableID_TimeOffsetSection == table_id());
   printf("  -- Time Offset Section ---\n");
   B10_MJD jst_time(bit_field16(pos_JST_time_ymd), bit_field32(pos_JST_time_hms));
   printf("    %s\n", jst_time.datetime());
}
