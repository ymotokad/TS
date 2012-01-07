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
#define IMPLEMENTING_PROGRAMASSOCIATIONSECTION
#include "ProgramAssociationSection.h"

FIELDWIDTH_PREAMBLE(ProgramAssociationSection)
FIELDWIDTH_CONTENT(pos_table_id,			8)
FIELDWIDTH_CONTENT(pos_section_syntax_indicator,	1)
FIELDWIDTH_CONTENT(pos_reserved1,			3)
FIELDWIDTH_CONTENT(pos_section_length,			12)
FIELDWIDTH_CONTENT(pos_transport_stream_id,		16)
FIELDWIDTH_CONTENT(pos_reserved2,			2)
FIELDWIDTH_CONTENT(pos_version_number,			5)
FIELDWIDTH_CONTENT(pos_current_next_indicator,		1)
FIELDWIDTH_CONTENT(pos_section_number,			8)
FIELDWIDTH_CONTENT(pos_last_next_indicator,		8)
FIELDWIDTH_CONTENT(pos_START_PROGRAM_DATA,		0)
FIELDWIDTH_POSTAMBLE(ProgramAssociationSection, /**/)

/*
 * other methods
 */

int ProgramAssociationSection::numPrograms() const {
   assert(isComplete());
   int len = section_length()
      - (sizeofBufferBefore(pos_START_PROGRAM_DATA) - sizeofBufferBefore(pos_section_length + 1))
      - 4; // CRC_32
   assert((len % 4) == 0);
   return len / 4;
}

uint16 ProgramAssociationSection::program_number(int idx) const {
   int off = sizeofBufferBefore(pos_START_PROGRAM_DATA);
   return byteAt(off + 4 * idx) << 8 | byteAt(off + 4 * idx + 1);
}

uint16 ProgramAssociationSection::network_PID(int idx) const {
   int off = sizeofBufferBefore(pos_START_PROGRAM_DATA);
   return (byteAt(off + idx * 4 + 2) & 0x1f) << 8 | byteAt(off + idx * 4 + 3);
}

uint16 ProgramAssociationSection::program_map_PID(int idx) const {
   return network_PID(idx);
}

void ProgramAssociationSection::dump(std::ostream *osp) const {
   //*osp << "  table_id=" << std::hex << std::showbase << (int)table_id() << std::endl;
   *osp << "  section_length=" << std::dec << section_length() << std::endl;
   *osp << "  section_number=" << std::dec << (int)section_number() << std::endl;
   *osp << "  last_section_number=" << std::dec << (int)last_section_number() << std::endl;
   int len = section_length()
      - (sizeofBufferBefore(pos_START_PROGRAM_DATA) - sizeofBufferBefore(pos_section_length + 1))
      - 4; // CRC_32
   for (int i = 0; (i * 4) < len; i++) {
      *osp << "  program_number=" << std::dec << (int)program_number(i) 
	   << " = " << std::hex << std::showbase << (int)network_PID(i) << std::endl;
   }
   hexdump(2, osp);
}
