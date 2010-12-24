/**
 *
 *
 */
#include <iostream>
#include <getopt.h>
#include "TransportStream.h"
#include "StdLogger.h"

static char rcsid[] = "@(#)$Id$";

bool opt_v = false;
Logger *logger;

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
	 std::cerr << "usage: " << argv[0] << " [-v]" << std::endl;
	 return 1;
      }
   }
   argv0 = argv[0];
   argc -= optind;
   argv += optind;

   logger = new StdLogger(LOGGER_ERROR | LOGGER_DEBUG);

   TransportStream ts;
   ts.setProcessOption_dump(true);
   ts.setProcessOption_showProgramInfo(true);
   ts.setProcessOption_writeTransportStream(NULL, false);
   
   ts.process(&std::cin);
   
   return 0;
}
