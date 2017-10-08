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
static char rcsid[] = "@(#)$Id$";
#ifdef __GNUG__
#pragma implementation
#endif
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <map>
#define IMPLEMENTING_B10_DESCRIPTORS
#include "B10_Descriptors.h"


/*
 * B10_Descriptor class
 */
Id2StringMap B10_Descriptor::descriptors;
void B10_Descriptor::initDescriptors() {
   descriptors[2] = "Video Stream";
   descriptors[3] = "Audio Stream";
   descriptors[tag_conditional_access] = "Conditional Access";
   descriptors[10] = "ISO 639 Language";
   descriptors[27] = "MPEG-4 Video";
   descriptors[28] = "MPEG-4 Audio";
   // Defined at "6.2.13 Service Descriptor" in ARIB STD B10 Section 2
   descriptors[tag_service] = "Service";
   // Defined at "6.2.15 Short Event Descriptor" in ARIB STD B10 Section 2
   descriptors[tag_short_event] = "Short Event";
   // Defined at "6.2.3 Component Descriptor" in ARIB STD B10 Section 2
   descriptors[tag_component] = "Component";
   // Defined at "6.2.16 Component Descriptor" in ARIB STD B10 Section 2
   descriptors[tag_stream_identifier] = "Stream Identifier";
   // Defined at "6.2.4 Content Descriptor" in ARIB STD B10 Section 2
   descriptors[tag_content] = "Content";
   descriptors[0xc1] = "Digital Copy Control";
   descriptors[0xcf] = "Logo Transmission";
   // Defined at "6.2.26 Audio Component Descriptor" in ARIB STD B10 Section 2
   descriptors[tag_audio_component] = "Audio Component";
   // Defined at "6.2.30 Video Decode Control Descriptor" in ARIB STD B10 Section 2
   descriptors[tag_video_decode_control] = "Video Decode Control";
   // Defined at "6.2.45 Content Availability Descriptor" in ARIB STD B10 Section 2
   descriptors[0xde] = "Content Availability";
   // Defined at "6.2.20 Data Component Descriptor" in ARIB STD B10 Section 2
   descriptors[0xfd] = "Data Component";
}
const char *B10_Descriptor::getName(uint8 tag) {
   static char buff[8]; // This is really thread-unsafe
   static bool initialized = false;
   if (!initialized) {
      initDescriptors();
      initialized = true;
   }

   Id2StringMap::const_iterator itr = descriptors.find(tag);
   if (itr == descriptors.end()) {
      sprintf(buff, "0x%02x", tag);
      return buff;
   }
   return itr->second;
}


/*
 * B10_Desc_VideoDecodeControl class
 */
FIELDWIDTH_PREAMBLE(B10_Desc_VideoDecodeControl)
FIELDWIDTH_CONTENT(B10_Descriptor::pos_descriptor_tag,		8)
FIELDWIDTH_CONTENT(B10_Descriptor::pos_descriptor_length,	8)
FIELDWIDTH_CONTENT(pos_still_picture_flag,			1)
FIELDWIDTH_CONTENT(pos_sequence_end_code_flag,			1)
FIELDWIDTH_CONTENT(pos_video_encode_format,			4)
FIELDWIDTH_CONTENT(pos_reserved_future_use,			2)
FIELDWIDTH_CONTENT(pos_end_of_data,				0)
FIELDWIDTH_POSTAMBLE(B10_Desc_VideoDecodeControl, /**/)

uint8 B10_Desc_VideoDecodeControl::video_encode_format() const {
   return bit_field8(pos_video_encode_format);
}

const char *B10_Desc_VideoDecodeControl::video_encode_format_string() const {
   static const char *videDecodeControls[] = {
   "1080p",	"1080i",	"720p",		"480p",
   "480i",	"240p",		"120p",		"2160p",
   "rsvd",	"rsvd",		"rsvd",		"rsvd",
   "rsvd",	"rsvd",		"rsvd",		"rsvd"
   };

   return videDecodeControls[video_encode_format()];
};


/*
 * B10_Desc_StreamIdentifier class
 */
FIELDWIDTH_PREAMBLE(B10_Desc_StreamIdentifier)
FIELDWIDTH_CONTENT(B10_Descriptor::pos_descriptor_tag,		8)
FIELDWIDTH_CONTENT(B10_Descriptor::pos_descriptor_length,	8)
FIELDWIDTH_CONTENT(pos_component_tag,				8)
FIELDWIDTH_CONTENT(pos_end_of_data,				0)
FIELDWIDTH_POSTAMBLE(B10_Desc_StreamIdentifier, /**/)

