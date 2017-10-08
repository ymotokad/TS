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

#ifndef ELEMENTARYSTREAM_H
#define ELEMENTARYSTREAM_H
#ifdef IMPLEMENTING_ELEMENTARYSTREAM
static const char *rcsid_ElementaryStream = "@(#)$Id$";
#endif /* IMPLEMENTING_ELEMENTARYSTREAM */
#include <iostream>
#include "TSTypes.h"
#include "BitStream.h"

/*
 * ElementaryStream
 */
class ElementaryStream : public BitStream {
#if 1 // Whole content should be remove.
 public:
   int getStreamType() const {
      return streamtype;
   }
   static const int StreamType_MPEG	= 1;
   static const int StreamType_ADTS	= 2;
   static const int StreamType_Caption	= 3;
 protected:
   void setStreamType(int type) {
      streamtype = type; 
   }
   int streamtype;
#endif
};


#endif /* ELEMENTARYSTREAM_H */
