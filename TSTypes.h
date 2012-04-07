/*
 *
 * 
 * 
 *
 */

#ifndef	TSTYPES_H
#define	TSTYPES_H
#ifdef IMPLEMENTING_TSTYPES
static char TSTypes_rcsid[] = "@(#)$Id$";
#endif
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


extern Logger *logger;

typedef unsigned char uint8;	// 8bit unsigned integer
typedef unsigned short uint16;	// 16bit unsigned integer
typedef unsigned int uint32;	// 32bit unsigned integer

#endif /* TSTYPES_H */
