#ifndef PROGRAMASSOCIATIONSECTION_H
#define PROGRAMASSOCIATIONSECTION_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_PROGRAMASSOCIATIONSECTION
static const char *rcsid_Nbp = "@(#)$Id$";
#endif /* IMPLEMENTING_PROGRAMASSOCIATIONSECTION */
#include "PacketSection.h"

class PrpgramAssociationSection {
 public:
   int load(uint8 *buffer);
   void dump(std::ostream *outputstream) const;
 protected:
   int section_length() const;
   
   uint8 *bytes;
};
   

#endif /* PROGRAMASSOCIATIONSECTION_H */
