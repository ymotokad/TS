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
#define IMPLEMENTING_PSI
#include "PSI.h"

inline uint8 incr_cc(uint8 cc) {
   return (cc + 1) & 0xf;
}


/*
 * other methods
 */
int PSI::append(uint8 continuous_counter, const ByteArray &data) {
   if (incr_cc(last_cc) != continuous_counter) return -1;
   last_cc = continuous_counter;
   return BitStream::append(data);
}
