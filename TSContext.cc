/*
 * TSContext - 
 *
 * 
 *
 */
static char rcsid[] = "@(#)$Id$";
#ifdef __GNUG__
#pragma implementation
#endif
#include <assert.h>
#define IMPLEMENTING_TSCONTEXT
#include "TSContext.h"
#include "PacketSection.h"


/*
 * constructors 
 */

TSContext::TSContext() {
}


/*
 * destructors 
 */

TSContext::~TSContext() {
}


/*
 * other methods
 */

void TSContext::setProgramMapTablePID(uint16 pid, uint16 program_number) {
   PMT_PIDs[pid] = program_number;
}

uint16 TSContext::getProgramMapTablePID(uint16 pid) {
   std::map<uint16, uint16>::iterator itr = PMT_PIDs.find(pid);
   if (itr == PMT_PIDs.end()) return 0;
   return itr->second;
}

bool TSContext::isProgramMapTablePID(uint16 pid) const {
   if (PMT_PIDs.find(pid) == PMT_PIDs.end()) return false;
   return true;
}

void TSContext::setIncompletePacketSection(uint16 pid, PacketSection *section) {
   incompleteSections[pid] = section;
}

bool TSContext::isIncompletePacketSection(uint16 pid) const {
   if (incompleteSections.find(pid) == incompleteSections.end()) return false;
   return true;
}

PacketSection *TSContext::appendPacketSectionData(uint16 pid, const ByteArray *data) {
   assert(isIncompletePacketSection(pid));
   PacketSection *sec = incompleteSections[pid];
   sec->append(*data);

   if (!sec->isCompleted()) return NULL;
   
   std::map<uint16, PacketSection *>::iterator it;
   it = incompleteSections.find(pid);
   incompleteSections.erase(it);
   return sec;
}

