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

#ifndef B24_CAPTION_H
#define B24_CAPTION_H
#ifdef __GNUG__
#pragma interface
#endif
#ifdef IMPLEMENTING_B24_CAPTION
static const char *rcsid_Nbp = "@(#)$Id$";
#endif /* IMPLEMENTING_B24_CAPTION */
#include "BitStream.h"
#include "ISO13818_PacketizedElementaryStream.h"
#include "ISO13818_SystemClock.h"


/*---------------------------
 * CaptionWriter class
 */
class CaptionWriter {
 public:
   CaptionWriter();
   virtual ~CaptionWriter();
   void setBaseTime(const ProgramClock &clk);
   void tellTime(const ProgramClock &clk);
   void append(unsigned char c);
   void append(std::string src);
   void delay(int deciseconds);
   virtual void flush();
   bool empty() const;
   void clear();
 protected:
   ProgramClock base_time;
   ProgramClock end_time() const;
   ProgramClock start_time;
   ProgramClock told_time;
   ProgramClock *delay_until;
   std::string caption;
};

inline void CaptionWriter::setBaseTime(const ProgramClock &clk) {
   base_time.set(clk);
}

inline bool CaptionWriter::empty() const {
   return caption.empty();
}


class B24_Caption_CaptionManagementData;
class B24_Caption_CaptionData;

/*---------------------------
 * B24_Caption_DataGroup
 */
class B24_Caption_DataGroup : public ElementaryStream {
 public:
   B24_Caption_DataGroup();
   uint8 data_group_id() const;
   uint16 data_group_size() const;
   uint8 data_group_link_number() const;
   uint8 last_data_group_link_number() const;
   void dump(std::ostream *osp) const;
   friend class B24_Caption_CaptionManagementData;
   friend class B24_Caption_CaptionData;
 protected:
   void initobj();
   void initvars();
};


/*
 * Order of data
 */
#define B24_Caption_DataGroup_data_group_id			0
#define B24_Caption_DataGroup_data_group_version		1
#define B24_Caption_DataGroup_data_group_link_number		2
#define B24_Caption_DataGroup_last_data_group_link_number	3
#define B24_Caption_DataGroup_data_group_size			4
#define B24_Caption_DataGroup_data_group_data_byte		5


inline uint8 B24_Caption_DataGroup::data_group_id() const {
   return bit_field8(B24_Caption_DataGroup_data_group_id);
};

inline uint16 B24_Caption_DataGroup::data_group_size() const {
   return bit_field16(B24_Caption_DataGroup_data_group_size);
};

inline uint8 B24_Caption_DataGroup::data_group_link_number() const {
   return bit_field8(B24_Caption_DataGroup_data_group_link_number);
};

inline uint8 B24_Caption_DataGroup::last_data_group_link_number() const {
   return bit_field8(B24_Caption_DataGroup_last_data_group_link_number);
};


/*---------------------------
 * B24_Caption_CaptionManagementData
 */
class B24_Caption_CaptionManagementData : public BitStream {
 public:
   B24_Caption_CaptionManagementData();
   B24_Caption_CaptionManagementData(const B24_Caption_DataGroup &dg);
   uint8 TMD() const;
   uint8 num_languages() const;
   void dump(std::ostream *osp) const;
 protected:
   void initobj();
   void initvars();
};


/*
 * Order of data
 */
#define B24_Caption_CaptionManagementData_TMD			0
#define B24_Caption_CaptionManagementData_Reserved1		1
#define B24_Caption_CaptionManagementData_num_languages		2


inline uint8 B24_Caption_CaptionManagementData::TMD() const {
   return bit_field8(B24_Caption_CaptionManagementData_TMD);
};

inline uint8 B24_Caption_CaptionManagementData::num_languages() const {
   return bit_field8(B24_Caption_CaptionManagementData_num_languages);
};


/*---------------------------
 * B24_Caption_CaptionData
 */
class B24_Caption_CaptionData : public BitStream {
 public:
   B24_Caption_CaptionData();
   B24_Caption_CaptionData(const B24_Caption_DataGroup &dg);
   uint8 TMD() const;
   uint32 data_unit_loop_length() const;
   void convert(CaptionWriter *writer);
   void dump(std::ostream *osp) const;
 protected:
   ByteArray getDataStream() const;
   void initobj();
   void initvars();
};


/*
 * Order of data
 */
#define B24_Caption_CaptionData_TMD			0
#define B24_Caption_CaptionData_Reserved1		1
#define B24_Caption_CaptionData_data_unit_loop_length	2
#define B24_Caption_CaptionData_data_unit_start		3


inline uint8 B24_Caption_CaptionData::TMD() const {
   return bit_field8(B24_Caption_CaptionManagementData_TMD);
};

inline uint32 B24_Caption_CaptionData::data_unit_loop_length() const {
   return bit_field32(B24_Caption_CaptionData_data_unit_loop_length);
};


/*---------------------------
 * B24_Caption_DataUnit
 */
class B24_Caption_DataUnit : public BitStream {
 public:
   B24_Caption_DataUnit();
   B24_Caption_DataUnit(const ByteArray &ba, int off = 0);
   uint8 unit_separator() const;
   uint8 data_unit_parameter() const;
   uint32 data_unit_size() const;
   void convert(CaptionWriter *writer);
   void dump(std::ostream *osp) const;
   int length() const;
 protected:
   ByteArray getCaptionStream() const;
   void initobj();
   void initvars();
};


/*
 * Order of data
 */
#define B24_Caption_DataUnit_unit_separator		0
#define B24_Caption_DataUnit_data_unit_parameter	1
#define B24_Caption_DataUnit_data_unit_size		2
#define B24_Caption_DataUnit_data_unit_data_byte	3


inline uint8 B24_Caption_DataUnit::unit_separator() const {
   return bit_field8(B24_Caption_DataUnit_unit_separator);
};

inline uint8 B24_Caption_DataUnit::data_unit_parameter() const {
   return bit_field8(B24_Caption_DataUnit_data_unit_parameter);
};

inline uint32 B24_Caption_DataUnit::data_unit_size() const {
   return bit_field32(B24_Caption_DataUnit_data_unit_size);
};

inline int B24_Caption_DataUnit::length() const {
   return sizeofBufferBefore(B24_Caption_DataUnit_data_unit_data_byte) + data_unit_size();
};


/*---------------------------
 * B24_CaptionStream class
 */
class B24_CaptionStream : public ISO13818_PacketizedElementaryStream {
 public:
   B24_CaptionStream() : ISO13818_PacketizedElementaryStream(StreamType_Caption) {
   }
   virtual ElementaryStream *readObject(); // ByteArray returned have to be deleted by caller side
 protected:
};



#endif /* B24_CAPTION_H */
