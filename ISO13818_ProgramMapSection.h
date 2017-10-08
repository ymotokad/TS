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

#ifndef ISO13818_PROGRAMMAPSECTION_H
#define ISO13818_PROGRAMMAPSECTION_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_ISO13818_PROGRAMMAPSECTION
static const char *rcsid_ISO13818_ProgramMapSection = "@(#)$Id$";
#endif /* IMPLEMENTING_ISO13818_PROGRAMMAPSECTION */
#include "ISO13818_Table.h"

typedef void (*StreamCallback)(uint16 pid, uint16 program, uint8 sttype, uint8 component_tag, int data_component_id, void *dtp);

class ISO13818_ProgramMapSection : public ISO13818_Section {
 public:
   ISO13818_ProgramMapSection();
   ISO13818_ProgramMapSection(uint8 continuous_counter);
   void dump(std::ostream *osp) const;
   void for_all_streams(StreamCallback scp, void *) const;

   uint8 version_number() const;
   bool current_next_indicator() const;
   int section_number() const;
   int last_section_number() const;
   uint16 PCR_PID() const;
   uint16 program_number() const;
   int program_info_length() const;

   static const int sttype_VideoMPEG2			= 2;
   static const int sttype_Data				= 6;
   static const int sttype_AudioAAC			= 15;
   static const int sttype_AudioAACForMPEG4		= 17;

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



#endif /* ISO13818_PROGRAMMAPSECTION_H */
