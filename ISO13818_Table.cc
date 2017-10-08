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
#include <iostream>
#define IMPLEMENTING_ISO13818_TABLE
#include "ISO13818_Table.h"

inline uint8 incr_cc(uint8 cc) {
   return (cc + 1) & 0xf;
}


/*
 * ISO13818_Section class
 */

FIELDWIDTH_PREAMBLE(ISO13818_Section)
FIELDWIDTH_CONTENT(pos_table_id,			8)
FIELDWIDTH_CONTENT(pos_section_syntax_indicator,	2)
FIELDWIDTH_CONTENT(pos_reserved1,			2)
FIELDWIDTH_CONTENT(pos_section_length,			12)
FIELDWIDTH_CONTENT(pos_start_of_section,		0)
FIELDWIDTH_POSTAMBLE(ISO13818_Section, /**/)

ISO13818_Section::ISO13818_Section(uint8 continuity_counter) : last_cc(continuity_counter) {
   initobj();
}

ISO13818_Section::~ISO13818_Section() {
}

bool ISO13818_Section::isComplete() const {
   assert(bufferAllocated());
   if (bufferLength() < sizeofBufferBefore(pos_start_of_section)) return false;
   int expected_length = sizeofBufferBefore(pos_start_of_section) + section_length();
   return expected_length <= bufferLength();
}

uint8 ISO13818_Section::table_id() const {
   return bit_field8(pos_table_id);
}

bool ISO13818_Section::section_syntax_indicator() const {
   return bit_field1(pos_section_syntax_indicator);
}

uint16 ISO13818_Section::section_length() const {
   return bit_field16(pos_section_length);
}

int ISO13818_Section::append(uint8 continuity_counter, const ByteArray &data, int off, int len) {
   if (incr_cc(last_cc) != continuity_counter) return -1;
   last_cc = continuity_counter;
   return BitStream::append(data, off, len);
}
