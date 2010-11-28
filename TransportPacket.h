#ifndef TRANSPORTPACKET_H
#define TRANSPORTPACKET_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_TRANSPORTPACKET
static const char *rcsid_Nbp = "@(#)$Id";
#endif /* IMPLEMENTING_TRANSPORTPACKET */
#include <fstream>
#include <iostream>

//#define NODEBUG

typedef unsigned char uint8;	// 8bit unsigned integer
typedef unsigned short uint16;	// 16bit unsigned integer

class AdaptationField {
 public:
   AdaptationField();
   ~AdaptationField();
   int load(std::istream *inputstream);
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
   
   uint8 byte0;
   uint8 byte1;
   uint8 *buffer;
};

inline int AdaptationField::field_length() const {
   return (int)byte0;
};


class TransportPacket {
 public:
   TransportPacket();
   ~TransportPacket();
   int load(std::istream *inputstream);
   void dump(std::ostream *outputstream) const;
 protected:
   uint8 sync_byte() const;
   bool transport_error_indicator() const;
   bool payload_unit_start_indicator() const;
   bool transport_priority() const;
   uint16 PID() const;
   uint8 transport_scrambling_control() const;
   uint8 adaptation_field_control() const;
   bool hasAdaptationField() const;
   bool hasPayload() const;
   uint8 continuity_counter() const;

   uint8 byte0;
   uint8 byte1;
   uint8 byte2;
   uint8 byte3;
   AdaptationField *adaptationField;
   int sizePayload;
   uint8 *payload;
};

#define SYNC_BYTE_VALUE 0x47
inline uint8 TransportPacket::sync_byte() const {
   return byte0;
};
inline bool TransportPacket::transport_error_indicator() const {
   return (byte1 & 0x80) == 0x80;
};
inline bool TransportPacket::payload_unit_start_indicator() const {
   return (byte1 & 0x40) == 0x40;
};
inline bool TransportPacket::transport_priority() const {
   return (byte1 & 0x20) == 0x20;
};
inline uint16 TransportPacket::PID() const {
   return (((byte1 & 0x1f) << 8) | byte2);
};
inline uint8 TransportPacket::transport_scrambling_control() const {
   return (byte3 & 0xc0) >> 6;
};
inline uint8 TransportPacket::adaptation_field_control() const {
   return (byte3 & 0x30) >> 4;
};
inline bool TransportPacket::hasAdaptationField() const {
   return (byte3 & 0x20) == 0x20;
};
inline bool TransportPacket::hasPayload() const {
   return (byte3 & 0x10) == 0x10;
};
inline uint8 TransportPacket::continuity_counter() const {
   return (byte3 & 0x0f);
};



   
   

#endif /* TRANSPORTPACKET_H */
