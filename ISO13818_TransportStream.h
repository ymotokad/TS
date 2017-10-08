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

#ifndef ISO13818_TRANSPORTSTREAM_H
#define ISO13818_TRANSPORTSTREAM_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_ISO13818_TRANSPORTSTREAM
static const char *rcsid_ISO13818_TransportStream = "@(#)$Id$";
#endif /* IMPLEMENTING_ISO13818_TRANSPORTSTREAM */
#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <list>
#include "TSTypes.h"
#include "ISO13818_Table.h"
#include "ISO13818_TransportPacket.h"
#include "ISO13818_ProgramAssociationSection.h"
#include "ISO13818_ProgramMapSection.h"
#include "B10_EventInformationTable.h"
#include "ISO13818_SystemClock.h"


typedef std::vector<ByteArray *> ByteArrayList;

typedef std::map<uint16, ISO13818_Section*, std::less<uint16> > Pid2SectionMap;

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
 * ISO13818_TransportStream
 */
class ISO13818_TransportStream {
 public:
   ISO13818_TransportStream();
   virtual ~ISO13818_TransportStream();
   int decode(BufferedInputStream *isp);
   void setOption_dump(bool onoff = true);
   void setOption_showProgramInfo(bool onoff = true);
   void setOption_writeTransportStream(const char *filename, bool onoff = true);
   const ProgramClock& getStreamTime() const;
   ProgramClock *getStreamTimeRoundUpBy(int seconds) const;
   void reset();

 private:
   void dumpPacket(const ISO13818_TransportPacket &packet);
   bool loadOption_dump;
   bool loadOption_showProgramInfo;
   bool loadOption_writeTransportStream;
   const char *filename;
   std::ostream *outStream;

 private:
   void loadTable(uint16 pid, const ISO13818_Section &section);
   void loadProgramAssociationTable(const ISO13818_Section &section);
   void loadProgramMapTable(const ISO13818_Section &section, uint16 pid);
   void loadServiceDescriptionTable(const ISO13818_Section &section);
   void loadTimeDateSection(const ISO13818_Section &section);
   void loadEventInformationTable(const ISO13818_Section &section);

   // Contextual Informations
 private:
   TSEvent tsEvent;
   void setTSEvent(TSEvent flag);
   void clearTSEvent();
   ISO13818_SystemClock sysclock;
   ISO13818_ProgramAssociationSection *latestProgramAssociationTable;
   Program2VersionMap latestEventInformationVersionByProgram;
   int packet_counter;
   B10_EventInformationTable *latestEventInformationTabale_Actual_Present;
 public:
   ISO13818_TransportPacket *packet;
   bool isActiveTSEvent(TSEvent flag) const;
   bool checkTSEvent(TSEvent flag) const;
   std::time_t getTime();
   ISO13818_ProgramAssociationSection *getLatestPAT() const;
   std::vector<uint16> programs;
   uint16 getPIDByProgram(uint16 program) const;
   uint16 getProgramMapPIDByPID(uint16 pid) const;
   ISO13818_ProgramMapSection *getProgramMapTableByPID(uint16 pid) const;
   const B10_EventInformationTable *getEventInformationTabale() const;
   std::vector<uint16> programs_updated;  // A list of programs whose PMT was updated
   
   // Program Specific Information
 private:
   void setPIDByProgram(uint16 program, uint16 pid);
   void setProgramMapSectionByPID(uint16 pid, ISO13818_ProgramMapSection *pmt);
   bool isProgramMapTablePID(uint16 pid) const;
   void clearProgramMapTables();
   bool isPSIComplete() const;
   void showPSI() const;
   std::map<uint16, uint16> program2PID;
   std::map<uint16, uint16> PID2Program;
   std::map<uint16, ISO13818_ProgramMapSection*> PID2ProgramMapSection;

   // Section managements
 private:
   Pid2SectionMap incompleteSections;
   void setIncompleteSection(uint16 pid, ISO13818_Section *sec);
   void unsetIncompleteSection(uint16 pid);
   void clearIncompleteSection();
   ISO13818_Section *getIncompleteSection(uint16 pid) const;
};

inline const ProgramClock& ISO13818_TransportStream::getStreamTime() const {
   return sysclock.getStreamTime();
}

inline ProgramClock *ISO13818_TransportStream::getStreamTimeRoundUpBy(int seconds) const {
   return sysclock.getStreamTimeRoundUpBy(seconds);
}

inline void ISO13818_TransportStream::setOption_dump(bool onoff) {
   loadOption_dump = onoff;
}

inline void ISO13818_TransportStream::setOption_showProgramInfo(bool onoff) {
   loadOption_showProgramInfo = onoff;
}

inline void ISO13818_TransportStream::setOption_writeTransportStream(const char *filename, bool onoff) {
   this->filename = filename;
   loadOption_writeTransportStream = onoff;
}

inline void ISO13818_TransportStream::setTSEvent(TSEvent flag) {
   tsEvent |= flag;
}

inline void ISO13818_TransportStream::clearTSEvent() {
   tsEvent = 0;
}

inline bool ISO13818_TransportStream::isActiveTSEvent(TSEvent flag) const {
   return ((tsEvent & flag) != 0);
}

inline bool ISO13818_TransportStream::checkTSEvent(TSEvent flag) const {
   return ((tsEvent & flag) != 0);
}

inline const B10_EventInformationTable *ISO13818_TransportStream::getEventInformationTabale() const {
   return latestEventInformationTabale_Actual_Present;
}


#endif /* ISO13818_TRANSPORTSTREAM_H */
