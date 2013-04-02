/*
 * AdaptationField - 
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
#define IMPLEMENTING_ADAPTATIONFIELD
#include "AdaptationField.h"
#include "SystemClock.h"


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
   return bit_field64(PCR_program_clock_reference_extension);
}
int PCR::length() const {
   return sizeofBufferBefore(PCR_END_OF_DATA);
}


/***
 * AdaptationField Class
 */

FIELDWIDTH_PREAMBLE(AdaptationField)
FIELDWIDTH_CONTENT(AdaptationField_adaptation_field_length,		8)
FIELDWIDTH_CONTENT(AdaptationField_discontinuity_indicator,		1)
FIELDWIDTH_CONTENT(AdaptationField_random_access_indicator,		1)
FIELDWIDTH_CONTENT(AdaptationField_elementary_stream_priority_indicator,1)
FIELDWIDTH_CONTENT(AdaptationField_PCR_flag,				1)
FIELDWIDTH_CONTENT(AdaptationField_OPCR_flag,				1)
FIELDWIDTH_CONTENT(AdaptationField_splicing_point_flag,			1)
FIELDWIDTH_CONTENT(AdaptationField_transport_private_data_flag,		1)
FIELDWIDTH_CONTENT(AdaptationField_adaptation_field_extension_flag,	1)
FIELDWIDTH_CONTENT(AdaptationField_stuffing_bytes,			0)
FIELDWIDTH_POSTAMBLE(AdaptationField, initvars())

/*
 * constructors 
 */

void AdaptationField::initvars() {
   PCR_available = false;
   loaded = false;
}
   

/*
 * other methods
 */

void AdaptationField::load() {
   int buflen = bufferLength();
   int n = (int)adaptation_field_length();
   if (adaptation_field_length() <= sizeofBufferBefore(AdaptationField_adaptation_field_extension_flag + 1)) {
      loaded = false;
      return;
   }
   if (hasPCR()) {
      PCR pcr;
      int buflen = bufferLength();
      int asize = pcr.length();
      int off = sizeofBufferBefore(AdaptationField_adaptation_field_extension_flag + 1);
      if (buflen >= off + asize) {
	 pcr.setBuffer(*this, off);
	 PCR_available = true;
	 base = pcr.base();
	 ext = pcr.ext();
      }
   }
   loaded = true;
}

void AdaptationField::dump(std::ostream *osp) const {
   *osp << "  -- Adaptation Field";
   *osp << " len=" << std::dec << (int)adaptation_field_length();
   *osp << std::endl;

   if (hasLoaded() && hasCompletePCR()) {
      char buf[20];
      printf("     PCR: %s\n", SystemClock_toString(buf, getBase()));
   }
   
   hexdump(4, osp);
}

