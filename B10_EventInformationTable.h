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

#ifndef B10_EVENTINFORMATIONTABLE_H
#define B10_EVENTINFORMATIONTABLE_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_B10_EVENTINFORMATIONTABLE
static const char *rcsid_B10_EventInformationTable = "@(#)$Id$";
#endif /* IMPLEMENTING_B10_EVENTINFORMATIONTABLE */
#include "ISO13818_Table.h"
#include "B10_MJD.h"

typedef void (*ComponentCallback)(uint16 program, const char *str, uint8 component_tag, void *dtp);

class B10_EventInformationTable : public ISO13818_Section {
 public:
   B10_EventInformationTable();
   B10_EventInformationTable(uint8 continuous_counter);
   void for_all_components(ComponentCallback ccp, void *) const;
   void dump(std::ostream *osp) const;

   uint16 service_id() const;
   uint8 version_number() const;
   bool current_next_indicator() const;
   uint8 section_number() const;
   uint8 last_section_number() const;
   uint16 transport_stream_id() const;
   uint16 original_network_id() const;

 protected:
   // Order of bit stream data
   static const int pos_service_id			= 4;
   static const int pos_reserved2			= 5;
   static const int pos_version_number			= 6;
   static const int pos_current_next_indicator		= 7;
   static const int pos_section_number			= 8;
   static const int pos_last_section_number		= 9;
   static const int pos_transport_stream_id		= 10;
   static const int pos_original_network_id		= 11;
   static const int pos_segment_last_section_number	= 12;
   static const int pos_last_table_id			= 13;
   static const int pos_start_of_individual_events	= 14;
   
 protected:
   void initobj();
};

class IndividualEvent : public BitStream {
 public:
   IndividualEvent();
   void dump(std::ostream *osp) const;
   void for_all_components(uint16 program, ComponentCallback ccp, void *) const;
   int length() const;

   uint16 event_id() const;
   uint16 start_time_ymd() const;
   uint32 start_time_hms() const;
   const char *start_time_string();
   uint32 duration() const;
   uint8 running_status() const;
   uint16 descriptors_loop_length() const;

 protected:
   // Order of bit stream data
   static const int pos_event_id			= 0;
   static const int pos_start_time_ymd			= 1;
   static const int pos_start_time_hms			= 2;
   static const int pos_duration			= 3;
   static const int pos_running_status			= 4;
   static const int pos_free_CA_mode			= 5;
   static const int pos_descriptors_loop_length		= 6;
   static const int pos_start_of_descriptors		= 7;
   
 protected:
   void initobj();
};



#endif /* B10_EVENTINFORMATIONTABLE_H */
