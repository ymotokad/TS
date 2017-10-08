/*
  This file is part of TS software suite.

  TS software suite is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  TS software suite is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with TS software suite.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef ISO13818_ADAPTATIONFIELD_H
#define ISO13818_ADAPTATIONFIELD_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_ISO13818_ADAPTATIONFIELD
static const char *rcsid_Nbp = "@(#)$Id$";
#endif /* IMPLEMENTING_ISO13818_ADAPTATIONFIELD */
#include "BitStream.h"

class ISO13818_AdaptationField : public BitStream {
 public:
   ISO13818_AdaptationField();
   void load();
   void dump(std::ostream *osp) const;
   int adaptation_field_length() const;
   bool hasLoaded() const;
   bool hasCompletePCR() const;
   uint64 getBase() const;
   uint16 getExt() const;
 protected:
   void initobj();
   void initvars();
   bool hasPCR() const;
   bool loaded;
   bool PCR_available;
   uint64 base;
   uint16 ext;
   /*
   bool hasOPCR() const;
   bool isSplicingPoint() const;
   bool hasTransportPrivateData() const;
   bool hasISO13818_AdaptationFieldExtension() const;
   */
};


/*
 * Order of data
 */
#define ISO13818_AdaptationField_adaptation_field_length			0
#define ISO13818_AdaptationField_discontinuity_indicator			1
#define ISO13818_AdaptationField_random_access_indicator			2
#define ISO13818_AdaptationField_elementary_stream_priority_indicator		3
#define ISO13818_AdaptationField_PCR_flag					4
#define ISO13818_AdaptationField_OPCR_flag					5
#define ISO13818_AdaptationField_splicing_point_flag				6
#define ISO13818_AdaptationField_transport_private_data_flag			7
#define ISO13818_AdaptationField_adaptation_field_extension_flag		8
#define ISO13818_AdaptationField_stuffing_bytes				9


inline int ISO13818_AdaptationField::adaptation_field_length() const {
   return (int)bit_field8(ISO13818_AdaptationField_adaptation_field_length);
};

inline bool ISO13818_AdaptationField::hasPCR() const {
   return bit_field1(ISO13818_AdaptationField_PCR_flag);
};

inline bool ISO13818_AdaptationField::hasLoaded() const {
   return loaded;
};

inline bool ISO13818_AdaptationField::hasCompletePCR() const {
   return PCR_available;
};

inline uint64 ISO13818_AdaptationField::getBase() const {
   assert(hasCompletePCR());
   return base;
};

inline uint16 ISO13818_AdaptationField::getExt() const {
   assert(hasCompletePCR());
   return ext;
};


#endif /* ISO13818_ADAPTATIONFIELD_H */