uint8 B10_Desc_StreamIdentifier::component_tag() const {
   return bit_field8(pos_component_tag);
}
const char *B10_Desc_StreamIdentifier::component_tag_string() const {
   static char buff[8]; // This is really thread-unsafe
   static Id2StringMap components;
   static bool initialized = false;
   if (!initialized) {
      components[0xc1] = "720p, aspect 4:3";
      initialized = true;
   }
   
   Id2StringMap::const_iterator itr = components.find(component_tag());
   if (itr == components.end()) {
      sprintf(buff, "0x%02x", component_tag());
      return buff;
   }
   return itr->second;
   
}


/*
 * B10_Desc_Service class
 */
FIELDWIDTH_PREAMBLE(B10_Desc_Service)
FIELDWIDTH_CONTENT(B10_Descriptor::pos_descriptor_tag,		8)
FIELDWIDTH_CONTENT(B10_Descriptor::pos_descriptor_length,	8)
FIELDWIDTH_CONTENT(pos_service_type,				8)
FIELDWIDTH_CONTENT(pos_service_provider_name_length,		8)
FIELDWIDTH_CONTENT(pos_start_of_service_provider_name,		0)
FIELDWIDTH_POSTAMBLE(B10_Desc_Service, initvars())

void B10_Desc_Service::initvars() {
   service_name_str = NULL;
   service_provider_name_str = NULL;
}

B10_Desc_Service::~B10_Desc_Service() {
   if (service_name_str != NULL) delete service_name_str;
   if (service_provider_name_str != NULL) delete service_provider_name_str;
}

uint8 B10_Desc_Service::service_type() const {
   return bit_field8(pos_service_type);
}
const char *B10_Desc_Service::service_type_string() const {
   static char buff[8]; // This is really thread-unsafe
   static Id2StringMap types;
   static bool initialized = false;
   if (!initialized) {
      types[0x01] = "Digital TV Service";
      types[0x02] = "Digital Audio Service";
      types[0xc0] = "Data Service";
      initialized = true;
   }
   
   Id2StringMap::const_iterator itr = types.find(service_type());
   if (itr == types.end()) {
      sprintf(buff, "0x%02x", service_type());
      return buff;
   }
   return itr->second;
}

uint8 B10_Desc_Service::service_provider_name_length() const {
   return bit_field8(pos_service_provider_name_length);
}

const char *B10_Desc_Service::service_provider_name_string()  {
   if (service_provider_name_str == NULL) {
      int len = service_provider_name_length();
      service_provider_name_str = new char[len + 1];
      int idx;
      int off = sizeofBufferBefore(pos_start_of_service_provider_name);
      for (idx = 0; idx < len; idx++) {
	 service_provider_name_str[idx] = byteAt(off + idx);
      }
      service_provider_name_str[idx] = '\0';
   }
   return service_provider_name_str;
}

uint8 B10_Desc_Service::service_name_length() const {
   int off = sizeofBufferBefore(pos_start_of_service_provider_name) + service_provider_name_length() + 1;
   return byteAt(off);
}

const char *B10_Desc_Service::service_name_string()  {
   if (service_name_str == NULL) {
      int len = service_name_length();
      service_name_str = new char[len + 1];
      int idx;
      int off = sizeofBufferBefore(pos_start_of_service_provider_name) + service_provider_name_length() + 1;
      for (idx = 0; idx < len; idx++) {
	 service_name_str[idx] = byteAt(off + idx);
      }
      service_name_str[idx] = '\0';
   }
   return service_name_str;
}


/*
 * B10_Desc_Component class
 */
FIELDWIDTH_PREAMBLE(B10_Desc_Component)
FIELDWIDTH_CONTENT(B10_Descriptor::pos_descriptor_tag,		8)
FIELDWIDTH_CONTENT(B10_Descriptor::pos_descriptor_length,	8)
FIELDWIDTH_CONTENT(pos_reserved,				4)
FIELDWIDTH_CONTENT(pos_stream_content,				4)
FIELDWIDTH_CONTENT(pos_component_type,				8)
FIELDWIDTH_CONTENT(pos_component_tag,				8)
FIELDWIDTH_CONTENT(pos_ISO_639_language_code,			24)
FIELDWIDTH_CONTENT(pos_start_of_text_char,			0)
FIELDWIDTH_POSTAMBLE(B10_Desc_Component, /**/)

