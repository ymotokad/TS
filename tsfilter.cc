/**
 *
 *
 */
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include "TransportStream.h"
#include "StdLogger.h"
#include "Spool.h"

static char rcsid[] = "@(#)$Id$";

Logger *logger;

static void usage(const char *argv0) {
   std::cerr << "usage: " << argv0 << " [options]" << " -i input.ts [-o output.ts]" << std::endl;
   std::cerr << " options:" << std::endl;
   std::cerr << "   -v            print verbose message" << std::endl;
   std::cerr << "   -s            show program information" << std::endl;
   std::cerr << "   -d            print debug information" << std::endl;
   std::cerr << "   -p program_id add specified program to output stream" << std::endl;
   std::cerr << "   -e            add EIT/SDT to output stream" << std::endl;
   std::cerr << "   -k N          probe leading N packets and skip until begining of the program, which is judged by Program Map Table change" << std::endl;
}

/*
 * Managing active PIDs
 */
typedef std::map<uint16, int, std::less<int> > PIDMap;
class ActivePID {
public:
   bool isActive(uint16 pid) const;
   void activate(uint16 pid);
   void deactivate(uint16 pid);
   void reset();
private:
   PIDMap pids;
};

inline bool ActivePID::isActive(uint16 pid) const {
   PIDMap::const_iterator itr = pids.find(pid);
   if (itr == pids.end()) return false;
   return true;
}

void ActivePID::activate(uint16 pid) {
   pids[pid] = 1;
}

void ActivePID::deactivate(uint16 pid) {
   PIDMap::iterator itr = pids.find(pid);
   if (itr == pids.end()) return;
   pids.erase(itr);
}

void ActivePID::reset() {
   pids.clear();
}

/*
 * Callback function for PMT parse
 */
void RegisterPIDFromPMT(uint16 pid, uint16 program, const char *tagstr, uint8 component_tag, void *dtp) {
   ActivePID *pids = (ActivePID *)dtp;
   pids->activate(pid);
}

typedef Spool<ByteArray> ByteArraySpool;

/*
 * main
 */
int main(int argc, char *argv[]) {
   int i;
   char *argv0;
   bool opt_v = false;
   bool opt_d = false;
   bool opt_s = false;
   bool opt_e = false;
   char *opt_i = NULL;
   char *opt_o = NULL;
   int program_id = -1;
   int probe_size = 0;
   ByteArraySpool *spool = NULL;

   // Parse command line options.
   int option_char;
   while ((option_char = getopt(argc, argv, "vdsi:o:p:ek:")) != -1) {
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
      case 'e':
	 opt_e = true;
	 break;
      case 'i':
	 opt_i = optarg;
	 break;
      case 'o':
	 opt_o = optarg;
	 break;
      case 'p':
	 program_id = atoi(optarg);
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
   std::ofstream ofs;
   if (opt_o != NULL) {
      ofs.open(opt_o);
      if (!ofs) {
	 std::cerr << argv0 << ": error openning file " << opt_o << std::endl;
	 return 1;
      }
      if (probe_size > 0) {
	 spool = new ByteArraySpool(probe_size);
      }
   }

   // Input stream
   TransportStream ts;
   ts.setOption_dump(opt_v);
   ts.setOption_showProgramInfo(opt_s);
   ts.setOption_writeTransportStream(NULL, false);
   
   ifs.exceptions(std::ios::badbit);
   ofs.exceptions(std::ios::badbit);
   try {
      ActivePID pidFilter;
      if (opt_o) {
	 pidFilter.activate(PID_ProgramAssociationTable);
	 pidFilter.activate(PID_TimeDateSection);
	 if (opt_e) {
	    pidFilter.activate(PID_EventInformationTable);
	    pidFilter.activate(PID_ServiceDescriptionTable);
	 }
      }
      
      while (!bisp->eof()) {

	 /*----------------------------
	  * Decode input stream
	  */
	 int len = ts.decode(bisp);
	 if (len <= 0) {
	    if (bisp->eof()) break;
	    logger->error("main: failed to decode input stream");
	    return 1;
	 }

	 if (opt_o && ts.packet) {
	    /*----------------------------
	     * Filterings
	     */
	    bool writePacket = true;

	    if (program_id >= 0) {
	       if (ts.isActiveTSEvent(TSEvent_Update_ProgramAssociationTable)) {
		  ProgramAssociationSection *pat = ts.getLatestPAT();
		  int numPrograms = pat->numPrograms();
		  for (int i = 0; i < numPrograms; i++) {
		     uint16 pno = pat->program_number(i);
		     if (pno == 0 || pno == program_id) {
			pidFilter.activate(pat->program_map_PID(i));
		     }
		  }
	       }
	       if (ts.isActiveTSEvent(TSEvent_Update_ProgramMapTable)) {
		  for (int i = 0; i < ts.programs_updated.size(); i++) {
		     uint16 pno = ts.programs_updated[i];
		     if (pno == 0 || pno == program_id) {
			uint16 pmt_pid = ts.getPIDByProgram(ts.programs_updated[i]);
			pidFilter.activate(pmt_pid);
			ProgramMapSection *pmt = ts.getProgramMapTableByPID(pmt_pid);
			assert(pmt != NULL);
			assert(pmt->isComplete());
			pidFilter.activate(pmt->PCR_PID());
			pmt->for_all_streams(RegisterPIDFromPMT, &pidFilter);
		     }
		  }
	       }
	    }

	    if (!pidFilter.isActive(ts.packet->PID())) {
	       writePacket = false;
	    }

	    /*----------------------------
	     * Write output stream
	     */
	    if (writePacket) {
	       if (spool) {
		  if (ts.isActiveTSEvent(TSEvent_Update_ProgramMapTable)) {
		     spool->flush();
		  }
	       
		  if (spool->length() < probe_size) {
		     spool->append(*(ts.packet->getRawdata()));
		  } else {
		     // spool is full. write all the data in the spool and stop spooling
		     assert(writePacket);
		     for (int i = 0; i < probe_size; i++) {
			const ByteArray *rawdata = spool->dataAt(i);
			assert(rawdata->length() == SIZEOF_PACKET);
			ofs.write((const char *)rawdata->part(), rawdata->length());
		     }
		     delete spool;
		     spool = NULL;
		  
		     const ByteArray *rawdata = ts.packet->getRawdata();
		     assert(rawdata->length() == SIZEOF_PACKET);
		     ofs.write((const char *)rawdata->part(), rawdata->length());
		  }
	       } else {
		  const ByteArray *rawdata = ts.packet->getRawdata();
		  assert(rawdata->length() == SIZEOF_PACKET);
		  ofs.write((const char *)rawdata->part(), rawdata->length());
	       }
	    }
	 }

	 /*----------------------------
	  * Post processings
	  */
	 if (ts.isActiveTSEvent(TSEvent_Update_ProgramMapTable)) {
	    ts.programs_updated.clear();
	 }
      }
   } catch (const std::ios::failure& error) {
      std::cerr << "I/O exception: " << error.what() << std::endl;
      return 1;
   }
   
   if (opt_o) {
      ofs.close();
   }
   ifs.close();
   if (spool != NULL) {
      delete spool;
   }

   return 0;
}
