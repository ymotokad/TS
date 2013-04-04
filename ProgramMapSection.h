#ifndef PROGRAMMAPSECTION_H
#define PROGRAMMAPSECTION_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_PROGRAMMAPSECTION
static const char *rcsid_ProgramMapSection = "@(#)$Id$";
#endif /* IMPLEMENTING_PROGRAMMAPSECTION */
#include "Table.h"
#include "ProgramAssociationSection.h"

typedef void (*StreamCallback)(uint16 pid, uint16 program, const char *tagstr, uint8 component_tag, void *dtp);

class ProgramMapSection : public Section {
 public:
   ProgramMapSection();
   ProgramMapSection(uint8 continuous_counter);
   void dump(std::ostream *osp) const;
   void for_all_streams(StreamCallback scp, void *) const;

   uint8 version_number() const;
   bool current_next_indicator() const;
   int section_number() const;
   int last_section_number() const;
   uint16 PCR_PID() const;
   uint16 program_number() const;
   int program_info_length() const;

protected:
   // Order of bit stream data
   static const int pos_program_number			= 4;
   static const int pos_reserved2			= 5;
   static const int pos_version_number			= 6;
   static const int pos_current_next_indicator		= 7;
   static const int pos_section_number			= 8;
   static const int pos_last_section_number		= 9;
   static const int pos_reserved3			= 10;
   static const int pos_PCR_PID				= 11;
   static const int pos_reserved4			= 12;
   static const int pos_program_info_length		= 13;
   static const int pos_START_PROGRAM_DATA		= 14;

 protected:
   void initobj();
};



#endif /* PROGRAMMAPSECTION_H */
