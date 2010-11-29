#ifndef TRANSPORTPACKET_H
#define TRANSPORTPACKET_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_TRANSPORTPACKET
static const char *rcsid_Nbp = "@(#)$Id$";
#endif /* IMPLEMENTING_TRANSPORTPACKET */
#include <fstream>
#include <iostream>
#include "PacketSection.h"
#include "AdaptationField.h"
#include "ProgramAssociationSection.h"

class TransportPacket {
 public:
   TransportPacket();
   ~TransportPacket();
   int load(std::istream *inputstream);
   void dump(std::ostream *outputstream) const;
   void dump_hex(std::ostream *outputstream) const;
   uint16 PID() const;
 protected:
   static const int sizeOfHeader = 4;
   uint8 sync_byte() const;
   bool transport_error_indicator() const;
   bool payload_unit_start_indicator() const;
   bool transport_priority() const;
   uint8 transport_scrambling_control() const;
   uint8 adaptation_field_control() const;
   bool hasAdaptationField() const;
   bool hasPayload() const;
   uint8 continuity_counter() const;

   uint8 bytes[512];
   AdaptationField *adaptationField;
   int sizePayload;
   uint8 *payload;
};

#define SYNC_BYTE_VALUE 0x47

#define PID_ProgramAssociationTable		0x0000
#define PID_ConditionalAccessTable		0x0001
#define PID_TransportStreamDescriptionTable	0x0002
#define PID_NullPacket				0x1fff

inline uint8 TransportPacket::sync_byte() const {
   return bytes[0];
};
inline bool TransportPacket::transport_error_indicator() const {
   return (bytes[1] & 0x80) == 0x80;
};
inline bool TransportPacket::payload_unit_start_indicator() const {
   return (bytes[1] & 0x40) == 0x40;
};
inline bool TransportPacket::transport_priority() const {
   return (bytes[1] & 0x20) == 0x20;
};
inline uint16 TransportPacket::PID() const {
   return (((bytes[1] & 0x1f) << 8) | bytes[2]);
};
inline uint8 TransportPacket::transport_scrambling_control() const {
   return (bytes[3] & 0xc0) >> 6;
};
inline uint8 TransportPacket::adaptation_field_control() const {
   return (bytes[3] & 0x30) >> 4;
};
inline bool TransportPacket::hasAdaptationField() const {
   return (bytes[3] & 0x20) == 0x20;
};
inline bool TransportPacket::hasPayload() const {
   return (bytes[3] & 0x10) == 0x10;
};
inline uint8 TransportPacket::continuity_counter() const {
   return (bytes[3] & 0x0f);
};



   
   

#endif /* TRANSPORTPACKET_H */
