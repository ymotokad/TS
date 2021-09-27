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
#include "ISO13818_MPEGStream.h"
#include "StdLogger.h"
#include "Spool.h"

static char rcsid[] = "@(#)$Id$";

Logger *logger;

#define MAX_PACKETS_WITHOUT_PCR		10000

static void usage(const char *argv0) {
   std::cerr << "usage: " << argv0 << " [options]" << " -i input.ts -o output.ts" << std::endl;
   std::cerr << " options:" << std::endl;
   std::cerr << "   -v            print verbose message" << std::endl;
   std::cerr << "   -d            print debug information" << std::endl;
   std::cerr << "   -p program_id add specified program to output stream" << std::endl;
   std::cerr << "   -e            add EIT/SDT to output stream" << std::endl;
   std::cerr << "   -g            sybchronize with GOP at begining" << std::endl;
   std::cerr << "   -m            skip until the first TimeDateSection, which usually transfered every 5 seconds" << std::endl;
   std::cerr << "   -k N          probe leading N packets and skip until begining of the program, which is judged by Program Map Table change" << std::endl;
   std::cerr << "   -s seconds    skip begining packets for specified seconds" << std::endl;
   std::cerr << "   -t seconds    stop after writing specified seconds of packets to output stream" << std::endl;
   std::cerr << "   -r filename   store first 100MB of the inputfile for debugging" << std::endl;
}

/*
 * Managing active PIDs
 */
#define ACTIVEPID_SHIFT		(3)
#define ACTIVEPID_BITSPERBYTE	(1 << ACTIVEPID_SHIFT) // 8
#define ACTIVEPID_NUM_PID	(0x10000)
#define ACTIVEPID_SZ		(ACTIVEPID_NUM_PID / ACTIVEPID_BITSPERBYTE)
typedef uint8 PIDMap[ACTIVEPID_SZ];
class ActivePID {
public:
   ActivePID();
   bool isActive(uint16 pid) const;
   void activate(uint16 pid);
   void deactivate(uint16 pid);
   void reset();
private:
   int pid_a(uint16 pid) const;
   int pid_b(uint16 pid) const;
   PIDMap pids;
};

inline ActivePID::ActivePID() {
   reset();
}

inline int ActivePID::pid_a(uint16 pid) const {
   return pid >> ACTIVEPID_SHIFT;
}
inline int ActivePID::pid_b(uint16 pid) const {
   return 1 << (pid & ((1 << ACTIVEPID_SHIFT) - 1));
}

inline bool ActivePID::isActive(uint16 pid) const {
   assert(pid < ACTIVEPID_SZ);
   return (pids[pid_a(pid)] & pid_b(pid)) != 0;
}

void ActivePID::activate(uint16 pid) {
   assert(pid < ACTIVEPID_SZ);
   pids[pid_a(pid)] |= pid_b(pid);
}

void ActivePID::deactivate(uint16 pid) {
   pids[pid_a(pid)] &= ~(pid_b(pid));
}

void ActivePID::reset() {
   for (int i = 0; i < ACTIVEPID_SZ; i++) {
      pids[i] = 0;
   }
}

/*
 * Callback function for PMT parse
 */
void RegisterPIDFromPMT(uint16 pid, uint16 program, uint8 sttype, uint8 component_tag, int data_component_id, void *dtp) {
   ActivePID *pids = (ActivePID *)dtp;
   pids->activate(pid);
}
void MapPESFromPMT(uint16 pid, uint16 program, uint8 sttype, uint8 component_tag, int data_component_id, void *dtp) {
   PESManager *pm = (PESManager *)dtp;
   if (sttype == ISO13818_ProgramMapSection::sttype_VideoMPEG2) {
      if (pm->getPES(pid) == NULL) {
	 pm->setPES(pid, new ISO13818_MPEGStream());
      }
   }
}

typedef Spool<ByteArray> ByteArraySpool;

/*
 * Writer Status
 */
class WriterStatus {
public:
   WriterStatus() {
      status = InitialStatus;
   }
   bool is(int sts) const {
      return status & sts;
   }

