/*
 * TransportStream - 
 *
 * 
 *
 */
static char rcsid[] = "@(#)$Id$";
#ifdef __GNUG__
#pragma implementation
#endif
#include <stdio.h>
#include <assert.h>
#define IMPLEMENTING_TRANSPORTSTREAM
#include "TransportStream.h"
#include "ProgramMapSection.h"
#include "ServiceDescriptionTable.h"
#include "EventInformationTable.h"
#include "TimeDateSection.h"
#include "AdaptationField.h"
   

/*
 * constructors 
 */

TransportStream::TransportStream() {
   loadOption_dump = false;
   loadOption_showProgramInfo = false;
   loadOption_writeTransportStream = false;
   latestProgramAssociationTable = NULL;
   tsEvent = 0;
   packet = NULL;
   packet_counter = 0;
}


/*
 * destructors 
 */

TransportStream::~TransportStream() {
   if (packet != NULL) delete packet;
   clearProgramMapTables();
}


/*
 * other methods
 */

#define NUM_BYTES_LOOK_FORWARD		(SIZEOF_PACKET * 3)
#define NUM_PACKETS_MATCH		4
static ByteArrayBuffer *find_packet(BufferedInputStream *isp) {
   ByteArrayBuffer *buff = isp->read(NUM_BYTES_LOOK_FORWARD + (SIZEOF_PACKET * (NUM_PACKETS_MATCH + 1)));
   if (buff == NULL) return NULL;
   if (buff->length() != NUM_BYTES_LOOK_FORWARD + (SIZEOF_PACKET * (NUM_PACKETS_MATCH + 1))) {
      logger->warning("find_packet: Tried to read %d bytes, but actually read %d bytes", NUM_BYTES_LOOK_FORWARD + (SIZEOF_PACKET * (NUM_PACKETS_MATCH + 1)), buff->length());
   }
   int idx_limit = buff->length() - (SIZEOF_PACKET * (NUM_PACKETS_MATCH + 1));
   for (int idx = 0; idx < idx_limit; idx++) {
      if (buff->at(idx) == SYNC_BYTE_VALUE) {
	 int n;
	 for (n = 1; n < NUM_PACKETS_MATCH; n++) {
	    if (buff->at(idx + SIZEOF_PACKET * n) != SYNC_BYTE_VALUE) break;
	 }
	 if (n == NUM_PACKETS_MATCH) {
	    logger->warning("decode: found sync_byte (0x%x) by skipping %d bytes", SYNC_BYTE_VALUE, idx);
	    // found
	    ByteArray *pushback = buff->subarray(idx);
	    isp->unread(*pushback);
	    delete pushback;
	    delete buff;
	    buff = isp->read(SIZEOF_PACKET);
	    assert(buff != NULL);
	    return buff;
	 }
      }
   }

   // couldn't find
   logger->warning("find_packet: couldn't find sync_byte after peeking %d bytes.", idx_limit);
   ByteArray *pushback = buff->subarray(NUM_BYTES_LOOK_FORWARD);
   isp->unread(*pushback);
   delete pushback;
   delete buff;
   return NULL;
}

