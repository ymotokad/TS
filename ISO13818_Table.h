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

#ifndef	ISO13818_TABLE_H
#define	ISO13818_TABLE_H
#ifdef IMPLEMENTING_ISO13818_TABLE
static const char ISO13818_Table_rcsid[] = "@(#)$Id$";
#endif
#include "TSTypes.h"
#include "BitStream.h"

/*
 * ISO13818_Section class
 */
class ISO13818_Section : public BitStream {
public:
   ISO13818_Section();
   ISO13818_Section(uint8 continuity_counter);
   ~ISO13818_Section();
   bool isComplete() const;
   int append(uint8 continuity_counter, const ByteArray &src, int off, int len = -1);

   uint8 table_id() const;
   bool section_syntax_indicator() const;
   uint16 section_length() const;
   uint8 last_continuity_counter() const;

protected:
   // Order of bit stream data
   static const int pos_table_id			= 0;
   static const int pos_section_syntax_indicator	= 1;
   static const int pos_reserved1			= 2;
   static const int pos_section_length			= 3;
   static const int pos_start_of_section		= 4;
   
 protected:
   uint8 last_cc;
   void initobj();
};

inline uint8 ISO13818_Section::last_continuity_counter() const {
   return last_cc;
}

/*
 * SubTable class
 */
class SubTable {
public:
   SubTable();
   virtual uint8 version_number() const = 0;
   int addSection(const ISO13818_Section &section);

protected:
   // array of Section
};

/*
 * ISO13818_Table class
 */
class ISO13818_Table {
public:
   ISO13818_Table();
private:
};

#endif	/* ISO13818_TABLE_H */
