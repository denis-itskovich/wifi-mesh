#ifndef _WIFI_MESH_STATUS_H
#define _WIFI_MESH_STATUS_H

/**
 * \file Status.h
 *
 * Defines status return codes
 *
 * \author Denis Itskovich
 * \date   1/12/2008
 */

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

	eSTATUS_SIMULATOR_STATION_NOT_FOUND,			///< Referred station was not found
	eSTATUS_SIMULATOR_FINISHED,						///< Simulation finished

	// terminator
	eSTATUS_LAST
} EStatus;

const wchar_t* GetStatusMessageW(EStatus status);
const char* GetStatusMessageA(EStatus status);

#ifdef UNICODE
	#define GetStatusMessage GetStatusMessageW
#else
	#define GetStatusMessage GetStatusMessageA
#endif // UNICODE

#endif // _WIFI_MESH_STATUS_H