int TransportStream::decode(BufferedInputStream *isp) {
   if (packet != NULL) {
      delete packet;
      packet = NULL;
   }
   packet_counter++;
   
   ByteArrayBuffer *buffer = isp->read(SIZEOF_PACKET);
   if (isp->eof()) return 0;
   packet = new TransportPacket(buffer);
   if (packet->sync_byte() != SYNC_BYTE_VALUE) {
      logger->warning("decode: wrong sync_byte (0x%x). skipping data...", packet->sync_byte());

      // Seek for sync byte
      ByteArray *pushback = buffer->subarray(1);
      isp->unread(*pushback);
      delete pushback;
      delete packet;
      packet = NULL;
      buffer = find_packet(isp);
      if (buffer == NULL) return 0;
      packet = new TransportPacket(buffer);
   }
      
   if (loadOption_dump) {
      dumpPacket(*packet);
   }
   uint16 pid = packet->PID();

   clearTSEvent();

   // System Clock
   if (packet->has_adaptation_field()) {
      const AdaptationField *ap = packet->getAdaptationField();
      if (ap->hasCompletePCR()) {
	 sysclock.sync(pid, ap->getBase(), ap->getExt());
      } else {
	 sysclock.tick();
      }
   } else {
      sysclock.tick();
   }

   // Load carry overs
   Section *prev = getIncompleteSection(pid);
   if (prev != NULL) {
      if (packet->has_payload() && packet->getPayload()->length() > 1) {
	 int prevlen;
	 if (packet->payload_unit_start_indicator()) {
	    int pointer_field = packet->getPayload()->at(0);
	    prevlen = prev->append(packet->continuity_counter(), *(packet->getPayload()), 0, pointer_field);
	 } else {
	    prevlen = prev->append(packet->continuity_counter(), *(packet->getPayload()), 0);
	 }
	 if (prevlen < 0) {
	    logger->warning("TransportStream::decode(): cc (%d) is not subsequent to prev (%d)", packet->continuity_counter(), prev->last_continuity_counter());
	 }
	 if (prev->isComplete()) {
	    loadTable(pid, *prev);
	 }
	 unsetIncompleteSection(pid);
	 delete prev;
      }
   }

   // Process packet
   if (pid == PID_ProgramAssociationTable ||
       pid == PID_ServiceDescriptionTable ||
       pid == PID_EventInformationTable ||
       pid == PID_TimeDateSection ||
       isProgramMapTablePID(pid)) {
      if (packet->payload_unit_start_indicator() && packet->has_payload() && packet->getPayload()->length() > 1) {
	 int pointer_field = packet->getPayload()->at(0);
	 Section *sec = new Section(packet->continuity_counter());
	 if (sec == NULL) return -1;
	 sec->setBuffer(packet->getPayload()->subarray(1 + pointer_field));
	 if (sec->isComplete()) {
	    loadTable(pid, *sec);
	    delete sec;
	 } else {
	    setIncompleteSection(pid, sec);
	 }
      } else {
	 // There is a case that TS start captured at a middle of a program
      }
   } else {
      // PES
   }

   return packet->bufferLength();
}

void TransportStream::loadTable(uint16 pid, const Section &section) {
   if (pid == PID_ProgramAssociationTable) {
      loadProgramAssociationTable(section);
   } else if (isProgramMapTablePID(pid)) {
      loadProgramMapTable(section, pid);
      /*
      if (loadOption_showProgramInfo && isPSIComplete()) {
	 showPSI();
      }
      */
   } else if (pid == PID_ServiceDescriptionTable) {
      loadServiceDescriptionTable(section);
   } else if (pid == PID_TimeDateSection) {
      loadTimeDateSection(section);
   } else if (pid == PID_EventInformationTable) {
      loadEventInformationTable(section);
   }
}

void TransportStream::dumpPacket(const TransportPacket &packet) {
   std::ostream *osp = &std::cout;
   *osp << "-- Packet";
   const char *pid;
   switch (packet.PID()) {
   case PID_ProgramAssociationTable:
      pid = "Program Association Table";
      break;
   case PID_ConditionalAccessTable:
      pid = "Conditional Access Table";
      break;
   case PID_TransportStreamDescriptionTable:
      pid = "Transport Stream Description Table";
      break;
   case PID_NullPacket:
      pid = "Null Packet";
      break;
   default:
      pid = NULL;
   }

   if (pid) {
      *osp << " PID=" << pid;
   } else {
      *osp << " PID=" << std::hex << std::showbase << (int)packet.PID();
   }
   if (pid == NULL && isProgramMapTablePID(packet.PID())) {
      *osp << "(PMT)";
   }
   *osp << " cc=" << std::dec << (int)packet.continuity_counter();
   *osp << std::endl;
   //*osp << "  Packet.length: " << length << std::endl;
   if (packet.payload_unit_start_indicator()) {
      *osp << "  payload_unit_start_indicator: 1" << std::endl;
   }

   if (packet.has_adaptation_field()) {
      packet.getAdaptationField()->dump(osp);
   }

   if (packet.has_payload()) {
      //*osp << "  -- Payoad";
      //*osp << std::endl;
      //packet.hexdump(4, osp);
   }
}

