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


/*
 * constructors 
 */

TransportStream::TransportStream() {
   processOption_dump = false;
   processOption_showProgramInfo = false;
   processOption_writeTransportStream = false;
}


/*
 * destructors 
 */

TransportStream::~TransportStream() {
   clearProgramMapTables();   
}


/*
 * other methods
 */

void TransportStream::process(std::istream *isp) {
   isp->exceptions(std::ios::badbit);
   try {
      while (!isp->eof()) {
	 TransportPacket packet(isp);
	 if (processOption_dump) {
	    dumpPacket(packet);
	 }

	 if (packet.PID() == PID_ProgramAssociationTable) {
	    processProgramAssociationTable(packet);
	 } else if (isProgramMapTablePID(packet.PID())) {
	    processProgramMapTable(packet);
	    if (processOption_showProgramInfo && isPSIComplete()) {
	       showPSI();
	    }
	 }
      }
   } catch (const std::ios::failure& error) {
      std::cerr << "I/O exception: " << error.what() << std::endl;
   }
}

void TransportStream::dumpPacket(const TransportPacket &packet) const {
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
      //adaptationField->dump(osp);
   }

   if (packet.has_payload()) {
      //*osp << "  -- Payoad";
      //*osp << std::endl;
      //packet.hexdump(4, osp);
   }
}


/*
 * PSI methods
 */

void TransportStream::processProgramAssociationTable(const TransportPacket &packet) {
   ProgramAssociationSection *pat
      = new ProgramAssociationSection(packet.continuity_counter());
   if (pat == NULL) {
      assert(pat != NULL);
      return;
   }
   int pointer_field = packet.getPayload()->at(0);
   assert(pointer_field == 0);  // for now
   pat->setBuffer(packet.getPayload()->subarray(1 + pointer_field));
   assert(pat->isComplete()); // for now

   clearProgramMapTables();
   
   int numPrograms = pat->numPrograms();
   //logger->debug("TransportStream::processProgramAssociationTable()");
   for (int i = 0; i < numPrograms; i++) {
      uint16 pno = pat->program_number(i);
      if (pno == 0) {
	 // Network PID
      } else {
	 //logger->debug("  register pno=%d, pid=0x%04x", pno, pat->program_map_PID(i));
	 setPIDByProgram(pno, pat->program_map_PID(i));
      }
   }
   delete pat;
}

void TransportStream::processProgramMapTable(const TransportPacket &packet) {
   ProgramMapSection *pmt;
   uint16 pid = packet.PID();
   if (packet.payload_unit_start_indicator()) {
      pmt = new ProgramMapSection(packet.continuity_counter());
      if (pmt == NULL) return;
      int pointer_field = packet.getPayload()->at(0);
      pmt->setBuffer(packet.getPayload()->subarray(1 + pointer_field));
      setProgramMapSectionByPID(pid, pmt);
   } else {
      pmt = getProgramMapSectionByPID(pid);
      if (pmt == NULL) {
	 // There is a case that TS start captured at a middle of a program
      } else {
	 int rc = pmt->append(packet.continuity_counter(), *(packet.getPayload()));
	 if (rc < 0) {
	    std::cout << "Warning: cc doesn't match" << std::endl;
	 }
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

ProgramMapSection *TransportStream::getProgramMapSectionByPID(uint16 pid) const {
   std::map<uint16, ProgramMapSection*>::const_iterator itr = PID2ProgramMapSection.find(pid);
   if (itr == PID2ProgramMapSection.end()) return NULL;
   return itr->second;
}

bool TransportStream::isProgramMapTablePID(uint16 pid) const {
   if (PID2Program.find(pid) == PID2Program.end()) return false;
   return true;
}

void TransportStream::clearProgramMapTables() {
   /*
   int idx = 0;
   while (idx < programs.size()) {
      uint16 pid = getPIDByProgram(programs[idx]);
      ProgramMapSection *pmt = getProgramMapSectionByPID(pid);
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
      ProgramMapSection *pmt = getProgramMapSectionByPID(pid);
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
      printf("-- program: %d, pid: 0x%04x\n", (int)programs[idx], (int)pid);
      ProgramMapSection *pmt = getProgramMapSectionByPID(pid);
      assert(pmt->isComplete());
      pmt->dump(&std::cout);
      idx++;
   }
}
