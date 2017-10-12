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
#include "B24_Caption.h"
#include "B10_Descriptors.h"
#include "ISO13818_MPEGStream.h"

static char rcsid[] = "@(#)$Id$";

Logger *logger;

static void usage(const char *argv0) {
   std::cerr << "usage: " << argv0 << " [options]" << " -i input.ts -o output.ass" << std::endl;
   std::cerr << " options:" << std::endl;
   std::cerr << "   -d             print debug information" << std::endl;
   std::cerr << "   -p program_id  select a program to extract captions" << std::endl;
}

/*
 * Callback function for PMT parse
 */
static void MapPESFromPMT(uint16 pid, uint16 program, uint8 sttype, uint8 component_tag, int data_component_id, void *dtp) {
   PESManager *pm = (PESManager *)dtp;
   ISO13818_PacketizedElementaryStream *oldpes = pm->getPES(pid);
   ISO13818_PacketizedElementaryStream *newpes = NULL;
   if (sttype == ISO13818_ProgramMapSection::sttype_Data) {
      if (data_component_id == B10_Desc_DataComponent::id_B24_caption) {
	 newpes = new B24_CaptionStream();
      }
   }

   if (newpes != NULL) {
      if (oldpes == NULL) {
	 pm->setPES(pid, newpes);
      } else {
	 if (oldpes->getStreamType() != newpes->getStreamType()) {
	    delete oldpes;
	    pm->setPES(pid, newpes);
	 }
      }
   }
}


/*
 * ASS Writer
 */
class AssWriter : public CaptionWriter {
public:
   AssWriter(std::ofstream *ofsp);
   void flush();
protected:
   std::ofstream *ofsp;
   bool initialized;
};



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
   if (argc != 0 || opt_i == NULL || opt_o == NULL) {
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
   ts.setOption_dump(opt_d);
   ts.setOption_showProgramInfo(opt_d);
   
   ifs.exceptions(std::ios::badbit);
   ofs.exceptions(std::ios::badbit);
   try {
      bool base_time_initialized = false;
      AssWriter writer(&ofs);
      PESManager pes_manager;

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

	    if (!base_time_initialized) {
	       const ProgramClock tsclock = ts.getStreamTime();
	       if (tsclock.isInitialized()) {
		  writer.setBaseTime(tsclock);
		  base_time_initialized = true;
	       }
	    }
	    
	    /*----------------------------
	     * Program managements
	     */
	    if (ts.checkTSEvent(TSEvent_Update_ProgramMapTable)) {
	       for (int i = 0; i < ts.programs_updated.size(); i++) {
		  uint16 pno = ts.programs_updated[i];
		  if (pno == 0 || pno == program_id || program_id == -1) {
		     uint16 pmt_pid = ts.getPIDByProgram(ts.programs_updated[i]);
		     ISO13818_ProgramMapSection *pmt = ts.getProgramMapTableByPID(pmt_pid);
		     assert(pmt != NULL);
		     assert(pmt->isComplete());
		     pmt->for_all_streams(MapPESFromPMT, &pes_manager);
		  }
	       }
	    }

	    /*----------------------------
	     * Decode PES
	     */
	    ISO13818_PacketizedElementaryStream *pes = pes_manager.getPES(ts.packet->PID());
	    if (pes != NULL && ts.packet->has_payload()) {
	       if (ts.packet->payload_unit_start_indicator()) {
		  pes->startUnit(ts.getStreamTime(), ts.packet->getPayload());
	       } else {
		  pes->putUnit(ts.packet->getPayload());
	       }
	       if (pes->getStreamType() == ISO13818_PacketizedElementaryStream::StreamType_Caption) {
		  B24_Caption_DataGroup *cdg = (B24_Caption_DataGroup *)pes->readObject();
		  if (cdg != NULL) {
		     if (cdg->data_group_id() == 0x0 || cdg->data_group_id() == 0x20) {
			//B24_Caption_CaptionManagementData cmd(*cdg);
		     } else {
			B24_Caption_CaptionData cd(*cdg);
			writer.tellTime(pes->getPacketStartTime());
			cd.convert(&writer);
		     }
		     delete cdg;
		  }
	       }
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

AssWriter::AssWriter(std::ofstream *ofsp) : CaptionWriter(), ofsp(ofsp), initialized(false) {
}

void AssWriter::flush() {
   assert(base_time.isInitialized());
   if (!caption.empty()) {
      if (!initialized) {
	 *ofsp << "[Script Info]" << std::endl;
	 *ofsp << "ScriptType: v4.00+" << std::endl;
	 *ofsp << "Collisions: Normal" << std::endl;
	 *ofsp << "ScaledBorderAndShadow: yes" << std::endl;
	 *ofsp << "Timer: 100.0000" << std::endl;
	 *ofsp << std::endl;
	 *ofsp << "[Events]" << std::endl;
	 *ofsp << "Format: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text" << std::endl;
	 initialized = true;
      }
      char buff[2048];
      ProgramClock start(start_time);
      start.subtract(base_time);
      ProgramClock end(end_time());
      end.subtract(base_time);
      snprintf(buff, sizeof(buff), "Dialogue: 0,%02u:%02u:%02u.%02u,%02u:%02u:%02u.%02u,Default,,,,,,%s",
	       start.toSeconds() / (60 * 60),
	       (start.toSeconds() % (60 * 60)) / 60,
	       start.toSeconds() % 60,
	       start.getMilliSeconds() / 10,
	       end.toSeconds() / (60 * 60),
	       (end.toSeconds() % (60 * 60)) / 60,
	       end.toSeconds() % 60,
	       end.getMilliSeconds() / 10,
	       caption.c_str());
      *ofsp << buff << std::endl;
   }
   clear();
}
