#ifndef ADAPTATIONFIELD_H
#define ADAPTATIONFIELD_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_ADAPTATIONFIELD
static const char *rcsid_Nbp = "@(#)$Id$";
#endif /* IMPLEMENTING_ADAPTATIONFIELD */
#include "BitStream.h"

class AdaptationField;

class AdaptationField : public BitStream {
 public:
   AdaptationField();
   void dump(std::ostream *osp) const;
   int adaptation_field_length() const;
 protected:
   void initobj();
   /*
   bool hasPCR() const;
   bool hasOPCR() const;
   bool isSplicingPoint() const;
   bool hasTransportPrivateData() const;
   bool hasAdaptationFieldExtension() const;
   */
};


/*
 * Order of data
 */
#define AdaptationField_adaptation_field_length			0
#define AdaptationField_discontinuity_indicator			1
#define AdaptationField_random_access_indicator			2
#define AdaptationField_elementary_stream_priority_indicator	3
#define AdaptationField_PCR_flag				4
#define AdaptationField_OPCR_flag				5
#define AdaptationField_splicing_point_flag			6
#define AdaptationField_transport_private_data_flag		7
#define AdaptationField_adaptation_field_extension_flag		8
#define AdaptationField_stuffing_bytes				9


inline int AdaptationField::adaptation_field_length() const {
   return (int)bit_field8(AdaptationField_adaptation_field_length);
};


#endif /* ADAPTATIONFIELD_H */