const char *B10_Desc_Component::component_string() {
   static char buff[16]; // This is really thread-unsafe
   static Id2StringMap videoMap;
   static Id2StringMap audioMap;
   static Id2StringMap h264Map;
   static bool initialized = false;
   if (!initialized) {
      videoMap[0x01] = "Video 480i(525i), 4:3 aspect ratio";
      videoMap[0x02] = "Video 480i(525i), 16:9 aspect ratio, with pan vectors";
      videoMap[0x03] = "Video 480i(525i), 16:9 aspect ratio, without pan vectors";
      videoMap[0x04] = "Video 480i(525i), >16:9 aspect ratio";
      videoMap[0xA1] = "Video 480p(525p), 4:3 aspect ratio";
      videoMap[0xA2] = "Video 480p(525p), 16:9 aspect ratio, with pan vectors";
      videoMap[0xA3] = "Video 480p(525p), 16:9 aspect ratio, without pan vectors";
      videoMap[0xA4] = "Video 480p(525p), >16:9 aspect ratio";
      videoMap[0xB1] = "Video 1080i(1125i), 4:3 aspect ratio";
      videoMap[0xB2] = "Video 1080i(1125i), 16:9 aspect ratio, with pan vectors";
      videoMap[0xB3] = "Video 1080i(1125i), 16:9 aspect ratio, without pan vectors";
      videoMap[0xB4] = "Video 1080i(1125i), >16:9 aspect ratio";
      videoMap[0xC1] = "Video 720p(750p), 4:3 aspect ratio";
      videoMap[0xC2] = "Video 720p(750p), 16:9 aspect ratio, with pan vectors";
      videoMap[0xC3] = "Video 720p(750p), 16:9 aspect ratio, without pan vectors";
      videoMap[0xC4] = "Video 720p(750p), >16:9 aspect ratio";
      videoMap[0xD1] = "Video 240p, 4:3 aspect ratio ";
      videoMap[0xD2] = "Video 240p, 4:3 aspect ratio, with pan vectors";
      videoMap[0xD3] = "Video 240p, 4:3 aspect ratio, without pan vector";
      videoMap[0xD4] = "Video 240p, 4:3 aspect ratio > 16:9";
      videoMap[0xE1] = "Video 1080p(1125p), 4:3 aspect ratio";
      videoMap[0xE2] = "Video 1080p(1125p), 16:9 aspect ratio, with pan vectors";
      videoMap[0xE3] = "Video 1080p(1125p), 16:9 aspect ratio, without pan vectors";
      videoMap[0xE4] = "Video 1080p(1125p), >16:9 aspect ratio";

      audioMap[0x01] = "Audio, 1/0 mode (single mono)";
      audioMap[0x02] = "Audio, 1/0+1/0 mode (dual mono)";
      audioMap[0x03] = "Audio, 2/0 mode (stereo)";
      audioMap[0x04] = "Audio, 2/1 mode";
      audioMap[0x05] = "Audio, 3/0 mode";
      audioMap[0x06] = "Audio, 2/2 mode";
      audioMap[0x07] = "Audio, 3/1 mode";
      audioMap[0x08] = "Audio, 3/2 mode";
      audioMap[0x09] = "Audio, 3/2+LFE mode";
      audioMap[0x40] = "Audio description for the visually impaired";
      audioMap[0x41] = "Audio for the hard of hearing";
      initialized = true;
   }

   uint8 type = component_type();
   if (stream_content() == 1) {
      Id2StringMap::const_iterator itr = videoMap.find(type);
      if (itr != videoMap.end()) return itr->second;
   } else if (stream_content() == 2) {
      Id2StringMap::const_iterator itr = audioMap.find(type);
      if (itr != audioMap.end()) return itr->second;
   }
   sprintf(buff, "0x%02x-0x%02x", stream_content(), type);
   return buff;
}

uint8 B10_Desc_Component::stream_content() const {
   return bit_field8(pos_stream_content);
}

uint8 B10_Desc_Component::component_type() const {
   return bit_field8(pos_component_type);
}

uint8 B10_Desc_Component::component_tag() const {
   return bit_field8(pos_component_tag);
}


/*
 * B10_Desc_AudioComponent class
 */
FIELDWIDTH_PREAMBLE(B10_Desc_AudioComponent)
FIELDWIDTH_CONTENT(B10_Descriptor::pos_descriptor_tag,	8)
FIELDWIDTH_CONTENT(B10_Descriptor::pos_descriptor_length,	8)
FIELDWIDTH_CONTENT(pos_reserved1,			4)
FIELDWIDTH_CONTENT(pos_stream_content,			4)
FIELDWIDTH_CONTENT(pos_component_type,			8)
FIELDWIDTH_CONTENT(pos_component_tag,			8)
FIELDWIDTH_CONTENT(pos_stream_type,			8)
FIELDWIDTH_CONTENT(pos_simulcast_group,			8)
FIELDWIDTH_CONTENT(pos_ES_multi_lingual_tag,		1)
FIELDWIDTH_CONTENT(pos_main_component_flag,		1)
FIELDWIDTH_CONTENT(pos_quality_indicator,		2)
FIELDWIDTH_CONTENT(pos_sampling_rate,			3)
FIELDWIDTH_CONTENT(pos_reserved2,			1)
FIELDWIDTH_CONTENT(pos_ISO_639_language_code,		24)
FIELDWIDTH_CONTENT(pos_end_of_fixed_data,		0)
FIELDWIDTH_POSTAMBLE(B10_Desc_AudioComponent, /**/)

