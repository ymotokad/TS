#ifndef PROGRAMASSOCIATIONSECTION_H
#define PROGRAMASSOCIATIONSECTION_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_PROGRAMASSOCIATIONSECTION
static const char *rcsid_ProgramAssociationSection = "@(#)$Id$";
#endif /* IMPLEMENTING_PROGRAMASSOCIATIONSECTION */
#include "Table.h"

class ProgramAssociationSection : public Section {
 public:
   ProgramAssociationSection();
   void dump(std::ostream *osp) const;
   int numPrograms() const;

   uint16 transport_stream_id() const;
   uint8 version_number() const;
   bool current_next_indicator() const;
   uint8 section_number() const;
   uint8 last_section_number() const;
   uint16 program_number(int idx) const;
   uint16 network_PID(int idx) const;
   uint16 program_map_PID(int idx) const;

protected:
   // Order of bit stream data
   static const int pos_transport_stream_id		= 4;
   static const int pos_reserved2			= 5;
   static const int pos_version_number			= 6;
   static const int pos_current_next_indicator		= 7;
   static const int pos_section_number			= 8;
   static const int pos_last_section_number		= 9;
   static const int pos_START_PROGRAM_DATA		= 10;
      
 protected:
   void initobj();
};


/*
 * Order of data
 */


inline uint16 ProgramAssociationSection::transport_stream_id() const {
   return bit_field16(pos_transport_stream_id);
}

inline uint8 ProgramAssociationSection::version_number() const {
   return bit_field8(pos_version_number);
}

inline bool ProgramAssociationSection::current_next_indicator() const {
   return bit_field1(pos_current_next_indicator);
}

inline uint8 ProgramAssociationSection::section_number() const {
   return bit_field8(pos_section_number);
}

inline uint8 ProgramAssociationSection::last_section_number() const {
   return bit_field8(pos_last_section_number);
}

#endif /* PROGRAMASSOCIATIONSECTION_H */
