/*
 *  - 
 *
 * 
 *
 */
static char rcsid[] = "@(#)$Id$";
#ifdef __GNUG__
#pragma implementation
#endif
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <map>
#define IMPLEMENTING_PROGRAMMAPSECTION
#include "ProgramMapSection.h"
#include "Descriptors.h"

FIELDWIDTH_PREAMBLE(ProgramMapSection)
FIELDWIDTH_CONTENT(pos_table_id,			8)
FIELDWIDTH_CONTENT(pos_section_syntax_indicator,	1)
FIELDWIDTH_CONTENT(pos_reserved1,			3)
FIELDWIDTH_CONTENT(pos_section_length,			12)
FIELDWIDTH_CONTENT(pos_program_number,			16)
FIELDWIDTH_CONTENT(pos_reserved2,			2)
FIELDWIDTH_CONTENT(pos_version_number,			5)
FIELDWIDTH_CONTENT(pos_current_next_indicator,		1)
FIELDWIDTH_CONTENT(pos_section_number,			8)
FIELDWIDTH_CONTENT(pos_last_section_number,		8)
FIELDWIDTH_CONTENT(pos_reserved3,			3)
FIELDWIDTH_CONTENT(pos_PCR_PID,				13)
FIELDWIDTH_CONTENT(pos_reserved4,			4)
FIELDWIDTH_CONTENT(pos_program_info_length,		12)
FIELDWIDTH_CONTENT(pos_START_PROGRAM_DATA,		0)
FIELDWIDTH_POSTAMBLE(ProgramMapSection, /**/)


static const char*stream_type(int sid) {
   static const char *stream_type_descriptions[] = {
      "ITU-T | ISO/IEC Reserved",
      "Video MPEG-1",		// "ISO/IEC 11172 Video",
      "Video MPEG-2",		// "ITU-T Rec. H.262 | ISO/IEC 13818-2 Video or ISO/IEC 11172-2 constrained parameter video stream",
      "Audio MP3",			// "ISO/IEC 11172 Audio",
      "ISO/IEC 13818-3 Audio",
      "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 private_sections",
      "Data",			// "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 PES packets containing private data",
      "ISO/IEC 13522 MHEG",
      "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Annex A DSM-CC",
      "ITU-T Rec. H.222.1",
      "ISO/IEC 13818-6 type A",
      "ISO/IEC 13818-6 type B",
      "ISO/IEC 13818-6 type C",
      "Data DSM-CC",		// "ISO/IEC 13818-6 type D",
      "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 auxiliary",
      "Audio AAC (MPEG-2)",	// "ISO/IEC 13818-7 Audio with ADTS transport syntax",
      "Video MPEG-4",		// "ISO/IEC 14496-2 Visual",
      "Audio AAC (MPEG-4)",	// "ISO/IEC 14496-3 Audio with the LATM transport syntax as defined in ISO/IEC 14496-3 / AMD 1",
      "ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in PES packets",
      "ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in ISO/IEC14496_sections.",
      "ISO/IEC 13818-6 Synchronized Download Protocol",
      "Metadata carried in PES packets",
      "Metadata carried in metadata_sections",
      "Metadata carried in ISO/IEC 13818-6 Data Carousel",
      "Metadata carried in ISO/IEC 13818-6 Object Carousel",
      "Metadata carried in ISO/IEC 13818-6 Synchronized Download Protocol",
      "IPMP stream (defined in ISO/IEC 13818-11, MPEG-2 IPMP)",
      "Video H264",			// "AVC video stream as defined in ITU-T Rec. H.264 | ISO/IEC 14496-10 Video",
      "ISO/IEC 14496-3 Audio, without using any additional transport syntax, such as DST, ALS and SLS",
      "ISO/IEC 14496-17 Text",
      "Auxiliary video stream as defined in ISO/IEC 23002-3   ",
   };
   if (sid < sizeof(stream_type_descriptions) / sizeof(*stream_type_descriptions)) {
      return stream_type_descriptions[sid];
   }
   if (sid < 0x7f) return "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved";
   return "User Private";
}

/*
 * other methods
 */

