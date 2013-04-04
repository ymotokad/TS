#ifndef EVENTINFORMATIONTABLE_H
#define EVENTINFORMATIONTABLE_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_EVENTINFORMATIONTABLE
static const char *rcsid_EventInformationTable = "@(#)$Id$";
#endif /* IMPLEMENTING_EVENTINFORMATIONTABLE */
#include "Table.h"
#include "MJD.h"

typedef void (*ComponentCallback)(uint16 program, const char *str, uint8 component_tag, void *dtp);

class EventInformationTable : public Section {
 public:
   EventInformationTable();
   EventInformationTable(uint8 continuous_counter);
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



#endif /* EVENTINFORMATIONTABLE_H */
