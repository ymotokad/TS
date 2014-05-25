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
#include <ctime>
#include <list>
#include "TSTypes.h"
#include "Table.h"
#include "TransportPacket.h"
#include "ProgramAssociationSection.h"
#include "ProgramMapSection.h"
#include "EventInformationTable.h"
#include "SystemClock.h"


typedef std::vector<ByteArray *> ByteArrayList;

typedef std::map<uint16, Section*, std::less<uint16> > Pid2SectionMap;

typedef std::map<uint16, uint8, std::less<uint16> > Program2VersionMap;


// Trasnport Stream Event Handling
typedef uint32 TSEvent;
#define TSEvent_Update_ProgramMapTable				((TSEvent)(1<<0))
#define TSEvent_Update_ProgramAssociationTable			((TSEvent)(1<<1))
#define TSEvent_Update_EventInformationTable_Actual_Present	((TSEvent)(1<<2))
#define TSEvent_Update_Time					((TSEvent)(1<<3))


/*
 * BufferedInputStream
 */
class BufferedInputStream {
public:
   BufferedInputStream(std::istream *isp);
   ByteArray *read(int len);
   void unread(const ByteArray &buff);
   bool eof() const;
private:
   std::istream *isp;
   ByteArrayList bufferUnread;
   int idxUnread;
};
inline bool BufferedInputStream::eof() const {
   if (bufferUnread.size() > 0) return false;
   return isp->eof();
}


/*
 * TransportStream
 */
class TransportStream {
 public:
   TransportStream();
   virtual ~TransportStream();
   int decode(BufferedInputStream *isp);
   void setOption_dump(bool onoff = true);
   void setOption_showProgramInfo(bool onoff = true);
   void setOption_writeTransportStream(const char *filename, bool onoff = true);
   const ProgramClock *getStreamTime() const;
   const ProgramClock *getStreamTimeRoundUpBy(int seconds) const;
   void reset();

 private:
   void dumpPacket(const TransportPacket &packet);
   bool loadOption_dump;
   bool loadOption_showProgramInfo;
   bool loadOption_writeTransportStream;
   const char *filename;
   std::ostream *outStream;

 private:
   void loadTable(uint16 pid, const Section &section);
   void loadProgramAssociationTable(const Section &section);
   void loadProgramMapTable(const Section &section, uint16 pid);
   void loadServiceDescriptionTable(const Section &section);
   void loadTimeDateSection(const Section &section);
   void loadEventInformationTable(const Section &section);

   // Contextual Informations
 private:
   TSEvent tsEvent;
   void setTSEvent(TSEvent flag);
   void clearTSEvent();
   SystemClock sysclock;
   ProgramAssociationSection *latestProgramAssociationTable;
   Program2VersionMap latestEventInformationVersionByProgram;
   int packet_counter;
   EventInformationTable *latestEventInformationTabale_Actual_Present;
 public:
   TransportPacket *packet;
   bool isActiveTSEvent(TSEvent flag) const;
   std::time_t getTime();
   ProgramAssociationSection *getLatestPAT() const;
   std::vector<uint16> programs;
   uint16 getPIDByProgram(uint16 program) const;
   uint16 getProgramMapPIDByPID(uint16 pid) const;
   ProgramMapSection *getProgramMapTableByPID(uint16 pid) const;
   const EventInformationTable *getEventInformationTabale() const;
   std::vector<uint16> programs_updated;  // A list of programs whose PMT was updated
   
   // Program Specific Information
 private:
   void setPIDByProgram(uint16 program, uint16 pid);
   void setProgramMapSectionByPID(uint16 pid, ProgramMapSection *pmt);
   bool isProgramMapTablePID(uint16 pid) const;
   void clearProgramMapTables();
   bool isPSIComplete() const;
   void showPSI() const;
   std::map<uint16, uint16> program2PID;
   std::map<uint16, uint16> PID2Program;
   std::map<uint16, ProgramMapSection*> PID2ProgramMapSection;

   // Section managements
 private:
   Pid2SectionMap incompleteSections;
   void setIncompleteSection(uint16 pid, Section *sec);
   void unsetIncompleteSection(uint16 pid);
   void clearIncompleteSection();
   Section *getIncompleteSection(uint16 pid) const;
};

inline const ProgramClock *TransportStream::getStreamTime() const {
   return sysclock.getStreamTime();
}

inline const ProgramClock *TransportStream::getStreamTimeRoundUpBy(int seconds) const {
   return sysclock.getStreamTimeRoundUpBy(seconds);
}

inline void TransportStream::setOption_dump(bool onoff) {
   loadOption_dump = onoff;
}

inline void TransportStream::setOption_showProgramInfo(bool onoff) {
   loadOption_showProgramInfo = onoff;
}

inline void TransportStream::setOption_writeTransportStream(const char *filename, bool onoff) {
   this->filename = filename;
   loadOption_writeTransportStream = onoff;
}

inline void TransportStream::setTSEvent(TSEvent flag) {
   tsEvent |= flag;
}

inline void TransportStream::clearTSEvent() {
   tsEvent = 0;
}

inline bool TransportStream::isActiveTSEvent(TSEvent flag) const {
   return ((tsEvent & flag) != 0);
}

inline const EventInformationTable *TransportStream::getEventInformationTabale() const {
   return latestEventInformationTabale_Actual_Present;
}


#endif /* TRANSPORTSTREAM_H */
