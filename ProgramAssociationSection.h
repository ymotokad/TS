#ifndef PROGRAMASSOCIATIONSECTION_H
#define PROGRAMASSOCIATIONSECTION_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_PROGRAMASSOCIATIONSECTION
static const char *rcsid_Nbp = "@(#)$Id$";
#endif /* IMPLEMENTING_PROGRAMASSOCIATIONSECTION */
#include "PacketSection.h"

class ProgramAssociationSection : public PacketSection {
 public:
   ProgramAssociationSection();
   int load(TSContext *tsc, std::istream *isp);
   int load(const ByteArray *data);
   void process(TSContext *tsc);
   void dump(std::ostream *osp) const;
 protected:
   uint8 table_id() const;
   bool section_syntax_indicator() const;
   uint16 section_length() const;
   uint16 transport_stream_id() const;
   uint8 version_number() const;
   bool current_next_indicator() const;
   uint8 section_number() const;
   uint8 last_section_number() const;
   uint16 program_number(int idx) const;
   uint16 network_PID(int idx) const;
   uint16 program_map_PID(int idx) const;
};

#define TableID_ProgramAssociationSection	0x00
#define TableID_ConditionalAccessSection	0x01
#define TableID_TSProgramMapSection		0x02
#define TableID_TSDescriptionSection		0x03


/*
 * Order of data
 */
#define PAS_table_id					0
#define PAS_section_syntax_indicator			1
#define PAS_zero					2
#define PAS_reserved1					3
#define PAS_section_length				4
#define PAS_transport_stream_id				5
#define PAS_reserved2					6
#define PAS_version_number				7
#define PAS_current_next_indicator			8
#define PAS_section_number				9
#define PAS_last_section_number				10
#define PAS_START_PROGRAM_DATA				11


#endif /* PROGRAMASSOCIATIONSECTION_H */