const char *B10_Desc_AudioComponent::component_string() {
   static char buff[512]; // This is really thread-unsafe
   static Id2StringMap componentTypeMap;
   static Id2StringMap streamTypeMap;
   static bool initialized = false;
   if (!initialized) {
      initialized = true;
      componentTypeMap[0x01] = "Audio, 1/0 mode (single mono)";
      componentTypeMap[0x02] = "Audio, 1/0+1/0 mode (dual mono)";
      componentTypeMap[0x03] = "Audio, 2/0 mode (stereo)";
      componentTypeMap[0x04] = "Audio, 2/1 mode";
      componentTypeMap[0x05] = "Audio, 3/0 mode";
      componentTypeMap[0x06] = "Audio, 2/2 mode";
      componentTypeMap[0x07] = "Audio, 3/1 mode";
      componentTypeMap[0x08] = "Audio, 3/2 mode";
      componentTypeMap[0x09] = "Audio, 3/2+LFE mode";
      componentTypeMap[0x40] = "Audio description for the visually impaired";
      componentTypeMap[0x41] = "Audio for the hard of hearing";

      streamTypeMap[0x00] = "ITU-T | ISO/IEC Reserved";
      streamTypeMap[0x01] = "ISO/IEC 11172 Video";
      streamTypeMap[0x02] = "ITU-T Rec. H.262 | ISO/IEC 13818-2 Video or ISO/IEC 11172-2 constrained parameter video stream";
      streamTypeMap[0x03] = "ISO/IEC 11172 Audio";
      streamTypeMap[0x04] = "ISO/IEC 13818-3 Audio";
      streamTypeMap[0x05] = "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 private_sections";
      streamTypeMap[0x06] = "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 PES packets containing private data";
      streamTypeMap[0x07] = "ISO/IEC 13522 MHEG";
      streamTypeMap[0x08] = "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Annex A DSM-CC";
      streamTypeMap[0x09] = "ITU-T Rec. H.222.1";
      streamTypeMap[0x0A] = "ISO/IEC 13818-6 type A";
      streamTypeMap[0x0B] = "ISO/IEC 13818-6 type B";
      streamTypeMap[0x0C] = "ISO/IEC 13818-6 type C";
      streamTypeMap[0x0D] = "ISO/IEC 13818-6 type D";
      streamTypeMap[0x0E] = "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 auxiliary";
      streamTypeMap[0x0F] = "ISO/IEC 13818-7 Audio with ADTS transport syntax";
      streamTypeMap[0x10] = "ISO/IEC 14496-2 Visual";
      streamTypeMap[0x11] = "ISO/IEC 14496-3 Audio with the LATM transport syntax as defined in ISO/IEC 14496-3 / AMD 1";
      streamTypeMap[0x12] = "ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in PES packets";
      streamTypeMap[0x13] = "      ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in ISO/IEC14496_sections.";
      streamTypeMap[0x14] = "ISO/IEC 13818-6 Synchronized Download Protocol";
   }

   if (stream_content() != 0x02) return "Audio, unknown";

   const char *ctype = NULL;
   const char *stype = NULL;
   Id2StringMap::const_iterator itr = componentTypeMap.find(component_type());
   if (itr != componentTypeMap.end()) ctype = itr->second;
   itr = streamTypeMap.find(stream_type());
   if (itr != streamTypeMap.end()) stype = itr->second;
   sprintf(buff, "[%s], [%s]", ctype, stype);
   return buff;
}

uint8 B10_Desc_AudioComponent::stream_content() const {
   return bit_field8(pos_stream_content);
}

uint8 B10_Desc_AudioComponent::component_type() const {
   return bit_field8(pos_component_type);
}

uint8 B10_Desc_AudioComponent::component_tag() const {
   return bit_field8(pos_component_tag);
}

uint8 B10_Desc_AudioComponent::stream_type() const {
   return bit_field8(pos_stream_type);
}


/*
 * B10_Desc_DataComponent class
 */
FIELDWIDTH_PREAMBLE(B10_Desc_DataComponent)
FIELDWIDTH_CONTENT(B10_Descriptor::pos_descriptor_tag,		8)
FIELDWIDTH_CONTENT(B10_Descriptor::pos_descriptor_length,	8)
FIELDWIDTH_CONTENT(pos_data_component_id,			16)
FIELDWIDTH_POSTAMBLE(B10_Desc_DataComponent, /**/)

uint16 B10_Desc_DataComponent::data_component_id() const {
   return bit_field16(pos_data_component_id);
}
