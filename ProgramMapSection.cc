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
#include <string>
#define IMPLEMENTING_PROGRAMMAPSECTION
#include "ProgramMapSection.h"

static ProgramMapSection nullobj;
static const int the_field_width[] = {
   8,	// PMS_table_id					0
   1,	// PMS_section_syntax_indicator			1
   1,	// PMS_zero					2
   2,	// PMS_reserved1				3
   12,	// PMS_section_length				4
   16,	// PMS_program_number				5
   2,	// PMS_reserved2				6
   5,	// PMS_version_number				7
   1,	// PMS_current_next_indicator			8
   8,	// PMS_section_number				9
   8,	// PMS_last_section_number			10
   3,	// PMS_reserved3				11
   13,	// PMS_PCR_PID					12
   4,	// PMS_reserved4				13
   12,	// PMS_program_info_length			14
   0	// PMS_START_PROGRAM_DATA			15
};
static const char *stream_type_descriptions[] = {
   "ITU-T | ISO/IEC Reserved",
   "ISO/IEC 11172 Video",
   "ITU-T Rec. H.262 | ISO/IEC 13818-2 Video or ISO/IEC 11172-2 constrained parameter video stream",
   "ISO/IEC 11172 Audio",
   "ISO/IEC 13818-3 Audio",
   "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 private_sections",
   "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 PES packets containing private data",
   "ISO/IEC 13522 MHEG",
   "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Annex A DSM-CC",
   "ITU-T Rec. H.222.1",
   "ISO/IEC 13818-6 type A",
   "ISO/IEC 13818-6 type B",
   "ISO/IEC 13818-6 type C",
   "ISO/IEC 13818-6 type D",
   "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 auxiliary",
   "ISO/IEC 13818-7 Audio with ADTS transport syntax",
   "ISO/IEC 14496-2 Visual",
   "ISO/IEC 14496-3 Audio with the LATM transport syntax as defined in ISO/IEC 14496-3 / AMD 1",
   "ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in PES packets",
   "ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in ISO/IEC14496_sections.",
   "ISO/IEC 13818-6 Synchronized Download Protocol"
};

inline const char*stream_type(int sid) {
   if (sid < sizeof(stream_type_descriptions) / sizeof(*stream_type_descriptions)) {
      return stream_type_descriptions[sid];
   }
   if (sid < 0x7f) return "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved";
   return "User Provate";
}

/*
 * constructors 
 */
ProgramMapSection::ProgramMapSection() {
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
int ProgramMapSection::load(TSContext *tsc, std::istream *isp) {
   return 0;
}
int ProgramMapSection::load(const ByteArray *data) {
   assert(getBuffer() == NULL);
   ByteArrayBuffer *buf = new ByteArrayBuffer(*data);
   setBuffer(buf);
   setFullLength(sizeofBufferBefore(PMS_section_length + 1) + section_length());
   return buf->length();
}

void ProgramMapSection::process(TSContext *tsc) {
   /*
   assert(getBuffer()->length() >= sizeofBufferBefore(PMS_section_length + 1) + section_length());
   if (table_id() != TableID_TSProgramMapSection) {
      hexdump(&std::cout, 2);
      assert(table_id() == TableID_TSProgramMapSection);
   }
   */

   int len = program_info_length();
   for (int i = 0; (i * 4) < len; i++) {
      // descriptor
   }
}

void ProgramMapSection::dump(std::ostream *osp) const {
   if (table_id() != 2) {
      hexdump(osp, 2);
      return;
   }
   try {
      const char *tid;
      *osp << "  table_id=" << std::hex << std::showbase << (int)table_id() << std::endl;
      *osp << "  section_length=" << std::dec << section_length()
	   << " (short of " << (int)(sizeofBufferBefore(PMS_section_length + 1) + section_length() - getBuffer()->length()) << " bytes)" << std::endl;
      *osp << "  program_number=" << std::dec << program_number() << std::endl;
      *osp << "  current_next_indicator=" << std::dec << current_next_indicator() << std::endl;
      *osp << "  section_number=" << std::dec << section_number() << std::endl;
      *osp << "  last_section_number=" << std::dec << last_section_number() << std::endl;
      *osp << "  program_info_length=" << std::dec << program_info_length() << std::endl;

      int idx = sizeofBufferBefore(PMS_START_PROGRAM_DATA);
      int idxmax = idx + program_info_length();
      while (idx < idxmax) {
	 int desctag = getBuffer()->at(idx);
	 int desclen = getBuffer()->at(idx + 1);
	 *osp << "    tag=" << std::dec << desctag << std::endl;
	 *osp << "    len=" << std::dec << desclen << std::endl;
	 ByteArray *data = getBuffer()->subarray(idx, desclen + 2);
	 hexdump(osp, data, 4, -1);
	 delete data;
	 idx += 2 + desclen;
      }
      *osp << "  stream info" << std::endl;
      idxmax = sizeofBufferBefore(PMS_section_length + 1) + section_length() - 4;
      while (idx < idxmax) {
	 int sttype = getBuffer()->at(idx);
	 int elmpid = getBuffer()->at(idx + 1) << 8 | getBuffer()->at(idx + 2);
	 elmpid &= 0x1fff;
	 int eslen = getBuffer()->at(idx + 3) << 8 | getBuffer()->at(idx + 4);
	 eslen &= 0x0fff;
	 *osp << "    --stream_type=" << std::hex << std::showbase << stream_type(sttype) << std::endl;
	 *osp << "    elm_PID=" << std::hex << std::showbase << elmpid << std::endl;
	 *osp << "    ES_len=" << std::dec << eslen << std::endl;
	 ByteArray *data = getBuffer()->subarray(idx, 5 + eslen);
	 hexdump(osp, data, 4, -1);
	 delete data;
	 idx += 5 + eslen;
      }
   } catch (ByteArrayOverflowException e) {
      *osp << "  buffer underflow!!" << std::endl;
   }
}

uint8 ProgramMapSection::table_id() const {
   return bit_field8(PMS_table_id);
};

int ProgramMapSection::section_length() const {
   return bit_field16(PMS_section_length);
};

int ProgramMapSection::section_number() const {
   return bit_field8(PMS_section_number);
};

bool ProgramMapSection::current_next_indicator() const {
   return bit_field1(PMS_current_next_indicator);
}

int ProgramMapSection::last_section_number() const {
   return bit_field8(PMS_last_section_number);
};

uint16 ProgramMapSection::program_number() const {
   return bit_field16(PMS_program_number);
};

int ProgramMapSection::program_info_length() const {
   return bit_field16(PMS_program_info_length);
};
