/*
 * 
 * 
 * @(#)$Id$
 */
#ifndef	ADTSHEADERS_H
#define	ADTSHEADERS_H
#ifdef IMPLEMENTING_ADTSHEADERS
static const char ADTSHeaders_rcsid[] = "@(#)$Id$";
#endif
#include "TSTypes.h"
#include "ElementaryStream.h"

/*
 * ADTSHeader class
 */
class ADTSHeader : public ElementaryStream {
public:
   ADTSHeader();
   void setBuffer(const ByteArray &data);

   uint16 sync_word() const;
   /*
   uint8 id() const;
   uint8 layer() const();
   bool protection_absent() const;
   uint8 profile() const;
   uint8 sampling_frequency_index() const;
   bool private_bit() const;
   uint8 channel_configuration() const;
   uint8 original_copy() const;
   uint8 home() const;
   bool copyright_id_bit() const;
   bool copyright_id_start() const;
   uint16 aac_frame_length() const;
   uint16 adts_buffer_fullness() const;
   */

protected:
   // Order of bit stream data
   static const int pos_sync_word			= 0;
   static const int pos_id				= 1;
   static const int pos_layer				= 2;
   static const int pos_protection_absent		= 3;
   static const int pos_profile				= 4;
   static const int pos_sampling_frequency_index	= 5;
   static const int pos_private_bit			= 6;
   static const int pos_channel_configuration		= 7;
   static const int pos_original_copy			= 8;
   static const int pos_home				= 9;
   static const int pos_copyright_id_bit		= 10;
   static const int pos_copyright_id_start		= 11;
   static const int pos_aac_frame_length		= 12;
   static const int pos_adts_buffer_fullness		= 13;
   
 protected:
   void initobj();
   void initialize();
};

inline uint16 ADTSHeader::sync_word() const {
   return bit_field16(pos_sync_word);
}


#endif	/* ADTSHEADERS_H */
