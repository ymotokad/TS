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
#define IMPLEMENTING_MPEGHEADERS
#include "ByteArrayBuffer.h"
#include "MPEGHeaders.h"


/*
 * MPEGHeader class
 */
void MPEGHeader::setBuffer(const ByteArray &data) {
   assert(mydata == NULL);
   mydata = new ByteArrayBuffer(data);
}
void MPEGHeader::initialize() {
   setStreamType(StreamType_MPEG);
}
FIELDWIDTH_PREAMBLE(MPEGHeader)
FIELDWIDTH_CONTENT(pos_start_code_prefix,			24)
FIELDWIDTH_CONTENT(pos_start_code,				8)
FIELDWIDTH_POSTAMBLE(MPEGHeader, initialize())

/*
 * MPEGPictureHeader class
 */
FIELDWIDTH_PREAMBLE(MPEGPictureHeader)
FIELDWIDTH_CONTENT(MPEGHeader::pos_start_code_prefix,	24)
FIELDWIDTH_CONTENT(MPEGHeader::pos_start_code,		8)
FIELDWIDTH_CONTENT(pos_temporal_sequence_number,	10)
FIELDWIDTH_CONTENT(pos_frame_type,			3)
FIELDWIDTH_CONTENT(pos_VBV_delay,			16)
FIELDWIDTH_POSTAMBLE(MPEGPictureHeader, /**/)

/*
 * MPEGGroupOfPictures class
 */
FIELDWIDTH_PREAMBLE(MPEGGroupOfPictures)
FIELDWIDTH_CONTENT(MPEGHeader::pos_start_code_prefix,	24)
FIELDWIDTH_CONTENT(MPEGHeader::pos_start_code,		8)
FIELDWIDTH_CONTENT(pos_drop_frameflag,			1)
FIELDWIDTH_CONTENT(pos_hour,				5)
FIELDWIDTH_CONTENT(pos_minute,				6)
FIELDWIDTH_CONTENT(pos_reserved,			1)
FIELDWIDTH_CONTENT(pos_second,				6)
FIELDWIDTH_CONTENT(pos_frame,				6)
FIELDWIDTH_CONTENT(pos_closed_GOP,			1)
FIELDWIDTH_CONTENT(pos_broken_GOP,			1)
FIELDWIDTH_POSTAMBLE(MPEGGroupOfPictures, /**/)
