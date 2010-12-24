#ifndef TRANSPORTSTREAM_H
#define TRANSPORTSTREAM_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_TRANSPORTSTREAM
static const char *rcsid_TransportStream = "@(#)$Id$";
#endif /* IMPLEMENTING_TRANSPORTSTREAM */
#include <iostream>
#include <vector>
#include <map>
#include "TSTypes.h"
#include "TransportPacket.h"
#include "ProgramMapSection.h"

class TransportStream {
 public:
   TransportStream();
   ~TransportStream();
   void process(std::istream *isp);
   void setProcessOption_dump(bool onoff = true);
   void setProcessOption_showProgramInfo(bool onoff = true);
   void setProcessOption_writeTransportStream(const char *filename, bool onoff = true);

 private:
   void dumpPacket(const TransportPacket &packet) const;
   bool processOption_dump;
   bool processOption_showProgramInfo;
   bool processOption_writeTransportStream;
   const char *filename;
   std::ostream *outStream;

   // Program Specific Information
 private:
   void processProgramAssociationTable(const TransportPacket &packet);
   void processProgramMapTable(const TransportPacket &packet);
   void setPIDByProgram(uint16 program, uint16 pid);
   uint16 getPIDByProgram(uint16 program) const;
   void setProgramMapSectionByPID(uint16 pid, ProgramMapSection *pmt);
   ProgramMapSection *getProgramMapSectionByPID(uint16 pid) const;
   bool isProgramMapTablePID(uint16 pid) const;
   void clearProgramMapTables();
   bool isPSIComplete() const;
   void showPSI() const;
   std::vector<uint16> programs;
   std::map<uint16, uint16> program2PID;
   std::map<uint16, uint16> PID2Program;
   std::map<uint16, ProgramMapSection*> PID2ProgramMapSection;
};

#define PID_ProgramAssociationTable		0x0000
#define PID_ConditionalAccessTable		0x0001
#define PID_TransportStreamDescriptionTable	0x0002
#define PID_NullPacket				0x1fff

inline void TransportStream::setProcessOption_dump(bool onoff) {
   processOption_dump = onoff;
}

inline void TransportStream::setProcessOption_showProgramInfo(bool onoff) {
   processOption_showProgramInfo = onoff;
}

inline void TransportStream::setProcessOption_writeTransportStream(const char *filename, bool onoff) {
   this->filename = filename;
   processOption_writeTransportStream = onoff;
}

#endif /* TRANSPORTSTREAM_H */
