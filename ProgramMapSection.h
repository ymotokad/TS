#ifndef PROGRAMMAPSECTION_H
#define PROGRAMMAPSECTION_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_PROGRAMMAPSECTION
static const char *rcsid_ProgramMapSection = "@(#)$Id$";
#endif /* IMPLEMENTING_PROGRAMMAPSECTION */
#include "PacketSection.h"
#include "ProgramAssociationSection.h"

class ProgramMapSection : public PacketSection {
 public:
   ProgramMapSection();
   int load(TSContext *tsc, std::istream *inputstream);
   int load(const ByteArray * data);
   void process(TSContext *tsc);
   void dump(std::ostream *outputstream) const;
 protected:
   uint8 table_id() const;
   int section_length() const;
   bool current_next_indicator() const;
   int section_number() const;
   int last_section_number() const;
   uint16 program_number() const;
   int program_info_length() const;
};


/*
 * Order of data
 */
#define PMS_table_id					0
#define PMS_section_syntax_indicator			1
#define PMS_zero					2
#define PMS_reserved1					3
#define PMS_section_length				4
#define PMS_program_number				5
#define PMS_reserved2					6
#define PMS_version_number				7
#define PMS_current_next_indicator			8
#define PMS_section_number				9
#define PMS_last_section_number				10
#define PMS_reserved3					11
#define PMS_PCR_PID					12
#define PMS_reserved4					13
#define PMS_program_info_length				14
#define PMS_START_PROGRAM_DATA				15


#endif /* PROGRAMMAPSECTION_H */
