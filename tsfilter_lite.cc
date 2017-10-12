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

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include "ISO13818_TransportStream.h"
#include "StdLogger.h"
#include "Spool.h"

static char rcsid[] = "@(#)$Id$";

Logger *logger;

static void usage(const char *argv0) {
   std::cerr << "usage: " << argv0 << " [options]" << " -i input.ts -o output.ts" << std::endl;
   std::cerr << " options:" << std::endl;
   std::cerr << "   -d            print debug information" << std::endl;
   std::cerr << "   -p program_id add specified program to output stream" << std::endl;
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
static void RegisterPIDFromPMT(uint16 pid, uint16 program, uint8 sttype, uint8 component_tag, int data_component_id, void *dtp) {
   ActivePID *pids = (ActivePID *)dtp;
   pids->activate(pid);
}


/*
 * main
 */
int main(int argc, char *argv[]) {
   int i;
   char *argv0;
   bool opt_d = false;
   char *opt_i = NULL;
   char *opt_o = NULL;
   int program_id = -1;

   // Parse command line options.
   int option_char;
   while ((option_char = getopt(argc, argv, "di:o:p:")) != -1) {
      switch (option_char) {
      case 'd':
	 opt_d = true;
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
      case 'h':
      default:
	 usage(argv[0]);
	 return 1;
      }
   }
   argv0 = argv[0];
   argc -= optind;
   argv += optind;
   if (argc != 0 || opt_i == NULL || opt_o == NULL || program_id == -1) {
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
   ofs.open(opt_o);
   if (!ofs) {
      std::cerr << argv0 << ": error openning file " << opt_o << std::endl;
      return 1;
   }

   // Input stream
   ISO13818_TransportStream ts;
   ts.setOption_writeTransportStream(NULL, false);
   
   ifs.exceptions(std::ios::badbit);
   ofs.exceptions(std::ios::badbit);
   try {
      ActivePID pidFilter;
      pidFilter.activate(PID_ProgramAssociationTable);
      pidFilter.activate(PID_TimeDateSection);
      pidFilter.activate(PID_EventInformationTable);
      pidFilter.activate(PID_ServiceDescriptionTable);

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

	 if (ts.packet) {

	    /*----------------------------
	     * Filterings
	     */
	    if (ts.checkTSEvent(TSEvent_Update_ProgramAssociationTable)) {
	       ISO13818_ProgramAssociationSection *pat = ts.getLatestPAT();
	       int numPrograms = pat->numPrograms();
	       for (int i = 0; i < numPrograms; i++) {
		  uint16 pno = pat->program_number(i);
		  if (pno == 0 || pno == program_id) {
		     pidFilter.activate(pat->program_map_PID(i));
		  }
	       }
	    }
	    if (ts.checkTSEvent(TSEvent_Update_ProgramMapTable)) {
	       for (int i = 0; i < ts.programs_updated.size(); i++) {
		  uint16 pno = ts.programs_updated[i];
		  if (pno == 0 || pno == program_id) {
		     uint16 pmt_pid = ts.getPIDByProgram(ts.programs_updated[i]);
		     pidFilter.activate(pmt_pid);
		     ISO13818_ProgramMapSection *pmt = ts.getProgramMapTableByPID(pmt_pid);
		     assert(pmt != NULL);
		     assert(pmt->isComplete());
		     pidFilter.activate(pmt->PCR_PID());
		     pmt->for_all_streams(RegisterPIDFromPMT, &pidFilter);
		  }
	       }
	    }
	    
	    /*----------------------------
	     * Write output stream
	     */
	    if (pidFilter.isActive(ts.packet->PID())) {
	       const ByteArray *rawdata = ts.packet->getRawdata();
	       assert(rawdata->length() == SIZEOF_PACKET);
	       ofs.write((const char *)rawdata->part(), rawdata->length());
	    }
	 }

	 /*----------------------------
	  * Post processings
	  */
	 if (ts.checkTSEvent(TSEvent_Update_ProgramMapTable)) {
	    ts.programs_updated.clear();
	 }
      }
   } catch (const std::ios::failure& error) {
      std::cerr << "I/O exception: " << error.what() << std::endl;
      return 1;
   }
   
   ofs.close();
   ifs.close();

   return 0;
}
