#ifndef TSCONTEXT_H
#define TSCONTEXT_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_TSCONTEXT
static const char *rcsid_TSContext = "@(#)$Id$";
#endif /* IMPLEMENTING_TSCONTEXT */
#include <map>
#include "TSTypes.h"
class PacketSection;
class ByteArray;

class TSContext {
 public:
   TSContext();
   ~TSContext();

   // Program Map Table
   void setProgramMapTablePID(uint16 pid, uint16 program_number);
   uint16 getProgramMapTablePID(uint16 pid); // returns program_number
   bool isProgramMapTablePID(uint16 pid) const;

   // Incomplete PacketSections
   void setIncompletePacketSection(uint16 pid, PacketSection *section);
   bool isIncompletePacketSection(uint16 pid) const;
   PacketSection *appendPacketSectionData(uint16 pid, const ByteArray *data); // return packet section if completed

   // 
 protected:
   std::map<uint16, uint16> PMT_PIDs;
   std::map<uint16, PacketSection*> incompleteSections;
};



#endif /* TSCONTEXT_H */
