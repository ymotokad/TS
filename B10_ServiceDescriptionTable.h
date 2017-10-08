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

#ifndef B10_SERVICEDESCRIPTIONTABLE_H
#define B10_SERVICEDESCRIPTIONTABLE_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_B10_SERVICEDESCRIPTIONTABLE
static const char *rcsid_B10_ServiceDescriptionTable = "@(#)$Id$";
#endif /* IMPLEMENTING_B10_SERVICEDESCRIPTIONTABLE */
#include "ISO13818_Table.h"

class B10_ServiceDescriptionTable : public ISO13818_Section {
 public:
   B10_ServiceDescriptionTable();
   void dump(std::ostream *osp) const;

   uint16 transport_stream_id() const;
   uint8 version_number() const;
   bool current_next_indicator() const;
   uint8 section_number() const;
   uint8 last_section_number() const;

 protected:
   // Order of bit stream data
   static const int pos_transport_stream_id		= 4;
   static const int pos_reserved2			= 5;
   static const int pos_version_number			= 6;
   static const int pos_current_next_indicator		= 7;
   static const int pos_section_number			= 8;
   static const int pos_last_section_number		= 9;
   static const int pos_original_network_id		= 10;
   static const int pos_reserved3			= 11;
   static const int pos_start_of_individual_services	= 12;
   
 protected:
   void initobj();
};

class IndividualService : public BitStream {
 public:
   IndividualService();
   void dump(std::ostream *osp) const;
   int length() const;

   uint16 service_id() const;
   uint8 EIT_user_defined_flags() const;
   bool EIT_schedule_flag() const;
   bool EIT_present_following_flag() const;
   uint8 running_status() const;
   bool free_CA_mode() const;
   uint16 descriptors_loop_length() const;

 protected:
   // Order of bit stream data
   static const int pos_service_id			= 0;
   static const int pos_reserved			= 1;
   static const int pos_EIT_user_defined_flags		= 2;
   static const int pos_EIT_schedule_flag		= 3;
   static const int pos_EIT_present_following_flag	= 4;
   static const int pos_running_status			= 5;
   static const int pos_free_CA_mode			= 6;
   static const int pos_descriptors_loop_length		= 7;
   static const int pos_start_of_descriptors		= 8;
   
 protected:
   void initobj();
};


#endif /* B10_SERVICEDESCRIPTIONTABLE_H */
