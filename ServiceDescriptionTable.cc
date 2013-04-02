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
#define IMPLEMENTING_SERVICEDESCRIPTIONTABLE
#include "Descriptors.h"
#include "ServiceDescriptionTable.h"


/*
 * ServiceDescriptionTable class
 */

FIELDWIDTH_PREAMBLE(ServiceDescriptionTable)
FIELDWIDTH_CONTENT(pos_table_id,			8)
FIELDWIDTH_CONTENT(pos_section_syntax_indicator,	1)
FIELDWIDTH_CONTENT(pos_reserved1,			3)
FIELDWIDTH_CONTENT(pos_section_length,			12)
FIELDWIDTH_CONTENT(pos_transport_stream_id,		16)
FIELDWIDTH_CONTENT(pos_reserved2,			2)
FIELDWIDTH_CONTENT(pos_version_number,			5)
FIELDWIDTH_CONTENT(pos_current_next_indicator,		1)
FIELDWIDTH_CONTENT(pos_section_number,			8)
FIELDWIDTH_CONTENT(pos_last_section_number,		8)
FIELDWIDTH_CONTENT(pos_original_network_id,		16)
FIELDWIDTH_CONTENT(pos_reserved3,			8)
FIELDWIDTH_CONTENT(pos_start_of_individual_services,	0)
FIELDWIDTH_POSTAMBLE(ServiceDescriptionTable, /**/)

void ServiceDescriptionTable::dump(std::ostream *osp) const {
   assert(table_id() == TableID_ServiceDescriptionTable_Actual ||
	  table_id() == TableID_ServiceDescriptionTable_Other);
   printf("  -- Service Description Table ---\n");
   printf("  table_id=0x%02x\n", table_id());
   printf("  section_length=%d\n", section_length());
   printf("  transport_stream_id=0x%04x\n", transport_stream_id());
   int length = section_length()
      - (sizeofBufferBefore(pos_start_of_individual_services) - sizeofBufferBefore(pos_section_length + 1))
      - 4; // CRC_32
   int svclen;
   for (int idx = 0; idx < length; idx += svclen) {
      IndividualService svc;
      svc.setBuffer(*this, sizeofBufferBefore(pos_start_of_individual_services) + idx, -1);
      svc.dump(osp);
      svclen = svc.length();
   }
}

uint16 ServiceDescriptionTable::transport_stream_id() const {
   return bit_field16(pos_transport_stream_id);
}

uint8 ServiceDescriptionTable::version_number() const {
   return bit_field8(pos_version_number);
}

bool ServiceDescriptionTable::current_next_indicator() const {
   return bit_field1(pos_current_next_indicator);
}

uint8 ServiceDescriptionTable::section_number() const {
   return bit_field8(pos_section_number);
}

uint8 ServiceDescriptionTable::last_section_number() const {
   return bit_field8(pos_last_section_number);
}


/*
 * IndividualService
 */

FIELDWIDTH_PREAMBLE(IndividualService)
FIELDWIDTH_CONTENT(pos_service_id,			16)
FIELDWIDTH_CONTENT(pos_reserved,			3)
FIELDWIDTH_CONTENT(pos_EIT_user_defined_flags,		3)
FIELDWIDTH_CONTENT(pos_EIT_schedule_flag,		1)
FIELDWIDTH_CONTENT(pos_EIT_present_following_floag,	1)
FIELDWIDTH_CONTENT(pos_running_status,			3)
FIELDWIDTH_CONTENT(pos_free_CA_mode,			1)
FIELDWIDTH_CONTENT(pos_descriptors_loop_length,		12)
FIELDWIDTH_CONTENT(pos_start_of_descriptors,		0)
FIELDWIDTH_POSTAMBLE(IndividualService, /**/)

int IndividualService::length() const {
   return sizeofBufferBefore(pos_descriptors_loop_length + 1) + descriptors_loop_length();
}

uint16 IndividualService::service_id() const {
   return bit_field16(pos_service_id);
}

uint8 IndividualService::EIT_user_defined_flags() const {
   return bit_field8(pos_EIT_user_defined_flags);
}

bool IndividualService::EIT_schedule_flag() const {
   return bit_field1(pos_EIT_schedule_flag);
}

bool IndividualService::EIT_present_following_flag() const {
   return bit_field1(pos_EIT_present_following_flag);
}

uint8 IndividualService::running_status() const {
   return bit_field8(pos_running_status);
}

bool IndividualService::free_CA_mode() const {
   return bit_field1(pos_free_CA_mode);
}

uint16 IndividualService::descriptors_loop_length() const {
   return bit_field16(pos_descriptors_loop_length);
}


void IndividualService::dump(std::ostream *osp) const {
   printf("  --service_id(program number)=%d\n", service_id());
   int len = descriptors_loop_length();
   int size;
   for (int i = 0; i < len; i += size) {
      int idx = sizeofBufferBefore(pos_start_of_descriptors) + i;
      int desctag = byteAt(idx);
      int desclen = byteAt(idx + 1);
      printf("    descriptor=%s, len=%d\n", Descriptor::getName(desctag), desclen);
      
      switch (desctag) {
      case Descriptor::tag_service:
	 {
	    Desc_Service desc;
	    desc.setBuffer(*this, idx);
	    printf("      service_type=[%s]\n", desc.service_type_string());
	    printf("      service_provider_name=[%s]\n", desc.service_provider_name_string());
	    printf("      service_name=[%s]\n", desc.service_name_string());
	 }
	 break;
      default:
	 hexdump(6, osp, idx, 2 + desclen);
      }

      size = 2 + desclen;
   }
}
