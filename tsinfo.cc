/**
 *
 *
 */
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include "StdLogger.h"
#include "TransportStream.h"

static char rcsid[] = "@(#)$Id$";

Logger *logger;

#define PROBE_SIZE	100000

/*
 * Managing PIDs
 */
class PIDInfo {
public:
   PIDInfo() {
      counter = 0;
      program = -1;
   }
   int counter;
   int program;
};
typedef std::map<uint16, PIDInfo*> PIDMap;
class PIDManager {
public:
   PIDManager() {
   }
   virtual ~PIDManager() {
      PIDMap::iterator itr = pinfos.begin();
      while (itr != pinfos.end()) {
	 delete itr->second;
	 itr++;
      }
   }
   void count(uint16 pid) {
      PIDMap::iterator itr = pinfos.find(pid);
      if (itr == pinfos.end()) {
	 PIDInfo *ip = new PIDInfo();
	 ip->counter++;
	 pinfos[pid] = ip;
      } else {
	 PIDInfo *ip = itr->second;
	 ip->counter++;
      }
   }
   uint16 mostFrequentPID() const {
      int freq_count = 0;
      int freq_pid = 0;
      PIDMap::const_iterator itr = pinfos.begin();
      while (itr != pinfos.end()) {
	 if (freq_count < itr->second->counter) {
	    freq_count = itr->second->counter;
	    freq_pid = itr->first;
	 }
	 itr++;
      }
      return freq_pid;
   }
   void setProgramByPID(uint16 pid, uint16 program) {
      PIDInfo *ip;
      PIDMap::iterator itr = pinfos.find(pid);
      if (itr == pinfos.end()) {
	 ip = new PIDInfo();
	 pinfos[pid] = ip;
      } else {
	 ip = itr->second;
      }
      ip->program = program;
   }
   int getProgramByPID(uint16 pid) const {
      PIDMap::const_iterator itr = pinfos.find(pid);
      if (itr == pinfos.end()) {
	 return -1;
      }
      return itr->second->program;
   }
private:
   PIDMap pinfos;
};

/*
 * for EIT
 */
class Component {
public:
   Component(const char *content) {
      this->content = content;
   }
   const char *content;
};
typedef std::map<uint8, Component*> ComponentMap; // key is a component_tag
class ComponentInfo {
public:
   ComponentInfo() {
   }
   virtual ~ComponentInfo() {
      ComponentMap::iterator itr = components.begin();
      while (itr != components.end()) {
	 delete itr->second;
	 itr++;
      }
   }
   void setComponent(uint8 component_tag, Component *pinfo) {
      ComponentMap::iterator itr = components.find(component_tag);
      if (itr != components.end()) {
	 delete itr->second;
      }
      components[component_tag] = pinfo;
   }
   Component *getComponent(uint8 component_tag) const {
      ComponentMap::const_iterator itr = components.find(component_tag);
      if (itr == components.end()) return NULL;
      return itr->second;
   }

private:
   ComponentMap components;
};

/*
 * for PMT
 */
class Stream {
public:
   Stream(uint8 component_tag) {
      this->component_tag = component_tag;
   }
   uint8 component_tag;
};

#define MAX_STREAMS 10
class ProgramInfo {
public:
   ProgramInfo() {
      num_streams = 0;
   }
   virtual ~ProgramInfo() {
      for (int i = 0; i < num_streams; i++) {
	 delete streams[i];
      }
   }
   void addStream(uint8 component_tag) {
      if (num_streams < MAX_STREAMS) {
	 streams[num_streams++] = new Stream(component_tag);
      }
   }
   int length() { return num_streams; }
   Stream *streamAt(int idx) {
      if (idx < num_streams) {
	 return streams[idx];
      }
      return NULL;
   }
   
private:
   Stream *streams[MAX_STREAMS];
   int num_streams;
};


typedef std::map<uint16, ProgramInfo*> ProgramInfoMap; // key is a program
typedef std::map<uint16, ComponentInfo*> ComponentInfoMap; // key is a program
class ProgramManager {
public:
   ProgramManager() {
   }
   virtual ~ProgramManager() {
      ProgramInfoMap::iterator itr = programs.begin();
      while (itr != programs.end()) {
	 delete itr->second;
	 itr++;
      }
   }
   void setProgramInfo(uint16 pno, ProgramInfo *pinfo) {
      ProgramInfoMap::iterator itr = programs.find(pno);
      if (itr != programs.end()) {
	 delete itr->second;
      }
      programs[pno] = pinfo;
   }
   ProgramInfo *getProgramInfo(uint16 pno) const {
      ProgramInfoMap::const_iterator itr = programs.find(pno);
      if (itr == programs.end()) return NULL;
      return itr->second;
   }
   void setComponentInfo(uint16 pno, ComponentInfo *cinfo) {
      ComponentInfoMap::iterator itr = components.find(pno);
      if (itr != components.end()) {
	 delete itr->second;
      }
      components[pno] = cinfo;
   }
   ComponentInfo *getComponentInfo(uint16 pno) const {
      ComponentInfoMap::const_iterator itr = components.find(pno);
      if (itr == components.end()) return NULL;
      return itr->second;
   }

private:
   ProgramInfoMap programs;
   ComponentInfoMap components;
};


/*
 * Callback functions
 */
void setProgramInfo(uint16 pid, uint16 program, uint8 sttype, uint8 component_tag, void *dtp) {
   PIDManager *pidmgr = (PIDManager *)dtp;
   pidmgr->setProgramByPID(pid, program);
}
void setProgramStream(uint16 pid, uint16 program, uint8 sttype, uint8 component_tag, void *dtp) {
   ProgramInfo *pi = (ProgramInfo *)dtp;
   pi->addStream(component_tag);
}
void setProgramComponents(uint16 program, const char *str, uint8 component_tag, void *dtp) {
   ComponentInfo *cip = (ComponentInfo *)dtp;
   Component *cp = cip->getComponent(component_tag);
   if (cp == NULL) {
      cp = new Component(str);
      cip->setComponent(component_tag, cp);
   }
   cp->content = str;
}



