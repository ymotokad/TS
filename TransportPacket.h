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
#include "ByteArrayBuffer.h"
#include "BitStreamSyntax.h"
#include "PacketSection.h"
#include "AdaptationField.h"
#include "ProgramAssociationSection.h"
#include "ProgramMapSection.h"
#include "TSContext.h"

class TransportPacket;

class TransportPacket : public PacketSection {
 public:
   TransportPacket();
   ~TransportPacket();
   int load(TSContext *tsc, std::istream *inputstream);
   int load(const ByteArray *);
   void process(TSContext *tsc);
   void dump(std::ostream *outputstream) const;
   void dump(TSContext *tsc, std::ostream *outputstream) const;
   uint16 PID() const;
 protected:
   uint8 sync_byte() const;
   bool transport_error_indicator() const;
   bool payload_unit_start_indicator() const;
   bool transport_priority() const;
   uint8 transport_scrambling_control() const;
   uint8 adaptation_field_control() const;
   bool has_adaptation_field() const;
   bool has_payload() const;
   uint8 continuity_counter() const;

   AdaptationField *adaptationField;
   ProgramAssociationSection *programAssociationSection;
   ProgramMapSection *programMapSection;
   ByteArray *payload;
   int length;
};

#define SYNC_BYTE_VALUE 0x47

#define PID_ProgramAssociationTable		0x0000
#define PID_ConditionalAccessTable		0x0001
#define PID_TransportStreamDescriptionTable	0x0002
#define PID_NullPacket				0x1fff

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


#endif /* TRANSPORTPACKET_H */