void TransportStream::setIncompleteSection(uint16 pid, Section *sec) {
   logger->debug("TransportStream::setIncompleteSection(0x%04x, 0x%lx)", pid, sec);
   Pid2SectionMap::iterator itr = incompleteSections.find(pid);
   if (itr != incompleteSections.end()) {
      Section *old = itr->second;
      incompleteSections.erase(itr);
      logger->debug("  deleting old=0x%lx", old);
      delete old;
   }
   incompleteSections[pid] = sec;
}

void TransportStream::unsetIncompleteSection(uint16 pid) {
   logger->debug("TransportStream::unsetIncompleteSection(0x%04x)", pid);
   Pid2SectionMap::iterator itr = incompleteSections.find(pid);
   if (itr != incompleteSections.end()) {
      incompleteSections.erase(itr);
   }
}

Section *TransportStream::getIncompleteSection(uint16 pid) const {
   Pid2SectionMap::const_iterator itr = incompleteSections.find(pid);
   if (itr == incompleteSections.end()) return NULL;
   return itr->second;
}


/*
 * PSI methods
 */

void TransportStream::loadProgramAssociationTable(const Section &section) {
   ProgramAssociationSection pat;
   pat.setBuffer(section);
   assert(pat.isComplete());

   clearProgramMapTables();

   if (latestProgramAssociationTable == NULL ||
       latestProgramAssociationTable->version_number() != pat.version_number()) {
      if (latestProgramAssociationTable != NULL) {
	 delete latestProgramAssociationTable;
      }
      latestProgramAssociationTable = new ProgramAssociationSection();
      latestProgramAssociationTable->setBuffer(section);
      setTSEvent(TSEvent_Update_ProgramAssociationTable);
   }

   int numPrograms = pat.numPrograms();
   //logger->debug("TransportStream::loadProgramAssociationTable()");
   for (int i = 0; i < numPrograms; i++) {
      uint16 pno = pat.program_number(i);
      if (pno == 0) {
	 // Network PID
      } else {
	 //logger->debug("  register pno=%d, pid=0x%04x", pno, pat.program_map_PID(i));
	 setPIDByProgram(pno, pat.program_map_PID(i));
      }
   }
   if (loadOption_showProgramInfo && isActiveTSEvent(TSEvent_Update_ProgramAssociationTable)) {
      int numPrograms = pat.numPrograms();
      char buf[20];
      printf("*** [%s] Program Association Table ***\n", SystemClock_toString(buf, sysclock.getRelativeTime()));
      for (int i = 0; i < numPrograms; i++) {
	 uint16 pno = pat.program_number(i);
	 printf("  program no=%d, PID=0x%04x\n", pno, pat.program_map_PID(i));
      }
   }
}

void TransportStream::loadProgramMapTable(const Section &section, uint16 pid) {
   ProgramMapSection pmt;
   pmt.setBuffer(section);

   ProgramMapSection *old_pmt = getProgramMapTableByPID(pid);
   if (old_pmt == NULL || old_pmt->version_number() != pmt.version_number()) {
      assert(pmt.isComplete());
      ProgramMapSection *new_pmt = new ProgramMapSection();
      new_pmt->setBuffer(section);
      setProgramMapSectionByPID(pid, new_pmt); // deletes old_pmt implicitly
      programs_updated.push_back(new_pmt->program_number());
      setTSEvent(TSEvent_Update_ProgramMapTable);
      if (loadOption_showProgramInfo) {
	 char buf[20];
	 printf("*** [%s] Program Map Table ***\n", SystemClock_toString(buf, sysclock.getRelativeTime()));
	 assert(pmt.isComplete());
	 printf("  -- pid: 0x%04x, program=%d, PCR_PID=0x%04x\n", (int)pid, (int)pmt.program_number(), pmt.PCR_PID());
	 pmt.dump(&std::cout);
      }
   }
}

