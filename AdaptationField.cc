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
#include <iostream>
#define IMPLEMENTING_ADAPTATIONFIELD
#include "AdaptationField.h"

static AdaptationField nullobj;
static const int the_field_width[] = {
   8,	// AdaptationField_adaptation_field_length		0
   1,	// AdaptationField_discontinuity_indicator		1
   1,	// AdaptationField_random_access_indicator		2
   1,	// AdaptationField_elementary_stream_priority_indicator	3
   1,	// AdaptationField_PCR_flag				4
   1,	// AdaptationField_OPCR_flag				5
   1,	// AdaptationField_splicing_point_flag			6
   1,	// AdaptationField_transport_private_data_flag		7
   1,	// AdaptationField_adaptation_field_extension_flag	8
   0	// AdaptationField_stuffing_bytes			9
};

static int field_width(int position) {
   int n = the_field_width[position];
   return (n + sizeof(uint8) - 1) / sizeof(uint8);
}

/*
 * constructors 
 */
AdaptationField::AdaptationField() {
   static int *the_bit_distance = NULL;

   if (this == &nullobj) {
      int len = sizeof(the_field_width) / sizeof(the_field_width[0]);
      the_bit_distance = new int[len + 1];
      initializeBitDistance(the_field_width, len, the_bit_distance);
   }
   assert(the_bit_distance != NULL);
   setBitDistance(the_bit_distance);
}


/*
 * other methods
 */

void AdaptationField::dump(std::ostream *osp) const {
   *osp << "  -- Adaptation Field";
   *osp << " len=" << std::dec << (int)adaptation_field_length();
   *osp << std::endl;
   hexdump(4, osp);
}
