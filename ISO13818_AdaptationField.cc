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
#define IMPLEMENTING_ISO13818_ADAPTATIONFIELD
#include "ISO13818_AdaptationField.h"
#include "ISO13818_SystemClock.h"


class PCR : public BitStream {
 public:
   PCR();
   uint64 base() const;
   uint16 ext() const;
   int length() const;
 protected:
   void initobj();
};
#define PCR_program_clock_reference_base			0
#define PCR_reserved1						1
#define PCR_program_clock_reference_extension			2
#define PCR_END_OF_DATA						3

FIELDWIDTH_PREAMBLE(PCR)
FIELDWIDTH_CONTENT(PCR_program_clock_reference_base,		33)
FIELDWIDTH_CONTENT(PCR_reserved1,				6)
FIELDWIDTH_CONTENT(PCR_program_clock_reference_extension,	9)
FIELDWIDTH_CONTENT(PCR_END_OF_DATA,				0)
FIELDWIDTH_POSTAMBLE(PCR, /**/)

uint64 PCR::base() const {
   return bit_field64(PCR_program_clock_reference_base);
}
uint16 PCR::ext() const {
   return bit_field16(PCR_program_clock_reference_extension);
}
int PCR::length() const {
   return sizeofBufferBefore(PCR_END_OF_DATA);
}


/***
 * ISO13818_AdaptationField Class
 */

FIELDWIDTH_PREAMBLE(ISO13818_AdaptationField)
FIELDWIDTH_CONTENT(ISO13818_AdaptationField_adaptation_field_length,		8)
FIELDWIDTH_CONTENT(ISO13818_AdaptationField_discontinuity_indicator,		1)
FIELDWIDTH_CONTENT(ISO13818_AdaptationField_random_access_indicator,		1)
FIELDWIDTH_CONTENT(ISO13818_AdaptationField_elementary_stream_priority_indicator,1)
FIELDWIDTH_CONTENT(ISO13818_AdaptationField_PCR_flag,				1)
FIELDWIDTH_CONTENT(ISO13818_AdaptationField_OPCR_flag,				1)
FIELDWIDTH_CONTENT(ISO13818_AdaptationField_splicing_point_flag,		1)
FIELDWIDTH_CONTENT(ISO13818_AdaptationField_transport_private_data_flag,	1)
FIELDWIDTH_CONTENT(ISO13818_AdaptationField_adaptation_field_extension_flag,	1)
FIELDWIDTH_CONTENT(ISO13818_AdaptationField_stuffing_bytes,			0)
FIELDWIDTH_POSTAMBLE(ISO13818_AdaptationField, initvars())

/*
 * constructors 
 */

void ISO13818_AdaptationField::initvars() {
   PCR_available = false;
   loaded = false;
}
   

/*
 * other methods
 */

void ISO13818_AdaptationField::load() {
   int buflen = bufferLength();
   int n = (int)adaptation_field_length();
   if (adaptation_field_length() <= sizeofBufferBefore(ISO13818_AdaptationField_adaptation_field_extension_flag + 1)) {
      loaded = false;
      return;
   }
   if (hasPCR()) {
      PCR pcr;
      int buflen = bufferLength();
      int asize = pcr.length();
      int off = sizeofBufferBefore(ISO13818_AdaptationField_adaptation_field_extension_flag + 1);
      if (buflen >= off + asize) {
	 //assert(buflen >= off + asize);
	 pcr.setBuffer(*this, off);
	 base = pcr.base();
	 ext = pcr.ext();
	 PCR_available = true;
      }
   }
   loaded = true;
}

void ISO13818_AdaptationField::dump(std::ostream *osp) const {
   *osp << "  -- Adaptation Field";
   *osp << " len=" << std::dec << (int)adaptation_field_length();
   *osp << std::endl;

   if (hasLoaded() && hasCompletePCR()) {
      char buf[20];
      ProgramClock pcr(getBase(), getExt());
      printf("     PCR: %s, 0x%09llx.0x%04x\n", pcr.toString(buf), getBase(), getExt());
   }
   
   hexdump(4, osp);
}

