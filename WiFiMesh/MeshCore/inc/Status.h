/**
 * \file Status.h
 *
 * Defines status return codes
 *
 * \author Denis Itskovich
 * \date   1/12/2008
 */

#ifndef _WIFI_MESH_STATUS_H
#define _WIFI_MESH_STATUS_H

/// Global return codes
typedef enum
{
	eSTATUS_COMMON_OK = 0,							///< Successful
	eSTATUS_COMMON_NO_MEMORY,						///< Allocation failed due to memory shortage
	eSTATUS_COMMON_INVALID_ARGUMENT,				///< Invalid argument provided <i>(usually NULL-pointer)</i>

	eSTATUS_LIST_INVALID_POSITION,					///< Invalid position provided
	eSTATUS_LIST_NOT_FOUND,							///< Item was not found
	eSTATUS_LIST_ENUMERATION_STOPPED,				///< Enumeration was stopped by enumerator

	eSTATUS_QUEUE_EMPTY,							///< Attempt to get an item from the empty queue

	eSTATUS_TIME_LINE_FINISHED,						///< Last event was reached

	eSTATUS_STATION_PACKET_NOT_ACCEPTED,			///< Packet did not pass destination filter

	eSTATUS_SIMULATOR_STATION_NOT_FOUND,			///< Referred station was not found
	eSTATUS_SIMULATOR_FINISHED,						///< Simulation finished

	eSTATUS_SCHEDULER_NO_PACKETS,					///< No ready packets found

	// terminator
	eSTATUS_LAST
} EStatus;

extern EStatus s_lastStatus;

const char* StatusGetMessage(EStatus status);

#endif // _WIFI_MESH_STATUS_H