void TransportStream::loadServiceDescriptionTable(const Section &section) {
   ServiceDescriptionTable sdt;
   sdt.setBuffer(section);
   assert(sdt.table_id() == TableID_ServiceDescriptionTable_Actual ||
	  sdt.table_id() == TableID_ServiceDescriptionTable_Other);
   if (loadOption_dump) sdt.dump(&std::cout);
}

void TransportStream::loadTimeDateSection(const Section &section) {
   TimeDateSection tdt;
   tdt.setBuffer(section);
   int table_id = tdt.table_id();
   if (table_id == TableID_TimeDateSection) {
      sysclock.setAbsoluteTime(tdt.convert());
      if (loadOption_dump) tdt.dump(&std::cout);
   } else {
      TimeOffsetSection tot;
      tot.setBuffer(section);
      if (tot.table_id() != TableID_TimeOffsetSection) {
	 logger->warning("TimeDateSection: inappropriate table_id: 0x%x", tot.table_id());
	 return;
      }
      sysclock.setAbsoluteTime(tot.convert());
      if (loadOption_dump) tot.dump(&std::cout);
   }
   setTSEvent(TSEvent_Update_Time);
   if (loadOption_showProgramInfo) {
      char buf1[20];
      std::time_t t = sysclock.getAbsoluteTime();
      std::tm *tp = localtime(&t);
      char buf2[1024];
      strftime(buf2, sizeof buf2, "%Y/%m/%d %H:%M:%S", tp);
      printf("*** [%s] %s ***\n", SystemClock_toString(buf1, sysclock.getRelativeTime()), buf2);
   }
}

void TransportStream::loadEventInformationTable(const Section &section) {
   EventInformationTable eit;
   eit.setBuffer(section);

   if (!(TableID_EventInformationTable_Actual_Present <= eit.table_id() &&
	 eit.table_id() <= TableID_EventInformationTable_max)) {
      logger->warning("EventInformationTable: inappropriate table_id: 0x%x", eit.table_id());
      return;
   }

   if (eit.table_id() == TableID_EventInformationTable_Actual_Present) {
      uint16 pno = eit.service_id();
      uint8 ver = eit.version_number();
      Program2VersionMap::iterator itr = latestEventInformationVersionByProgram.find(pno);
      if (itr == latestEventInformationVersionByProgram.end()) {
	 setTSEvent(TSEvent_Update_EventInformationTable_Actual_Present);
	 latestEventInformationVersionByProgram[pno] = ver;
      } else {
	 uint8 latest_version = itr->second;
	 if (latest_version != ver) {
	    setTSEvent(TSEvent_Update_EventInformationTable_Actual_Present);
	    latestEventInformationVersionByProgram[pno] = ver;
	 }
      }
      if (loadOption_showProgramInfo && isActiveTSEvent(TSEvent_Update_EventInformationTable_Actual_Present)) {
	 char buf[20];
	 printf("*** [%s] ", SystemClock_toString(buf, sysclock.getRelativeTime()));
	 eit.dump(&std::cout);
      }
   }
}


void TransportStream::setPIDByProgram(uint16 pno, uint16 pid) {
   programs.push_back(pno);
   program2PID[pno] = pid;
   PID2Program[pid] = pno;
}

uint16 TransportStream::getPIDByProgram(uint16 pno) const {
   std::map<uint16, uint16>::const_iterator itr = program2PID.find(pno);
   if (itr == program2PID.end()) return 0;
   return itr->second;
}

bool TransportStream::isProgramMapTablePID(uint16 pid) const {
   if (PID2Program.find(pid) == PID2Program.end()) return false;
   return true;
}

void TransportStream::setProgramMapSectionByPID(uint16 pid, ProgramMapSection *pmt) {
   //logger->debug("TransportStream::setProgramMapSectionByPID(0x%04x, 0x%lx)", pid, pmt);
   std::map<uint16, ProgramMapSection*>::iterator itr = PID2ProgramMapSection.find(pid);
   if (itr != PID2ProgramMapSection.end()) {
      ProgramMapSection *old = itr->second;
      PID2ProgramMapSection.erase(itr);
      //logger->debug("  deleting old=0x%lx", old);
      delete old;
   }
   PID2ProgramMapSection[pid] = pmt;
}

