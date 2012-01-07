/*
 *  - 
 *
 * 
 *
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
#include "Descriptors.h"
#include "TimeDateSection.h"

/*
 * TimeDateSection class
 */

FIELDWIDTH_PREAMBLE(TimeDateSection)
FIELDWIDTH_CONTENT(pos_table_id,			8)
FIELDWIDTH_CONTENT(pos_section_syntax_indicator,	1)
FIELDWIDTH_CONTENT(pos_reserved1,			3)
FIELDWIDTH_CONTENT(pos_section_length,			12)
FIELDWIDTH_CONTENT(pos_JST_time_ymd,			16)
FIELDWIDTH_CONTENT(pos_JST_time_hms,			24)
FIELDWIDTH_POSTAMBLE(TimeDateSection, /**/)

TimeDateSection::TimeDateSection(uint8 continuous_counter) : Section(continuous_counter) {
   initobj();
}

std::time_t TimeDateSection::convert() const {
   MJD jst_time(bit_field16(pos_JST_time_ymd), bit_field32(pos_JST_time_hms));
   return jst_time.convert();
}

void TimeDateSection::dump(std::ostream *osp) const {
   assert(TableID_TimeDateSection == table_id());
   printf("  -- Time Date Section ---\n");
   MJD jst_time(bit_field16(pos_JST_time_ymd), bit_field32(pos_JST_time_hms));
   printf("    %s\n", jst_time.datetime());
}

/*
 * TimeOffsetSection class
 */

FIELDWIDTH_PREAMBLE(TimeOffsetSection)
FIELDWIDTH_CONTENT(pos_table_id,			8)
FIELDWIDTH_CONTENT(pos_section_syntax_indicator,	1)
FIELDWIDTH_CONTENT(pos_reserved1,			3)
FIELDWIDTH_CONTENT(pos_section_length,			12)
FIELDWIDTH_CONTENT(pos_JST_time_ymd,			16)
FIELDWIDTH_CONTENT(pos_JST_time_hms,			24)
FIELDWIDTH_CONTENT(pos_reserved,			4)
FIELDWIDTH_CONTENT(pos_descriptors_loop_length,		12)
FIELDWIDTH_CONTENT(pos_start_of_descriptors,		0)
FIELDWIDTH_POSTAMBLE(TimeOffsetSection, /**/)

TimeOffsetSection::TimeOffsetSection(uint8 continuous_counter) : Section(continuous_counter) {
   initobj();
}

std::time_t TimeOffsetSection::convert() const {
   MJD jst_time(bit_field16(pos_JST_time_ymd), bit_field32(pos_JST_time_hms));
   return jst_time.convert();
}

void TimeOffsetSection::dump(std::ostream *osp) const {
   assert(TableID_TimeOffsetSection == table_id());
   printf("  -- Time Offset Section ---\n");
   MJD jst_time(bit_field16(pos_JST_time_ymd), bit_field32(pos_JST_time_hms));
   printf("    %s\n", jst_time.datetime());
}
