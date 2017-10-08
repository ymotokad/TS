#ifndef B10_DESCRIPTORS_H
#define B10_DESCRIPTORS_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_B10_DESCRIPTORS
static const char *rcsid_B10_Descriptor = "@(#)$Id$";
#endif /* IMPLEMENTING_B10_DESCRIPTORS */
#include <map>
#include "TSTypes.h"
#include "BitStream.h"

typedef std::map<uint8, const char*, std::less<uint8> > Id2StringMap;

class B10_Descriptor {
 public:
   static const char *getName(uint8 tag);

   // Order of bit stream data
   static const int pos_descriptor_tag		= 0;
   static const int pos_descriptor_length	= 1;

   // Descriptor tag
   static const int tag_video_stream		= 2;
   static const int tag_audio_stream		= 3;
   static const int tag_conditional_access	= 9;
   static const int tag_iso639_language		= 0x0a;
   static const int tag_service			= 0x48;
   static const int tag_short_event		= 0x4d;
   static const int tag_component		= 0x50;
   static const int tag_stream_identifier	= 0x52;
   static const int tag_content			= 0x54;
   static const int tag_audio_component		= 0xc4;
   static const int tag_video_decode_control	= 0xc8;
   static const int tag_data_component		= 0xfd;
 private:
   static Id2StringMap descriptors;
   static void initDescriptors();
};



/*
 * B10_Desc_VideoDecodeControl
 */
class B10_Desc_VideoDecodeControl : public BitStream {
 public:
   B10_Desc_VideoDecodeControl();
   uint8 video_encode_format() const;
   const char *video_encode_format_string() const;

   // Video encode format
   static const int video_1080p		= 0;
   static const int video_1080i		= 1;
   static const int video_720p		= 2;
   static const int video_480p		= 3;
   static const int video_480i		= 4;
   static const int video_240p		= 5;
   static const int video_120p		= 6;
   static const int video_2160p		= 7;

 protected:
   // Order of bit stream data
   static const int pos_still_picture_flag	= 2;
   static const int pos_sequence_end_code_flag	= 3;
   static const int pos_video_encode_format	= 4;
   static const int pos_reserved_future_use	= 5;
   static const int pos_end_of_data		= 6;
      
   void initobj();
};


/*
 * B10_Desc_StreamIdentifier
 */
class B10_Desc_StreamIdentifier : public BitStream {
 public:
   B10_Desc_StreamIdentifier();
   uint8 component_tag() const;
   const char *component_tag_string() const;

 protected:
   // Order of bit stream data
   static const int pos_component_tag		= 2;
   static const int pos_end_of_data		= 3;
      
   void initobj();
};


/*
 * B10_Desc_Service
 */
class B10_Desc_Service : public BitStream {
 public:
   B10_Desc_Service();
   virtual ~B10_Desc_Service();
   uint8 service_type() const;
   const char *service_type_string() const;
   uint8 service_provider_name_length() const;
   const char *service_provider_name_string();
   uint8 service_name_length() const;
   const char *service_name_string();

 protected:
   // Order of bit stream data
   static const int pos_service_type			= 2;
   static const int pos_service_provider_name_length	= 3;
   static const int pos_start_of_service_provider_name	= 4;
      
   void initobj();
   void initvars();
   char *service_name_str;
   char *service_provider_name_str;
};


/*
 * B10_Desc_Component
 */
class B10_Desc_Component : public BitStream {
 public:
   B10_Desc_Component();
   const char *component_string();
   uint8 stream_content() const;
   uint8 component_type() const;
   uint8 component_tag() const;

 protected:
   // Order of bit stream data
   static const int pos_reserved		= 2;
   static const int pos_stream_content		= 3;
   static const int pos_component_type		= 4;
   static const int pos_component_tag		= 5;
   static const int pos_ISO_639_language_code	= 6;
   static const int pos_start_of_text_char	= 7;

   void initobj();
};


/*
 * B10_Desc_AudioComponent
 */
class B10_Desc_AudioComponent : public BitStream {
 public:
   B10_Desc_AudioComponent();
   const char *component_string();
   uint8 stream_content() const;
   uint8 component_type() const;
   uint8 component_tag() const;
   uint8 stream_type() const;

 protected:
   // Order of bit stream data
   static const int pos_reserved1		= 2;
   static const int pos_stream_content		= 3;
   static const int pos_component_type		= 4;
   static const int pos_component_tag		= 5;
   static const int pos_stream_type		= 6;
   static const int pos_simulcast_group		= 7;
   static const int pos_ES_multi_lingual_tag	= 8;
   static const int pos_main_component_flag	= 9;
   static const int pos_quality_indicator	= 10;
   static const int pos_sampling_rate		= 11;
   static const int pos_reserved2		= 12;
   static const int pos_ISO_639_language_code	= 13;
   static const int pos_end_of_fixed_data	= 14;

   void initobj();
};


/*
 * B10_Desc_DataComponent
 */
class B10_Desc_DataComponent : public BitStream {
 public:
   B10_Desc_DataComponent();
   uint16 data_component_id() const;

   static const uint16 id_B24_caption		= 0x0008;
   
 protected:
   // Order of bit stream data
   static const int pos_data_component_id	= 2;

   void initobj();
};


#endif /* B10_DESCRIPTORS_H */