ProgramMapSection *TransportStream::getProgramMapTableByPID(uint16 pid) const {
   std::map<uint16, ProgramMapSection*>::const_iterator itr = PID2ProgramMapSection.find(pid);
   if (itr == PID2ProgramMapSection.end()) return NULL;
   return itr->second;
}

void TransportStream::clearProgramMapTables() {
   /*
   int idx = 0;
   while (idx < programs.size()) {
      uint16 pid = getPIDByProgram(programs[idx]);
      ProgramMapSection *pmt = getProgramMapTableByPID(pid);
      delete pmt;
      idx++;
   }
   PID2ProgramMapSection.clear();
   */
   programs.clear();
   PID2Program.clear();
   program2PID.clear();
}

bool TransportStream::isPSIComplete() const {
   int idx = 0;
   //logger->debug("TransportStream::isPSIComplete():");
   while (idx < programs.size()) {
      uint16 pid = getPIDByProgram(programs[idx]);
      ProgramMapSection *pmt = getProgramMapTableByPID(pid);
      //logger->debug("  pid=0x%04x, pmt=0x%lx", pid, pmt);
      if (pmt == NULL || !pmt->isComplete()) return false;
      idx++;
   }
   return idx > 0;
}

void TransportStream::showPSI() const {
   printf("*** Program Information ***\n");
   int idx = 0;
   while (idx < programs.size()) {
      uint16 pid = getPIDByProgram(programs[idx]);
      ProgramMapSection *pmt = getProgramMapTableByPID(pid);
      assert(pmt->isComplete());
      printf("-- pid: 0x%04x, program=%d, PCR_PID=0x%04x\n", (int)pid, (int)programs[idx], pmt->PCR_PID());
      if (loadOption_dump) pmt->dump(&std::cout);
      idx++;
   }
}

std::time_t TransportStream::getLatestTimestamp() {
   return sysclock.getAbsoluteTime();
}

ProgramAssociationSection *TransportStream::getLatestPAT() const {
   return latestProgramAssociationTable;
}


BufferedInputStream::BufferedInputStream(std::istream *p) {
   isp = p;
   idxUnread = 0;
   bufferUnread = NULL;
}

ByteArrayBuffer *BufferedInputStream::read(int len) {
   ByteArrayBuffer *buff = NULL;
   int nRead = 0;
   if (bufferUnread) {
      assert(idxUnread < bufferUnread->length());
      nRead = len;
      if (nRead > (bufferUnread->length() - idxUnread)) {
	 nRead = bufferUnread->length() - idxUnread;
      }
      buff = new ByteArrayBuffer(bufferUnread->part(idxUnread, nRead), nRead);
      idxUnread += nRead;
      if (idxUnread == bufferUnread->length()) {
	 delete bufferUnread;
	 bufferUnread = NULL;
      }
   }
   if (nRead < len) {
      uint8 room[len - nRead];
      while (nRead < len) {
	 isp->read((char *)room, len - nRead);
	 int n = isp->gcount();
	 if (n <= 0) {
	    //logger->warning("BufferedInputStream::read(%d): gcount() returns 0. rdstate=0x%08x (fail:0x%x, eof:0x%x, bad:0x%x)", len, isp->rdstate(), isp->failbit, isp->eofbit, isp->badbit);
	    assert(isp->eof() || isp->fail());
	    return buff;
	 }
	 if (!buff) {
	    assert(nRead == 0);
	    buff = new ByteArrayBuffer(room, n);
	 } else {
	    buff->append(room, n);
	 }
	 //if (nRead + n < len) logger->warning("BufferedInputStream::read(%d): Tried to read %d bytes, but read only %d bytes this time, and %d bytes so far.", len, len - nRead, n, nRead + n);
	 nRead += n;
      }
   } else {
      assert(nRead == len);
   }
   return buff;
}

void BufferedInputStream::unread(const ByteArray &buff) {
   if (bufferUnread) {
      bufferUnread->append(buff);
   } else {
      bufferUnread = new ByteArrayBuffer(buff);
      idxUnread = 0;
   }
}
