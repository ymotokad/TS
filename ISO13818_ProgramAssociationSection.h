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

#ifndef ISO13818_PROGRAMASSOCIATIONSECTION_H
#define ISO13818_PROGRAMASSOCIATIONSECTION_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_ISO13818_PROGRAMASSOCIATIONSECTION
static const char *rcsid_ISO13818_ProgramAssociationSection = "@(#)$Id$";
#endif /* IMPLEMENTING_ISO13818_PROGRAMASSOCIATIONSECTION */
#include "ISO13818_Table.h"

class ISO13818_ProgramAssociationSection : public ISO13818_Section {
 public:
   ISO13818_ProgramAssociationSection();
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


inline uint16 ISO13818_ProgramAssociationSection::transport_stream_id() const {
   return bit_field16(pos_transport_stream_id);
}

inline uint8 ISO13818_ProgramAssociationSection::version_number() const {
   return bit_field8(pos_version_number);
}

inline bool ISO13818_ProgramAssociationSection::current_next_indicator() const {
   return bit_field1(pos_current_next_indicator);
}

inline uint8 ISO13818_ProgramAssociationSection::section_number() const {
   return bit_field8(pos_section_number);
}

inline uint8 ISO13818_ProgramAssociationSection::last_section_number() const {
   return bit_field8(pos_last_section_number);
}

#endif /* ISO13818_PROGRAMASSOCIATIONSECTION_H */