#define SHORTDUMP
#ifdef SHORTDUMP
void ProgramMapSection::dump(std::ostream *osp) const {
   if (table_id() != TableID_TSProgramMapSection) {
      hexdump(2, osp);
      return;
   }
   try {
      int snum = 0;
      
      int idx = sizeofBufferBefore(pos_START_PROGRAM_DATA);
      int idxmax = idx + program_info_length();
      while (idx < idxmax) {
	 int desclen = byteAt(idx + 1);
	 idx += 2 + desclen;
      }
      idxmax = sizeofBufferBefore(pos_section_length + 1) + section_length() - 4;
      while (idx < idxmax) {
	 int sttype = byteAt(idx);
	 int elmpid = byteAt(idx + 1) << 8 | byteAt(idx + 2);
	 elmpid &= 0x1fff;
	 int eslen = byteAt(idx + 3) << 8 | byteAt(idx + 4);
	 eslen &= 0x0fff;
	 *osp << "    Stream #" << std::dec << snum << "[0x" << std::hex << elmpid << "] " << stream_type(sttype) << ": ";
	 
	 idx += 5;
	 int es_tail = idx + eslen;
	 while (idx < es_tail) {
	    int ed_tag = byteAt(idx);
	    //*osp << "ed_tag=0x" << std::hex << ed_tag << " ";
	    int len = 2 + byteAt(idx + 1);
	    if (ed_tag == Descriptor::tag_video_decode_control) {
	       Desc_VideoDecodeControl desc;
	       desc.setBuffer(*this, idx, len);
	       *osp << desc.video_encode_format_string() << " ";
	    } else if (ed_tag == Descriptor::tag_stream_identifier) {
	       Desc_StreamIdentifier desc;
	       desc.setBuffer(*this, idx, len);
	       *osp << "component_tag (for EIT)=" << desc.component_tag_string() << " ";
	    } else {
	       *osp << "ed_tag=0x" << std::hex << ed_tag << " ";
	    }
	    idx += len;
	 }
	 *osp << std::endl;
	 snum++;
      }
   } catch (ByteArrayOverflowException e) {
      logger->error("  buffer underflow!!");
   }
}
#else
void ProgramMapSection::dump(std::ostream *osp) const {
   if (table_id() != TableID_TSProgramMapSection) {
      hexdump(2, osp);
      return;
   }
   try {
      const char *tid;
      *osp << "  table_id=" << std::hex << std::showbase << (int)table_id() << std::endl;
      //*osp << "  current_next_indicator=" << std::dec << current_next_indicator() << std::endl;
      *osp << "  section_number=" << std::dec << section_number() << std::endl;
      *osp << "  last_section_number=" << std::dec << last_section_number() << std::endl;
      *osp << "  PCR_PID=" << std::hex << std::showbase << PCR_PID() << std::endl;
      //*osp << "  program_info_length=" << std::dec << program_info_length() << std::endl;

      int idx = sizeofBufferBefore(pos_START_PROGRAM_DATA);
      int idxmax = idx + program_info_length();
      hexdump(4, osp, idx, program_info_length());
      while (idx < idxmax) {
	 int desctag = byteAt(idx);
	 int desclen = byteAt(idx + 1);
	 *osp << "    descriptor=" << Descriptor::getName(desctag)
	      << ", len=" << (int)desclen + 2
	      << std::endl;
	 idx += 2 + desclen;
      }
      *osp << "  stream info" << std::endl;
      idxmax = sizeofBufferBefore(pos_section_length + 1) + section_length() - 4;
      while (idx < idxmax) {
	 int sttype = byteAt(idx);
	 int elmpid = byteAt(idx + 1) << 8 | byteAt(idx + 2);
	 elmpid &= 0x1fff;
	 int eslen = byteAt(idx + 3) << 8 | byteAt(idx + 4);
	 eslen &= 0x0fff;
	 *osp << "    --stream_type=" << std::hex << std::showbase << sttype << ": " << stream_type(sttype)
	      << ", pid=" << std::hex << std::showbase << elmpid << std::endl;
	 //*osp << "    ES_len=" << std::dec << eslen << std::endl;
	 idx += 5;
	 int es_tail = idx + eslen;
	 hexdump(8, osp, idx, eslen);
	 while (idx < es_tail) {
	    int ed_tag = byteAt(idx);
	    int len = 2 + byteAt(idx + 1);
	    *osp << "        element descriptor="  << Descriptor::getName(ed_tag)
		 << ", len=" << len
		 << std::endl;
	    if (ed_tag == Descriptor::tag_video_decode_control) {
	       Desc_VideoDecodeControl desc;
	       desc.setBuffer(*this, idx, len);
	       const char *p = desc.video_encode_format_string();
	       *osp << "          decode format="  << p << std::endl;
	    } else if (ed_tag == Descriptor::tag_stream_identifier) {
	       Desc_StreamIdentifier desc;
	       desc.setBuffer(*this, idx, len);
	       *osp << "          component tag="  << desc.component_tag_string() << std::endl;
	    }
	    idx += len;
	 }
      }
   } catch (ByteArrayOverflowException e) {
      logger->error("  buffer underflow!!");
   }
}
#endif

void ProgramMapSection::for_all_streams(StreamCallback scp, void *dtp) const {
   if (table_id() != TableID_TSProgramMapSection) {
      return;
   }
   try {
      int idx = sizeofBufferBefore(pos_START_PROGRAM_DATA);
      int idxmax = idx + program_info_length();
      while (idx < idxmax) {
	 int desclen = byteAt(idx + 1);
	 idx += 2 + desclen;
      }
      idxmax = sizeofBufferBefore(pos_section_length + 1) + section_length() - 4;
      while (idx < idxmax) {
	 int sttype = byteAt(idx);
	 int elmpid = byteAt(idx + 1) << 8 | byteAt(idx + 2);
	 elmpid &= 0x1fff;
	 int eslen = byteAt(idx + 3) << 8 | byteAt(idx + 4);
	 eslen &= 0x0fff;
	 (*scp)(elmpid, stream_type(sttype), dtp);
	 
	 idx += 5;
	 int es_tail = idx + eslen;
	 while (idx < es_tail) {
	    int ed_tag = byteAt(idx);
	    int len = 2 + byteAt(idx + 1);
	    idx += len;
	 }
      }
   } catch (ByteArrayOverflowException e) {
      logger->error("buffer underflow!!");
   }
}

uint8 ProgramMapSection::version_number() const {
   return bit_field8(pos_version_number);
};

int ProgramMapSection::section_number() const {
   return bit_field8(pos_section_number);
};

bool ProgramMapSection::current_next_indicator() const {
   return bit_field1(pos_current_next_indicator);
}

int ProgramMapSection::last_section_number() const {
   return bit_field8(pos_last_section_number);
};

uint16 ProgramMapSection::PCR_PID() const {
   return bit_field16(pos_PCR_PID);
};

uint16 ProgramMapSection::program_number() const {
   return bit_field16(pos_program_number);
};

int ProgramMapSection::program_info_length() const {
   return bit_field16(pos_program_info_length);
};
