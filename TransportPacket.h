#ifndef TRANSPORTPACKET_H
#define TRANSPORTPACKET_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_TRANSPORTPACKET
static const char *rcsid_TransportPacket = "@(#)$Id$";
#endif /* IMPLEMENTING_TRANSPORTPACKET */
#include <iostream>
#include "TSTypes.h"
#include "ByteArrayBuffer.h"
#include "BitStream.h"

class AdaptationField;

class TransportPacket : public BitStream {
 public:
   TransportPacket();
   TransportPacket(std::istream *isp);
   virtual ~TransportPacket();
   const AdaptationField getAdaptationField() const;
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
   void initBitDistance();
   const AdaptationField *adaptationField;
   const ByteArray *payload;
};


#define SIZEOF_PACKET				188
#define SYNC_BYTE_VALUE				0x47

/*
 * Order of data
 */
#define TransportPacket_sync_byte			0
#define TransportPacket_transport_error_indicator	1
#define TransportPacket_payload_unit_start_indicator	2
#define TransportPacket_transport_priority		3
#define TransportPacket_PID				4
#define TransportPacket_transport_scrambling_control	5
#define TransportPacket_has_adaptation_field		6
#define TransportPacket_has_payload			7
#define TransportPacket_continuity_counter		8
#define TransportPacket_StartOfData			9

inline const ByteArray *TransportPacket::getPayload() const {
   return payload;
}

inline bool TransportPacket::transport_error_indicator() const {
   return bit_field1(TransportPacket_transport_error_indicator);
}

inline bool TransportPacket::payload_unit_start_indicator() const {
   return bit_field1(TransportPacket_payload_unit_start_indicator);
}

inline bool TransportPacket::transport_priority() const {
   return bit_field1(TransportPacket_transport_priority);
}

inline uint16 TransportPacket::PID() const {
   return bit_field16(TransportPacket_PID);
}

inline uint8 TransportPacket::transport_scrambling_control() const {
   return bit_field8(TransportPacket_transport_scrambling_control);
}

inline bool TransportPacket::has_adaptation_field() const {
   return bit_field1(TransportPacket_has_adaptation_field);
}

inline bool TransportPacket::has_payload() const {
   return bit_field1(TransportPacket_has_payload);
}

inline uint8 TransportPacket::continuity_counter() const {
   return bit_field8(TransportPacket_continuity_counter);
}


#endif /* TRANSPORTPACKET_H */
