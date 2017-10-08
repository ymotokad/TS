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
#define IMPLEMENTING_ISO13818_PROGRAMASSOCIATIONSECTION
#include "ISO13818_ProgramAssociationSection.h"

FIELDWIDTH_PREAMBLE(ISO13818_ProgramAssociationSection)
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
FIELDWIDTH_POSTAMBLE(ISO13818_ProgramAssociationSection, /**/)

/*
 * other methods
 */

int ISO13818_ProgramAssociationSection::numPrograms() const {
   assert(isComplete());
   int len = section_length()
      - (sizeofBufferBefore(pos_START_PROGRAM_DATA) - sizeofBufferBefore(pos_section_length + 1))
      - 4; // CRC_32
   if ((len % 4) != 0) {
      logger->warning("ISO13818_ProgramAssociationSection: length %d is not a multiple of four", len);
   }
   return len / 4;
}

uint16 ISO13818_ProgramAssociationSection::program_number(int idx) const {
   int off = sizeofBufferBefore(pos_START_PROGRAM_DATA);
   return byteAt(off + 4 * idx) << 8 | byteAt(off + 4 * idx + 1);
}

uint16 ISO13818_ProgramAssociationSection::network_PID(int idx) const {
   int off = sizeofBufferBefore(pos_START_PROGRAM_DATA);
   return (byteAt(off + idx * 4 + 2) & 0x1f) << 8 | byteAt(off + idx * 4 + 3);
}

uint16 ISO13818_ProgramAssociationSection::program_map_PID(int idx) const {
   return network_PID(idx);
}

void ISO13818_ProgramAssociationSection::dump(std::ostream *osp) const {
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
