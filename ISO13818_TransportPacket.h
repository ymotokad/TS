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

#ifndef ISO13818_TRANSPORTPACKET_H
#define ISO13818_TRANSPORTPACKET_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_ISO13818_TRANSPORTPACKET
static const char *rcsid_ISO13818_TransportPacket = "@(#)$Id$";
#endif /* IMPLEMENTING_ISO13818_TRANSPORTPACKET */
#include <iostream>
#include "TSTypes.h"
#include "ByteArrayBuffer.h"
#include "BitStream.h"

class ISO13818_AdaptationField;

/*
 * ISO13818_TransportPacket
 */
class ISO13818_TransportPacket : public BitStream {
 public:
   ISO13818_TransportPacket();
   ISO13818_TransportPacket(ByteArray *rawdata);
   virtual ~ISO13818_TransportPacket();
   int write(std::ostream *osp) const;
   const ByteArray *getRawdata() const;
   const ISO13818_AdaptationField *getAdaptationField() const;
   const ByteArray *getPayload() const;

   uint8 sync_byte() const;
   bool transport_error_indicator() const;
   bool payload_unit_start_indicator() const;
   bool transport_priority() const;
   uint16 PID() const;
   uint8 transport_scrambling_control() const;
   uint8 adaptation_field_control() const;
   bool has_adaptation_field() const;
   bool has_payload() const;
   uint8 continuity_counter() const;
 protected:
   void initobj();
   void initvars();
   const ISO13818_AdaptationField *adaptationField;
   const ByteArray *payload;
};


#define SIZEOF_PACKET				188
#define SYNC_BYTE_VALUE				0x47

/*
 * Order of data
 */
#define ISO13818_TransportPacket_sync_byte			0
#define ISO13818_TransportPacket_transport_error_indicator	1
#define ISO13818_TransportPacket_payload_unit_start_indicator	2
#define ISO13818_TransportPacket_transport_priority		3
#define ISO13818_TransportPacket_PID				4
#define ISO13818_TransportPacket_transport_scrambling_control	5
#define ISO13818_TransportPacket_has_adaptation_field		6
#define ISO13818_TransportPacket_has_payload			7
#define ISO13818_TransportPacket_continuity_counter		8
#define ISO13818_TransportPacket_StartOfData			9

inline const ByteArray *ISO13818_TransportPacket::getPayload() const {
   return payload;
}

inline const ISO13818_AdaptationField *ISO13818_TransportPacket::getAdaptationField() const {
   return adaptationField;
}

inline uint8 ISO13818_TransportPacket::sync_byte() const {
   return bit_field8(ISO13818_TransportPacket_sync_byte);
}

inline bool ISO13818_TransportPacket::transport_error_indicator() const {
   return bit_field1(ISO13818_TransportPacket_transport_error_indicator);
}

inline bool ISO13818_TransportPacket::payload_unit_start_indicator() const {
   return bit_field1(ISO13818_TransportPacket_payload_unit_start_indicator);
}

inline bool ISO13818_TransportPacket::transport_priority() const {
   return bit_field1(ISO13818_TransportPacket_transport_priority);
}

inline uint16 ISO13818_TransportPacket::PID() const {
   return bit_field16(ISO13818_TransportPacket_PID);
}

inline uint8 ISO13818_TransportPacket::transport_scrambling_control() const {
   return bit_field8(ISO13818_TransportPacket_transport_scrambling_control);
}

inline bool ISO13818_TransportPacket::has_adaptation_field() const {
   return bit_field1(ISO13818_TransportPacket_has_adaptation_field);
}

inline bool ISO13818_TransportPacket::has_payload() const {
   return bit_field1(ISO13818_TransportPacket_has_payload);
}

inline uint8 ISO13818_TransportPacket::continuity_counter() const {
   return bit_field8(ISO13818_TransportPacket_continuity_counter);
}


#endif /* ISO13818_TRANSPORTPACKET_H */
