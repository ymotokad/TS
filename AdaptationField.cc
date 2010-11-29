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

/*
 * constructors 
 */

AdaptationField::AdaptationField() {
}


/*
 * destructors 
 */

AdaptationField::~AdaptationField() {
}

/*
 * other methods
 */
int AdaptationField::load(uint8 *buffer) {
   // Header
   bytes = buffer;
   return field_length() + 1;
}

void AdaptationField::dump(std::ostream *osp) const {
   *osp << "  -- Adaptation Field";
   *osp << " len=" << std::dec << (int)field_length();
   *osp << std::endl;
   *osp << "    byte0: " << std::hex << std::showbase << (unsigned int)bytes[0] << std::endl;
   *osp << "    byte1: " << std::hex << std::showbase << (unsigned int)bytes[1] << std::endl;
   hexdump(osp, bytes, field_length() - 2, 4);
}
