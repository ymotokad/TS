#ifndef PSI_H
#define PSI_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_PSI
static const char *rcsid_PSI = "@(#)$Id$";
#endif /* IMPLEMENTING_PSI */
#include "BitStream.h"

class PSI : public BitStream {
 public:
   PSI(uint8 continuous_counter = 0);
   virtual bool isComplete() const = 0;
   int append(uint8 continuous_counter, const ByteArray &src);
 protected:
   uint8 last_cc;
};

inline PSI::PSI(uint8 continuous_counter) : last_cc(continuous_counter) {
}


#endif /* PSI_H */
