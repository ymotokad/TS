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

FIELDWIDTH_PREAMBLE(AdaptationField)
FIELDWIDTH_CONTENT(TransportPacket_sync_byte,				8)
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
FIELDWIDTH_POSTAMBLE(AdaptationField, /**/)

/*
 * constructors 
 */

/*
 * other methods
 */

void AdaptationField::dump(std::ostream *osp) const {
   *osp << "  -- Adaptation Field";
   *osp << " len=" << std::dec << (int)adaptation_field_length();
   *osp << std::endl;
   hexdump(4, osp);
}
