/**
 *
 *
 */
#include <iostream>
#include <fstream>
#include <getopt.h>
#include "TransportStream.h"
#include "StdLogger.h"

static char rcsid[] = "@(#)$Id$";

bool opt_v = false;
Logger *logger;

static void usage(const char *argv0) {
   std::cerr << "usage: " << argv0 << " [-v]" << " tsfile" << std::endl;
}

int main(int argc, char *argv[]) {
   int i;
   char *argv0;

   // Parse command line options.
   int option_char;
   while ((option_char = getopt(argc, argv, "v")) != -1) {
      switch (option_char) {
      case 'v':
	 opt_v = true;
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
   if (argc != 1) {
      usage(argv0);
      return 1;
   }

   // Open TS file for input
   std::ifstream ifs(argv[0]);

   //StdLogger lgr(LOGGER_ERROR | LOGGER_WARNING | LOGGER_DEBUG);
   StdLogger lgr(LOGGER_ERROR | LOGGER_WARNING);
   logger = &lgr;

   TransportStream ts;
   ts.setOption_dump(false);
   ts.setOption_showProgramInfo(true);
   ts.setOption_writeTransportStream(NULL, false);
   
   ifs.exceptions(std::ios::badbit);
   try {
      while (!ifs.eof()) {
	 int len = ts.decode(&ifs);
	 if (len < 0 && !ifs.eof()) {
	    logger->error("main: failed to decode input stream");
	    return 1;
	 }

	 if (ts.isActiveTSEvent(TSEvent_Update_Time)) {
	    std::time_t t = ts.getLatestTimestamp();
	    std::tm *tp = localtime(&t);
	    char buf[1024];
	    strftime(buf, sizeof buf, "%Y/%m/%d %H:%M:%S", tp);
	    printf("*** %s ***\n", buf);
	 }
	 if (ts.isActiveTSEvent(TSEvent_Update_ProgramAssociationTable)) {
	    ProgramAssociationSection *pat = ts.getLatestPAT();
	    int numPrograms = pat->numPrograms();
	    printf("*** Program Association Table ***\n");
	    for (int i = 0; i < numPrograms; i++) {
	       uint16 pno = pat->program_number(i);
	       printf("  program no=%d, PID=0x%04x\n", pno, pat->program_map_PID(i));
	    }
	 }
	 if (ts.isActiveTSEvent(TSEvent_Update_ProgramMapTable)) {
	    printf("*** Program Map Table ***\n");
	    int idx = 0;
	    while (idx < ts.programs_updated.size()) {
	       uint16 pid = ts.getPIDByProgram(ts.programs_updated[idx]);
	       ProgramMapSection *pmt = ts.getProgramMapTableByPID(pid);
	       if (pmt != NULL) {
		  assert(pmt->isComplete());
		  printf("-- pid: 0x%04x, program=%d, PCR_PID=0x%04x\n", (int)pid, (int)ts.programs_updated[idx], pmt->PCR_PID());
		  pmt->dump(&std::cout);
	       }
	       idx++;
	    }
	    ts.programs_updated.clear();
	 }
      }
   } catch (const std::ios::failure& error) {
      std::cerr << "I/O exception: " << error.what() << std::endl;
   }
   
   return 0;
}
