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
#define IMPLEMENTING_ISO13818_MPEGHEADERS
#include "ByteArrayBuffer.h"
#include "ISO13818_MPEGHeaders.h"


/*
 * ISO13818_MPEGHeader class
 */
void ISO13818_MPEGHeader::setBuffer(const ByteArray &data) {
   assert(mydata == NULL);
   mydata = new ByteArrayBuffer(data);
}
void ISO13818_MPEGHeader::initialize() {
   setStreamType(StreamType_MPEG);
}
FIELDWIDTH_PREAMBLE(ISO13818_MPEGHeader)
FIELDWIDTH_CONTENT(pos_start_code_prefix,			24)
FIELDWIDTH_CONTENT(pos_start_code,				8)
FIELDWIDTH_POSTAMBLE(ISO13818_MPEGHeader, initialize())

/*
 * MPEGPictureHeader class
 */
FIELDWIDTH_PREAMBLE(MPEGPictureHeader)
FIELDWIDTH_CONTENT(ISO13818_MPEGHeader::pos_start_code_prefix,	24)
FIELDWIDTH_CONTENT(ISO13818_MPEGHeader::pos_start_code,		8)
FIELDWIDTH_CONTENT(pos_temporal_sequence_number,	10)
FIELDWIDTH_CONTENT(pos_frame_type,			3)
FIELDWIDTH_CONTENT(pos_VBV_delay,			16)
FIELDWIDTH_POSTAMBLE(MPEGPictureHeader, /**/)

/*
 * MPEGGroupOfPictures class
 */
FIELDWIDTH_PREAMBLE(MPEGGroupOfPictures)
FIELDWIDTH_CONTENT(ISO13818_MPEGHeader::pos_start_code_prefix,	24)
FIELDWIDTH_CONTENT(ISO13818_MPEGHeader::pos_start_code,		8)
FIELDWIDTH_CONTENT(pos_drop_frameflag,			1)
FIELDWIDTH_CONTENT(pos_hour,				5)
FIELDWIDTH_CONTENT(pos_minute,				6)
FIELDWIDTH_CONTENT(pos_reserved,			1)
FIELDWIDTH_CONTENT(pos_second,				6)
FIELDWIDTH_CONTENT(pos_frame,				6)
FIELDWIDTH_CONTENT(pos_closed_GOP,			1)
FIELDWIDTH_CONTENT(pos_broken_GOP,			1)
FIELDWIDTH_POSTAMBLE(MPEGGroupOfPictures, /**/)
