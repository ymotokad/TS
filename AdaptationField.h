#ifndef ADAPTATIONFIELD_H
#define ADAPTATIONFIELD_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_ADAPTATIONFIELD
static const char *rcsid_Nbp = "@(#)$Id$";
#endif /* IMPLEMENTING_ADAPTATIONFIELD */
#include "PacketSection.h"

class AdaptationField : public PacketSection {
 public:
   AdaptationField();
   ~AdaptationField();
   int load(uint8 *buffer);
   void dump(std::ostream *outputstream) const;
 protected:
   int field_length() const;
   /*
   bool hasPCR() const;
   bool hasOPCR() const;
   bool isSplicingPoint() const;
   bool hasTransportPrivateData() const;
   bool hasAdaptationFieldExtension() const;
   */
   
   uint8 *bytes;
};

inline int AdaptationField::field_length() const {
   return (int)bytes[0];
};


#endif /* ADAPTATIONFIELD_H */
