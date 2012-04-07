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
#include <stdio.h>
#include <iostream>
#define IMPLEMENTING_EVENTINFORMATIONTABLE
#include "Descriptors.h"
#include "EventInformationTable.h"


/*
 * EventInformationTable class
 */

FIELDWIDTH_PREAMBLE(EventInformationTable)
FIELDWIDTH_CONTENT(pos_table_id,			8)
FIELDWIDTH_CONTENT(pos_section_syntax_indicator,	1)
FIELDWIDTH_CONTENT(pos_reserved1,			3)
FIELDWIDTH_CONTENT(pos_section_length,			12)
FIELDWIDTH_CONTENT(pos_service_id,			16)
FIELDWIDTH_CONTENT(pos_reserved2,			2)
FIELDWIDTH_CONTENT(pos_version_number,			5)
FIELDWIDTH_CONTENT(pos_current_next_indicator,		1)
FIELDWIDTH_CONTENT(pos_section_number,			8)
FIELDWIDTH_CONTENT(pos_last_section_number,		8)
FIELDWIDTH_CONTENT(pos_transport_stream_id,		16)
FIELDWIDTH_CONTENT(pos_original_network_id,		16)
FIELDWIDTH_CONTENT(pos_segment_last_section_number,	8)
FIELDWIDTH_CONTENT(pos_last_table_id,			8)
FIELDWIDTH_CONTENT(pos_start_of_individual_events,	0)
FIELDWIDTH_POSTAMBLE(EventInformationTable, /**/)

EventInformationTable::EventInformationTable(uint8 continuous_counter) : Section(continuous_counter) {
   initobj();
}

void EventInformationTable::dump(std::ostream *osp) const {
   assert(TableID_EventInformationTable_Actual_Present <= table_id() &&
	  table_id() <= TableID_EventInformationTable_max);
   if (table_id() != TableID_EventInformationTable_Actual_Present) return; // For now
   printf("*** Event Information Table ***\n");
   printf("  table_id=0x%02x\n", table_id());
   printf("  section_length=%d\n", section_length());
   printf("  service_id=%d\n", service_id());
   printf("  version_number=%d\n", version_number());
   printf("  current_next_indicator=%d\n", current_next_indicator());
   printf("  section_number=%d\n", section_number());
   printf("  transport_stream_id=0x%04x\n", transport_stream_id());
   printf("  original_network_id=0x%04x\n", original_network_id());
   printf("  last_section_number=%d\n", last_section_number());
   int length = section_length()
      - (sizeofBufferBefore(pos_start_of_individual_events) - sizeofBufferBefore(pos_section_length + 1))
      - 4; // CRC_32
   int evtlen;
   for (int idx = 0; idx < length; idx += evtlen) {
      IndividualEvent evt;
      evt.setBuffer(*this, sizeofBufferBefore(pos_start_of_individual_events) + idx, -1);
      evt.dump(osp);
      evtlen = evt.length();
   }
}

uint16 EventInformationTable::service_id() const {
   return bit_field16(pos_service_id);
}

uint16 EventInformationTable::transport_stream_id() const {
   return bit_field16(pos_transport_stream_id);
}

uint16 EventInformationTable::original_network_id() const {
   return bit_field16(pos_original_network_id);
}

uint8 EventInformationTable::version_number() const {
   return bit_field8(pos_version_number);
}

bool EventInformationTable::current_next_indicator() const {
   return bit_field1(pos_current_next_indicator);
}

uint8 EventInformationTable::section_number() const {
   return bit_field8(pos_section_number);
}

uint8 EventInformationTable::last_section_number() const {
   return bit_field8(pos_last_section_number);
}


/*
 * IndividualEvent
 */

FIELDWIDTH_PREAMBLE(IndividualEvent)
FIELDWIDTH_CONTENT(pos_event_id,			16)
FIELDWIDTH_CONTENT(pos_start_time_ymd,			16)
FIELDWIDTH_CONTENT(pos_start_time_hms,			24)
FIELDWIDTH_CONTENT(pos_duration,			24)
FIELDWIDTH_CONTENT(pos_running_status,			3)
FIELDWIDTH_CONTENT(pos_free_CA_mode,			1)
FIELDWIDTH_CONTENT(pos_descriptors_loop_length,		12)
FIELDWIDTH_CONTENT(pos_start_of_descriptors,		0)
FIELDWIDTH_POSTAMBLE(IndividualEvent, /**/)

int IndividualEvent::length() const {
   return sizeofBufferBefore(pos_descriptors_loop_length + 1) + descriptors_loop_length();
}

uint16 IndividualEvent::event_id() const {
   return bit_field16(pos_event_id);
}

uint16 IndividualEvent::start_time_ymd() const {
   return bit_field16(pos_start_time_ymd);
}

uint32 IndividualEvent::start_time_hms() const {
   return bit_field32(pos_start_time_hms);
}

uint32 IndividualEvent::duration() const {
   return bit_field32(pos_duration);
}

uint8 IndividualEvent::running_status() const {
   return bit_field8(pos_running_status);
}

uint16 IndividualEvent::descriptors_loop_length() const {
   return bit_field16(pos_descriptors_loop_length);
}


void IndividualEvent::dump(std::ostream *osp) const {
   MJD start_time(start_time_ymd(), start_time_hms());

   printf("    --event_id=0x%x, len=%d\n", event_id(), length());
   printf("    start_time=%s, duration=%d\n", start_time.datetime(), duration());
   printf("    descriptor_loop_length=%d\n", descriptors_loop_length());
   int len = descriptors_loop_length();
   int size;
   for (int i = 0; i < len; i += size) {
      int idx = sizeofBufferBefore(pos_start_of_descriptors) + i;
      int desctag = byteAt(idx);
      int desclen = byteAt(idx + 1);
      printf("      descriptor=%s, len=%d\n", Descriptor::getName(desctag), desclen);
      
      switch (desctag) {
      case Descriptor::tag_service:
	 {
	    Desc_Service desc;
	    desc.setBuffer(*this, idx, 2 + desclen);
	    printf("        service_type=[%s]\n", desc.service_type_string());
	    printf("        service_provider_name=[%s]\n", desc.service_provider_name_string());
	    printf("        service_name=[%s]\n", desc.service_name_string());
	 }
	 break;
      case Descriptor::tag_component:
	 {
	    Desc_Component desc;
	    desc.setBuffer(*this, idx, 2 + desclen);
	    printf("        component is [%s]\n", desc.component_string());
	    printf("        component_tag=0x%02x\n", desc.component_tag());
	 }
	 break;
      case Descriptor::tag_audio_component:
	 {
	    Desc_AudioComponent desc;
	    desc.setBuffer(*this, idx, 2 + desclen);
	    printf("        component is [%s]\n", desc.component_string());
	    printf("        component_tag=0x%02x\n", desc.component_tag());
	 }
	 break;
      default:
	 hexdump(8, osp, idx, 2 + desclen);
	 break;
      }

      size = 2 + desclen;
   }
}
