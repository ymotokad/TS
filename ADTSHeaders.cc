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
#include <map>
#define IMPLEMENTING_ADTSHEADERS
#include "ByteArrayBuffer.h"
#include "ADTSHeaders.h"


/*
 * ADTSHeader class
 */
void ADTSHeader::setBuffer(const ByteArray &data) {
   assert(mydata == NULL);
   mydata = new ByteArrayBuffer(data);
}
void ADTSHeader::initialize() {
   setStreamType(StreamType_ADTS);
}
FIELDWIDTH_PREAMBLE(ADTSHeader)
FIELDWIDTH_CONTENT(pos_sync_word,				12)
FIELDWIDTH_CONTENT(pos_id,					1)
FIELDWIDTH_CONTENT(pos_layer,					2)
FIELDWIDTH_CONTENT(pos_protection_absent,			1)
FIELDWIDTH_CONTENT(pos_profile,					2)
FIELDWIDTH_CONTENT(pos_sampling_frequency_index,		4)
FIELDWIDTH_CONTENT(pos_private_bit,				1)
FIELDWIDTH_CONTENT(pos_channel_configuration,			3)
FIELDWIDTH_CONTENT(pos_original_copy,				1)
FIELDWIDTH_CONTENT(pos_home,					1)
FIELDWIDTH_CONTENT(pos_copyright_id_bit,			1)
FIELDWIDTH_CONTENT(pos_copyright_id_start,			1)
FIELDWIDTH_CONTENT(pos_aac_frame_length,			13)
FIELDWIDTH_CONTENT(pos_adts_buffer_fullness,			11)
FIELDWIDTH_POSTAMBLE(ADTSHeader, initialize())
