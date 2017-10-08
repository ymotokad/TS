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

#ifndef	TSTYPES_H
#define	TSTYPES_H
#ifdef IMPLEMENTING_TSTYPES
static char TSTypes_rcsid[] = "@(#)$Id$";
#endif
#include <stdint.h>
#include "Logger.h"


// Predefined PID
#define PID_ProgramAssociationTable			0x0000
#define PID_ConditionalAccessTable			0x0001
#define PID_TransportStreamDescriptionTable		0x0002
#define PID_ServiceDescriptionTable			0x0011
#define PID_EventInformationTable			0x0012
#define PID_TimeDateSection				0x0014
#define PID_NullPacket					0x1fff

// Table ID
#define TableID_ProgramAssociationSection		0x00
#define TableID_ConditionalAccessSection		0x01
#define TableID_TSProgramMapSection			0x02
#define TableID_TSDescriptionSection			0x03
#define TableID_ServiceDescriptionTable_Actual		0x42
#define TableID_ServiceDescriptionTable_Other		0x46
#define TableID_EventInformationTable_Actual_Present	0x4e
#define TableID_EventInformationTable_Other_Present	0x4f
#define TableID_EventInformationTable_max		0x6f
#define TableID_TimeDateSection				0x70
#define TableID_TimeOffsetSection			0x73


#define TS_CLOCK_FREQUENCY	27000000 // 27 MHz
#define TS_CLOCK_EXTENSION_MAX	299 // extension field takes 0 to 299
#define TS_CLOCK_BASE_FREQUENCY	(TS_CLOCK_FREQUENCY / (TS_CLOCK_EXTENSION_MAX + 1)) // 90 KHz
#define TS_CLOCK_BASE_BITLEN	33



extern Logger *logger;

typedef uint8_t uint8;		// 8bit unsigned integer
typedef uint16_t uint16;	// 16bit unsigned integer
typedef uint32_t uint32;	// 32bit unsigned integer
typedef uint64_t uint64;	// 64bit unsigned integer
typedef int64_t int64;		// 64bit signed integer

#endif /* TSTYPES_H */
