/**
 *
 *
 */
#include <iostream>
#include <getopt.h>
#include "TransportPacket.h"
#include "TSContext.h"

static char rcsid[] = "@(#)$Id$";

bool opt_v = false;

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

   TSContext tsc;
   std::cin.exceptions(std::ios::badbit);
   try {
      // Read header informations.
      while (!std::cin.eof()) {
	 TransportPacket packet;

	 int len = packet.load(&tsc, &std::cin);
	 if (len <= 0) break;
	 packet.process(&tsc);
	 packet.dump(&tsc, &std::cout);
      }
   } catch (const std::ios::failure& error) {
      std::cerr << "I/O exception: " << error.what() << std::endl;
      return 1;
   }
   return 0;
}