   void skipLeadingPacket() {
      status = SkippingLeadingPackets;
   }
   void waitForPMT() {
      status = WaitingForPMT;
   }
   void skipHasCompleted(bool opt_g, int seconds_to_record) {
      if (opt_g) status = WaitingForGOP;
      else if (seconds_to_record > 0) status = WritingByTime;
      else status = Writing;
   }
   void PMTHasFound(bool opt_g, int seconds_to_record) {
      if (opt_g) status = WaitingForGOP;
      else if (seconds_to_record > 0) status = WritingByTime;
      else status = Writing;
   }
   void waitForGOP() {
      status = WaitingForGOP;
   }
   void waitForTDS() {
      status = WaitingForTDS;
   }
   void startWriting() {
      status = Writing;
   }
   void startTimedWriting() {
      status = WritingByTime;
   }
   void stopWriting() {
      status = WritingCompleted;
   }

   static const int InitialStatus		= (1 << 0);
   static const int SkippingLeadingPackets	= (1 << 1);
   static const int WaitingForPMT		= (1 << 2);
   static const int WaitingForGOP		= (1 << 3);
   static const int Writing			= (1 << 4);
   static const int WritingByTime		= (1 << 5);
   static const int WritingCompleted		= (1 << 6);
   static const int WaitingForTDS		= (1 << 7);

protected:
   int status;
};


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
   bool opt_g = false;
   bool opt_m = false;
   char *opt_i = NULL;
   char *opt_o = NULL;
   char *opt_r = NULL;
   int program_id = -1;
   int probe_size = 0;
   int seconds_to_skip = 0;
   int seconds_to_record = 0;
   ByteArraySpool *spool = NULL;

   // Parse command line options.
   int option_char;
   while ((option_char = getopt(argc, argv, "vdi:o:p:egmk:s:t:r:")) != -1) {
      switch (option_char) {
      case 'v':
	 opt_v = true;
	 break;
      case 'd':
	 opt_d = true;
	 break;
      case 'e':
	 opt_e = true;
	 break;
      case 'g':
	 opt_g = true;
	 break;
      case 'm':
	 opt_m = true;
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
      case 's':
	 seconds_to_skip = atoi(optarg);
	 break;
      case 't':
	 seconds_to_record = atoi(optarg);
	 break;
      case 'r':
	 opt_r = optarg;
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
   if (argc != 0) {
      usage(argv0);
      return 1;
   }
   if (seconds_to_skip > 0 && probe_size > 0) {
      std::cerr << argv0 << ": -k cannot be combined with -s" << std::endl;
      return 1;
   }
   
   WriterStatus writer_status;
   if (seconds_to_skip > 0) {
      writer_status.skipLeadingPacket();
   } else if (probe_size > 0) {
      writer_status.waitForPMT();
   } else if (opt_g) {
      writer_status.waitForGOP();
   } else if (opt_m) {
      writer_status.waitForTDS();
   } else if (seconds_to_record > 0) {
      writer_status.startTimedWriting();
   } else {
      writer_status.startWriting();
   }

   int logmode = LOGGER_ERROR | LOGGER_WARNING;
   if (opt_d) {
      logmode |= LOGGER_DEBUG;
   }
   StdLogger lgr(logmode);
   logger = &lgr;

   // Open files
   std::istream *ifsp = NULL;
   if (opt_i == NULL) {
      ifsp = &std::cin;
   } else {
      ifsp = new std::ifstream(opt_i);
      if (!ifsp) {
	 std::cerr << argv0 << ": error openning file " << opt_i << std::endl;
	 return 1;
      }
   }
   BufferedInputStream *bisp = new BufferedInputStream(ifsp);
   std::ostream *ofsp = NULL;
   if (opt_o == NULL) {
      ofsp = &std::cout;
   } else {
      ofsp = new std::ofstream(opt_o);
      if (!ofsp) {
	 std::cerr << argv0 << ": error openning file " << opt_o << std::endl;
	 return 1;
      }
   }
   if (probe_size > 0) {
      spool = new ByteArraySpool(probe_size);
   }
   std::ofstream rfs;
   int rfs_remainingBytes = 0;
   if (opt_r != NULL) {
      rfs.open(opt_r);
      if (!rfs) {
	 std::cerr << argv0 << ": error openning file " << opt_r << std::endl;
	 return 1;
      }
      rfs.exceptions(std::ios::badbit);
      rfs_remainingBytes = 100 * 1024 * 1024;
   }

   // Input stream
   ISO13818_TransportStream ts;
   ts.setOption_dump(opt_v);
   ts.setOption_showProgramInfo(opt_v);
   ts.setOption_writeTransportStream(NULL, false);
   
   ifsp->exceptions(std::ios::badbit);
   ofsp->exceptions(std::ios::badbit);
   try {
      ActivePID pidFilter;
      pidFilter.activate(PID_ProgramAssociationTable);
      pidFilter.activate(PID_TimeDateSection);
      if (opt_e) {
	 pidFilter.activate(PID_EventInformationTable);
	 pidFilter.activate(PID_ServiceDescriptionTable);
      }

      int packet_counter = 0;
      const ProgramClock *skip_until = NULL;
      ProgramClock *record_until = NULL;
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
	    if (writer_status.is(WriterStatus::WaitingForGOP)) {
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
		  if (pes->getStreamType() == ISO13818_PacketizedElementaryStream::StreamType_MPEG) {
		     ElementaryStream *obj;
		     while ((obj = pes->readObject()) != NULL) {
			ISO13818_MPEGHeader *mpeg = (ISO13818_MPEGHeader *)obj;
			//printf("PES get: \n"); mpeg->hexdump(2, &std::cout);
			if (writer_status.is(WriterStatus::WaitingForGOP)) {
			   uint8 c = mpeg->start_code();
			   if (c == ISO13818_MPEGStream::StartCode_GroupOfPictures) {
			      if (seconds_to_record > 0) {
				 writer_status.startTimedWriting();
			      } else {
				 writer_status.startWriting();
			      }
			      break;
			   }
			} else {
			   // Just discard it
			}
			//ISO13818_MPEGStream::dumpHeader(mpeg);
			delete obj;
		     }
		  }
	       }
	    }
	    /*----------------------------
	     * Store rawdata for debugging
	     */
	    if (rfs_remainingBytes > 0) {
	       const ByteArray *rawdata = ts.packet->getRawdata();
	       assert(rawdata->length() == SIZEOF_PACKET);
	       rfs.write((const char *)rawdata->part(), rawdata->length());
	       rfs_remainingBytes -= rawdata->length();
	    }

	    /*----------------------------
	     * Filterings
	     */
	    if (ts.checkTSEvent(TSEvent_Update_ProgramAssociationTable)) {
	       ISO13818_ProgramAssociationSection *pat = ts.getLatestPAT();
	       int numPrograms = pat->numPrograms();
	       for (int i = 0; i < numPrograms; i++) {
		  uint16 pno = pat->program_number(i);
		  if (program_id < 0 || pno == 0 || pno == program_id) {
		     pidFilter.activate(pat->program_map_PID(i));
		  }
	       }
	    }
	    if (ts.checkTSEvent(TSEvent_Update_ProgramMapTable)) {
	       for (int i = 0; i < ts.programs_updated.size(); i++) {
		  uint16 pno = ts.programs_updated[i];
		  if (program_id < 0 || pno == 0 || pno == program_id) {
		     uint16 pmt_pid = ts.getPIDByProgram(ts.programs_updated[i]);
		     pidFilter.activate(pmt_pid);
		     ISO13818_ProgramMapSection *pmt = ts.getProgramMapTableByPID(pmt_pid);
		     assert(pmt != NULL);
		     assert(pmt->isComplete());
		     pidFilter.activate(pmt->PCR_PID());
		     pmt->for_all_streams(RegisterPIDFromPMT, &pidFilter);
		     pmt->for_all_streams(MapPESFromPMT, &pes_manager);
		  }
	       }
	    }

	    /*----------------------------
	     * Skip Leading Packets?
	     */
	    if (writer_status.is(WriterStatus::SkippingLeadingPackets)) {
	       const ProgramClock now = ts.getStreamTime();
	       if (skip_until == NULL) {
		  if (now.isInitialized()) {
		     ProgramClock *t = new ProgramClock(now);
		     assert(seconds_to_skip > 0);
		     t->append(seconds_to_skip);
		     skip_until = t;
		  } else {
		     if (packet_counter > MAX_PACKETS_WITHOUT_PCR) {
			logger->error("Couldn't find PCR in %d packets.", MAX_PACKETS_WITHOUT_PCR);
			return 1;
		     }
		  }
		  packet_counter++;
		  continue;
	       } else {
		  assert(now.isInitialized());
		  if (skip_until->isGreaterThanOrEqualTo(now)) {
		     packet_counter++;
		     continue;
		  } else {
		     delete skip_until;
		     skip_until = NULL;
		     writer_status.skipHasCompleted(opt_g, seconds_to_record);
		  }
	       }
	    }

	    /*----------------------------
	     * Skip until TimeDateSection?
	     */
	    if (writer_status.is(WriterStatus::WaitingForTDS)) {
	       if (ts.checkTSEvent(TSEvent_Update_Time)) {
		  if (seconds_to_record > 0) {
		     writer_status.startTimedWriting();
		  } else {
		     writer_status.startWriting();
		  }
	       }
	    }

	    /*----------------------------
	     * Check Write Timer
	     */
	    if (writer_status.is(WriterStatus::WritingByTime)) {
	       assert(seconds_to_record != 0);
	       const ProgramClock now = ts.getStreamTime();
	       if (record_until == NULL) {
		  if (now.isInitialized()) {
		     record_until = new ProgramClock(now);
		     record_until->append(seconds_to_record);
		  } else {
		     if (packet_counter > MAX_PACKETS_WITHOUT_PCR) {
			logger->error("Couldn't find PCR in %d packets.", MAX_PACKETS_WITHOUT_PCR);
			return 1;
		     }
		  }
	       } else {
		  if (!record_until->isGreaterThanOrEqualTo(now)) {
		     delete record_until;
		     record_until = NULL;
		     writer_status.stopWriting();
		     break;
		  }
	       }
	    }

	    /*----------------------------
	     * Spool
	     */
	    if (writer_status.is(WriterStatus::WaitingForPMT)) {
	       if (ts.checkTSEvent(TSEvent_Update_ProgramMapTable | TSEvent_Update_EventInformationTable_Actual_Present)) {
		  spool->flush();
	       } else {
		  if (spool->length() < probe_size) {
		     spool->append(*(ts.packet->getRawdata()));
		  } else {
		     // spool is full. push all the data in the spool back into input stream and stop spooling
		     for (int i = probe_size - 1; i >= 0; i--) {
			const ByteArray *rawdata = spool->dataAt(i);
			bisp->unread(*rawdata);
			//assert(rawdata->length() == SIZEOF_PACKET);
			//ofsp->write((const char *)rawdata->part(), rawdata->length());
		     }
		     delete spool;
		     spool = NULL;
		     ts.reset();
		     writer_status.PMTHasFound(opt_g, seconds_to_record);
		  }
	       }
	    }
	    
	    /*----------------------------
	     * Write output stream
	     */
	    if (writer_status.is(WriterStatus::Writing |
				 WriterStatus::WritingByTime)) {
	       if (pidFilter.isActive(ts.packet->PID())) {
		  const ByteArray *rawdata = ts.packet->getRawdata();
		  assert(rawdata->length() == SIZEOF_PACKET);
		  ofsp->write((const char *)rawdata->part(), rawdata->length());
	       }
	    }
	 }

	 /*----------------------------
	  * Post processings
	  */
	 if (ts.checkTSEvent(TSEvent_Update_ProgramMapTable)) {
	    ts.programs_updated.clear();
	 }

	 packet_counter++;
      }
   } catch (const std::ios::failure& error) {
      std::cerr << "I/O exception: " << error.what() << std::endl;
      return 1;
   }
   
   if (spool != NULL) {
      for (int i = 0; i < spool->length(); i++) {
	 const ByteArray *rawdata = spool->dataAt(i);
	 ofsp->write((const char *)rawdata->part(), rawdata->length());
      }
      delete spool;
   }
   if (opt_r != NULL) {
      rfs.close();
   }
   if (opt_i) {
      delete ifsp;
   }
   ofsp->flush();
   if (opt_o) {
      delete ofsp;
   }

   return 0;
}
