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

static ProgramAssociationSection nullobj;
static const int the_field_width[] = {
   8,	// PAS_table_id					0
   1,	// PAS_section_syntax_indicator			1
   1,	// PAS_zero					2
   2,	// PAS_reserved1				3
   12,	// PAS_section_length				4
   16,	// PAS_transport_stream_id			5
   2,	// PAS_reserved2				6
   5,	// PAS_version_number				7
   1,	// PAS_current_next_indicator			8
   8,	// PAS_section_number				9
   8,	// PAS_last_next_indicator			10
   0	// PAS_START_PROGRAM_DATA			11
};

/*
 * constructors 
 */
ProgramAssociationSection::ProgramAssociationSection() {
   static int *the_bit_distance = NULL;

   if (this == &nullobj) {
      int len = sizeof(the_field_width) / sizeof(the_field_width[0]);
      the_bit_distance = new int[len + 1];
      initializeBitDistance(the_field_width, len, the_bit_distance);
   }
   assert(the_bit_distance != NULL);
   setBitDistance(the_bit_distance);
}

/*
 * destructors 
 */

/*
 * other methods
 */
int ProgramAssociationSection::load(TSContext *tsc, std::istream *isp) {
   return 0;
}
int ProgramAssociationSection::load(const ByteArray *data) {
   setBuffer(data);
   setFullLength(sizeofBufferBefore(PAS_section_length + 1) + section_length());
   return data->length();
}

void ProgramAssociationSection::process(TSContext *tsc) {
   int len = section_length()
      - (sizeofBufferBefore(PAS_START_PROGRAM_DATA) - sizeofBufferBefore(PAS_section_length + 1))
      - 4; // CRC_32
   for (int i = 0; (i * 4) < len; i++) {
      uint16 pno = program_number(i);
      if (pno == 0) {
	 // Network PID
      } else {
	 tsc->setProgramMapTablePID(program_map_PID(i), pno);
      }
   }
}


void ProgramAssociationSection::dump(std::ostream *osp) const {
   //*osp << "  table_id=" << std::hex << std::showbase << (int)table_id() << std::endl;
   *osp << "  section_length=" << std::dec << section_length() << std::endl;
   *osp << "  section_number=" << std::dec << (int)section_number() << std::endl;
   *osp << "  last_section_number=" << std::dec << (int)last_section_number() << std::endl;
   int len = section_length()
      - (sizeofBufferBefore(PAS_START_PROGRAM_DATA) - sizeofBufferBefore(PAS_section_length + 1))
      - 4; // CRC_32
   for (int i = 0; (i * 4) < len; i++) {
      *osp << "  program_number=" << std::dec << (int)program_number(i) 
	   << " = " << std::hex << std::showbase << (int)network_PID(i) << std::endl;
   }
   hexdump(osp, 2);
}


uint8 ProgramAssociationSection::table_id() const {
   return bit_field8(PAS_table_id);
}

bool ProgramAssociationSection::section_syntax_indicator() const {
   return bit_field1(PAS_section_syntax_indicator);
}

uint16 ProgramAssociationSection::section_length() const {
   return bit_field16(PAS_section_length);
}

uint16 ProgramAssociationSection::transport_stream_id() const {
   return bit_field16(PAS_transport_stream_id);
}

uint8 ProgramAssociationSection::version_number() const {
   return bit_field8(PAS_version_number);
}

bool ProgramAssociationSection::current_next_indicator() const {
   return bit_field1(PAS_current_next_indicator);
}

uint8 ProgramAssociationSection::section_number() const {
   return bit_field8(PAS_section_number);
}

uint8 ProgramAssociationSection::last_section_number() const {
   return bit_field8(PAS_last_section_number);
}

uint16 ProgramAssociationSection::program_number(int idx) const {
   const ByteArray *buff = getBuffer();
   int off = sizeofBufferBefore(PAS_START_PROGRAM_DATA);
   return buff->at(off + 4 * idx) << 8 | buff->at(off + 4 * idx + 1);
}

uint16 ProgramAssociationSection::network_PID(int idx) const {
   const ByteArray *buff = getBuffer();
   int off = sizeofBufferBefore(PAS_START_PROGRAM_DATA);
   return (buff->at(off + idx * 4 + 2) & 0x1f) << 8 | buff->at(off + idx * 4 + 3);
}

uint16 ProgramAssociationSection::program_map_PID(int idx) const {
   return network_PID(idx);
}