static void usage(const char *argv0) {
   std::cerr << "usage: " << argv0 << " [options]" << " -i input.ts" << std::endl;
   std::cerr << " options:" << std::endl;
   std::cerr << "   -v            print verbose message" << std::endl;
   std::cerr << "   -s            show program information" << std::endl;
   std::cerr << "   -d            print debug information" << std::endl;
   std::cerr << "   -p            print the most major program id" << std::endl;
   std::cerr << "   -c            print stream details for the most major program" << std::endl;
   std::cerr << "   -k N          probe in leading N packets. If not specified, " << PROBE_SIZE << " will be used. Use 0 to evaluate all packets." << std::endl;
}

/*
 * main
 */
int main(int argc, char *argv[]) {
   char *argv0;
   bool opt_v = false;
   bool opt_d = false;
   bool opt_s = false;
   bool opt_p = false;
   bool opt_c = false;
   char *opt_i = NULL;
   int probe_size = PROBE_SIZE;

   // Parse command line options.
   int option_char;
   while ((option_char = getopt(argc, argv, "vdsi:pck:")) != -1) {
      switch (option_char) {
      case 'v':
	 opt_v = true;
	 break;
      case 'd':
	 opt_d = true;
	 opt_v = true;
	 break;
      case 's':
	 opt_s = true;
	 break;
      case 'i':
	 opt_i = optarg;
	 break;
      case 'p':
	 opt_p = true;
	 break;
      case 'c':
	 opt_c = true;
	 break;
      case 'k':
	 probe_size = atoi(optarg);
	 break;
      case 'h':
      default:
	 usage(argv[0]);
	 return 1;
      }
   }
   argv0 = argv[0];
   argc -= optind;
   argv += optind;
   if (argc != 0 || opt_i == NULL) {
      usage(argv0);
      return 1;
   }

   int logmode = LOGGER_ERROR | LOGGER_WARNING;
   if (opt_d) {
      logmode |= LOGGER_DEBUG;
   }
   StdLogger lgr(logmode);
   logger = &lgr;

   // Open files
   std::ifstream ifs(opt_i);
   if (!ifs) {
      std::cerr << argv0 << ": error openning file " << opt_i << std::endl;
      return 1;
   }
   BufferedInputStream *bisp = new BufferedInputStream(&ifs);

   int packet_counter = 0;

   // Input stream
   TransportStream ts;
   ts.setOption_dump(opt_v);
   ts.setOption_showProgramInfo(opt_s);
   ifs.exceptions(std::ios::badbit);
   try {
      PIDManager pid_manager;
      ProgramManager prog_manager;
      
      while (!bisp->eof()) {
	 packet_counter++;
	 if (probe_size != 0 && packet_counter > probe_size) break;

	 /*----------------------------
	  * Decode input stream
	  */
	 int len = ts.decode(bisp);
	 if (len <= 0) {
	    if (bisp->eof()) break;
	    logger->error("main: failed to decode input stream");
	    return 1;
	 }

	 pid_manager.count(ts.packet->PID());

	 if (ts.isActiveTSEvent(TSEvent_Update_ProgramMapTable)) {
	    for (int i = 0; i < ts.programs_updated.size(); i++) {
	       uint16 pmt_pid = ts.getPIDByProgram(ts.programs_updated[i]);
	       if (pmt_pid != 0) {
		  ProgramMapSection *pmt = ts.getProgramMapTableByPID(pmt_pid);
		  assert(pmt->isComplete());
		  pmt->for_all_streams(setProgramInfo, &pid_manager);

		  ProgramInfo *pi = new ProgramInfo();
		  pmt->for_all_streams(setProgramStream, pi);
		  prog_manager.setProgramInfo(ts.programs_updated[i], pi);
	       }
	    }
	 }

	 if (ts.isActiveTSEvent(TSEvent_Update_EventInformationTable_Actual_Present)) {
	    const EventInformationTable *eit = ts.getEventInformationTabale();
	    assert(eit != NULL);
	    ComponentInfo *cip = prog_manager.getComponentInfo(eit->service_id());
	    if (cip == NULL) {
	       cip = new ComponentInfo();
	       prog_manager.setComponentInfo(eit->service_id(), cip);
	    }
	    eit->for_all_components(setProgramComponents, cip);
	 }

	 /*----------------------------
	  * Post processings
	  */
	 if (ts.isActiveTSEvent(TSEvent_Update_ProgramMapTable)) {
	    ts.programs_updated.clear();
	 }

      }

      if (opt_p) {
	 uint16 pid = pid_manager.mostFrequentPID();
	 if (pid != 0) {
	    uint16 program = pid_manager.getProgramByPID(pid);
	    printf("program id: %d\n", program);
	    
	    if (opt_c) {
	       ProgramInfo *pip = prog_manager.getProgramInfo(program);
	       if (pip != NULL) {
		  for (int i = 0; i < pip->length(); i++) {
		     Stream *sp = pip->streamAt(i);
		     ComponentInfo *cip = prog_manager.getComponentInfo(program);
		     if (cip != NULL) {
			Component *cp = cip->getComponent(sp->component_tag);
			if (cp != NULL) {
			   printf("  Stream: %s\n", cp->content);
			}
		     }
		  }
	       }
	    }
	 }
      }
   } catch (const std::ios::failure& error) {
      std::cerr << "I/O exception: " << error.what() << std::endl;
      return 1;
   }
   
   ifs.close();

   return 0;
}
