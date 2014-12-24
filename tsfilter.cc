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
#include "MPEGStream.h"
#include "ADTSStream.h"
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
   std::cerr << "   -k N          probe leading N packets and skip until begining of the program, which is judged by Program Map Table change" << std::endl;
   std::cerr << "   -s seconds    skip begining packets for specified seconds" << std::endl;
   std::cerr << "   -t seconds    stop after writing specified seconds of packets to output stream" << std::endl;
   std::cerr << "   -l filename   store first 100MB of the inputfile for debugging" << std::endl;
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
 * Managing PID to PES
 */
typedef std::map<uint16, PacketizedElementaryStream *, std::less<int> > PID2PESMap;
class PESManager {
public:
   ~PESManager() {
      for (PID2PESMap::iterator itr = pess.begin(); itr != pess.end(); itr++) {
	 delete itr->second;
      }
   }
   void clear() {
      for (PID2PESMap::iterator itr = pess.begin(); itr != pess.end(); itr++) {
	 delete itr->second;
      }
      pess.clear();
   }
   void setPES(uint16 pid, PacketizedElementaryStream *pes) {
      pess[pid] = pes;
   }
   PacketizedElementaryStream *getPES(uint16 pid) {
      PID2PESMap::const_iterator itr = pess.find(pid);
      if (itr == pess.end()) return NULL;
      return itr->second;
   }
private:
   PID2PESMap pess;
};

/*
 * Callback function for PMT parse
 */
void RegisterPIDFromPMT(uint16 pid, uint16 program, uint8 sttype, uint8 component_tag, void *dtp) {
   ActivePID *pids = (ActivePID *)dtp;
   pids->activate(pid);
}
void MapPESFromPMT(uint16 pid, uint16 program, uint8 sttype, uint8 component_tag, void *dtp) {
   PESManager *pm = (PESManager *)dtp;
   if (sttype == ProgramMapSection::sttype_VideoMPEG2) {
      if (pm->getPES(pid) == NULL) {
	 pm->setPES(pid, new MPEGStream());
      }
   } else if (sttype == ProgramMapSection::sttype_AudioAAC) {
      if (pm->getPES(pid) == NULL) {
	 pm->setPES(pid, new ADTSStream());
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
   char *opt_i = NULL;
   char *opt_o = NULL;
   char *opt_l = NULL;
   int program_id = -1;
   int probe_size = 0;
   int seconds_to_skip = 0;
   int seconds_to_record = 0;
   ByteArraySpool *spool = NULL;

   // Parse command line options.
   int option_char;
   while ((option_char = getopt(argc, argv, "vdi:o:p:egk:s:t:")) != -1) {
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
      case 'l':
	 opt_l = optarg;
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
   std::ofstream lfs;
   int lfs_remainingBytes = 0;
   if (opt_l != NULL) {
      lfs.open(opt_l);
      if (!lfs) {
	 std::cerr << argv0 << ": error openning file " << opt_l << std::endl;
	 return 1;
      }
      lfs.exceptions(std::ios::badbit);
      lfs_remainingBytes = 100 * 1024 * 1024;
   }

   // Input stream
   TransportStream ts;
   ts.setOption_dump(opt_v);
   ts.setOption_showProgramInfo(opt_v);
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
	       PacketizedElementaryStream *pes = pes_manager.getPES(ts.packet->PID());
	       if (pes != NULL && ts.packet->has_payload()) {
		  pes->put(ts.packet->getPayload());
		  ElementaryStream *obj;
		  while ((obj = pes->readObject()) != NULL) {
		     if (obj->getStreamType() == obj->StreamType_MPEG) {
			MPEGHeader *mpeg = (MPEGHeader *)obj;
			if (writer_status.is(WriterStatus::WaitingForGOP)) {
			   uint8 c = mpeg->start_code();
			   if (c == MPEGStream::StartCode_GroupOfPictures) {
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
			//MPEGStream::dumpHeader(mpeg);
		     } else if (obj->getStreamType() == obj->StreamType_ADTS) {
			ADTSHeader *adts = (ADTSHeader *)obj;
			//printf("DBGPES: adts sync=0x%04x\n", adts->sync_word());
		     }
		     delete obj;
		  }
	       }
	    }
	    /*----------------------------
	     * Store for debugging
	     */
	    if (lfs_remainingBytes > 0) {
	       const ByteArray *rawdata = ts.packet->getRawdata();
	       assert(rawdata->length() == SIZEOF_PACKET);
	       lfs.write((const char *)rawdata->part(), rawdata->length());
	       lfs_remainingBytes -= rawdata->length();
	    }

	    /*----------------------------
	     * Filterings
	     */
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
			pmt->for_all_streams(MapPESFromPMT, &pes_manager);
		     }
		  }
	       }
	    }

	    /*----------------------------
	     * Skip Leading Packets?
	     */
	    if (writer_status.is(WriterStatus::SkippingLeadingPackets)) {
	       const ProgramClock *now = ts.getStreamTime();
	       if (skip_until == NULL) {
		  if (now->isInitialized()) {
		     ProgramClock *t = new ProgramClock(*now);
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
		  assert(now->isInitialized());
		  if (skip_until->isGreaterThanOrEqualTo(*now)) {
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
	     * Check Write Timer
	     */
	    if (writer_status.is(WriterStatus::WritingByTime)) {
	       assert(seconds_to_record != 0);
	       const ProgramClock *now = ts.getStreamTime();
	       if (record_until == NULL) {
		  if (now->isInitialized()) {
		     record_until = new ProgramClock(*now);
		     record_until->append(seconds_to_record);
		  } else {
		     if (packet_counter > MAX_PACKETS_WITHOUT_PCR) {
			logger->error("Couldn't find PCR in %d packets.", MAX_PACKETS_WITHOUT_PCR);
			return 1;
		     }
		  }
	       } else {
		  if (!record_until->isGreaterThanOrEqualTo(*now)) {
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
	       if (ts.isActiveTSEvent(TSEvent_Update_ProgramMapTable | TSEvent_Update_EventInformationTable_Actual_Present)) {
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
			//ofs.write((const char *)rawdata->part(), rawdata->length());
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

	 packet_counter++;
      }
   } catch (const std::ios::failure& error) {
      std::cerr << "I/O exception: " << error.what() << std::endl;
      return 1;
   }
   
   if (opt_o) {
      if (spool != NULL) {
	 for (int i = 0; i < spool->length(); i++) {
	    const ByteArray *rawdata = spool->dataAt(i);
	    ofs.write((const char *)rawdata->part(), rawdata->length());
	 }
	 delete spool;
      }
      ofs.close();
   }
   lfs.close();
   ifs.close();

   return 0;
}
