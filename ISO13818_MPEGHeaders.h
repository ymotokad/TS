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

#ifndef	ISO13818_MPEGHEADERS_H
#define	ISO13818_MPEGHEADERS_H
#ifdef IMPLEMENTING_ISO13818_MPEGHEADERS
static const char ISO13818_MPEGHeaders_rcsid[] = "@(#)$Id$";
#endif
#include "TSTypes.h"
#include "ElementaryStream.h"

/*
 * ISO13818_MPEGHeader class
 */
class ISO13818_MPEGHeader : public ElementaryStream {
public:
   ISO13818_MPEGHeader();
   void setBuffer(const ByteArray &data);

   uint8 start_code() const;

protected:
   // Order of bit stream data
   static const int pos_start_code_prefix		= 0;
   static const int pos_start_code			= 1;
   
 protected:
   void initobj();
   void initialize();
};

inline uint8 ISO13818_MPEGHeader::start_code() const {
   return bit_field8(pos_start_code);
}


/*
 * MPEGPictureHeader class
 */
class MPEGPictureHeader : public ISO13818_MPEGHeader {
 public:
   MPEGPictureHeader();
   uint16 temporal_sequence_number() const;
   uint8 frame_type() const;
   uint16 VBV_delay() const;
 protected:
   // Order of bit stream data
   static const int pos_temporal_sequence_number	= 2;
   static const int pos_frame_type			= 3;
   static const int pos_VBV_delay			= 4;
      
   void initobj();
};

inline uint16 MPEGPictureHeader::temporal_sequence_number() const {
   return bit_field32(pos_temporal_sequence_number);
}

inline uint8 MPEGPictureHeader::frame_type() const {
   return bit_field32(pos_frame_type);
}

inline uint16 MPEGPictureHeader::VBV_delay() const {
   return bit_field32(pos_VBV_delay);
}


/*
 * MPEGGroupOfPictures class
 */
class MPEGGroupOfPictures : public ISO13818_MPEGHeader {
 public:
   MPEGGroupOfPictures();
   bool drop_frame_flag() const;
   uint8 hour() const;
   uint8 minute() const;
   uint8 second() const;
   uint8 frame() const;
   bool closed_GOP() const;
   bool broken_GOP() const;
 protected:
   // Order of bit stream data
   static const int pos_drop_frame_flag		= 2;
   static const int pos_hour			= 3;
   static const int pos_minute			= 4;
   static const int pos_reserved		= 5;
   static const int pos_second			= 6;
   static const int pos_frame			= 7;
   static const int pos_closed_GOP		= 8;
   static const int pos_broken_GOP		= 9;
      
   void initobj();
};

inline bool MPEGGroupOfPictures::drop_frame_flag() const {
   return bit_field1(pos_drop_frame_flag);
}

inline uint8 MPEGGroupOfPictures::hour() const {
   return bit_field32(pos_hour);
}

inline uint8 MPEGGroupOfPictures::minute() const {
   return bit_field32(pos_minute);
}

inline uint8 MPEGGroupOfPictures::second() const {
   return bit_field32(pos_second);
}

inline uint8 MPEGGroupOfPictures::frame() const {
   return bit_field32(pos_frame);
}

inline bool MPEGGroupOfPictures::closed_GOP() const {
   return bit_field32(pos_closed_GOP);
}

inline bool MPEGGroupOfPictures::broken_GOP() const {
   return bit_field32(pos_broken_GOP);
}

#endif	/* ISO13818_MPEGHEADERS_H */
