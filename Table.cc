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
#include <iostream>
#define IMPLEMENTING_TABLE
#include "Table.h"

inline uint8 incr_cc(uint8 cc) {
   return (cc + 1) & 0xf;
}


/*
 * Section class
 */

FIELDWIDTH_PREAMBLE(Section)
FIELDWIDTH_CONTENT(pos_table_id,			8)
FIELDWIDTH_CONTENT(pos_section_syntax_indicator,	2)
FIELDWIDTH_CONTENT(pos_reserved1,			2)
FIELDWIDTH_CONTENT(pos_section_length,			12)
FIELDWIDTH_CONTENT(pos_start_of_section,		0)
FIELDWIDTH_POSTAMBLE(Section, /**/)

Section::Section(uint8 continuity_counter) : last_cc(continuity_counter) {
   initobj();
}

Section::~Section() {
}

bool Section::isComplete() const {
   assert(bufferAllocated());
   if (bufferLength() < sizeofBufferBefore(pos_start_of_section)) return false;
   int expected_length = sizeofBufferBefore(pos_start_of_section) + section_length();
   return expected_length <= bufferLength();
}

uint8 Section::table_id() const {
   return bit_field8(pos_table_id);
}

bool Section::section_syntax_indicator() const {
   return bit_field1(pos_section_syntax_indicator);
}

uint16 Section::section_length() const {
   return bit_field16(pos_section_length);
}

int Section::append(uint8 continuity_counter, const ByteArray &data, int off, int len) {
   if (incr_cc(last_cc) != continuity_counter) return -1;
   last_cc = continuity_counter;
   return BitStream::append(data, off, len);
}
