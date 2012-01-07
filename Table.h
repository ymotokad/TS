// This may look like C code, but it is really -*- C++ -*-
/*
 * 
 * 
 * @(#)$Id$
 */
#ifndef	TABLE_H
#define	TABLE_H
#ifdef IMPLEMENTING_TABLE
static const char Table_rcsid[] = "@(#)$Id$";
#endif
#include "TSTypes.h"
#include "BitStream.h"

/*
 * Section class
 */
class Section : public BitStream {
public:
   Section();
   Section(uint8 continuity_counter);
   ~Section();
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

inline uint8 Section::last_continuity_counter() const {
   return last_cc;
}

/*
 * SubTable class
 */
class SubTable {
public:
   SubTable();
   virtual uint8 version_number() const = 0;
   int addSection(const Section &section);

protected:
   // array of Section
};

/*
 * Table class
 */
class Table {
public:
   Table();
private:
};

#endif	/* TABLE_H */
