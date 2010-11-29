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
#include <iostream>
#define IMPLEMENTING_PROGRAMASSOCIATIONSECTION
#include "ProgramAssociationSection.h"

/*
 * constructors 
 */


/*
 * destructors 
 */

/*
 * other methods
 */
int AdaptationField::load(uint8 *buffer) {
   return 0;
}

void AdaptationField::dump(std::ostream *osp) const {
}
