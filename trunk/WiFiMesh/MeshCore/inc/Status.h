/*********************************************************************************
WiFi Mesh Simulator PRO
Copyright (C) 2009 Denis Itskovich

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*********************************************************************************/

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

	eSTATUS_SORTED_LIST_ALREADY_EXISTS,             ///< Could not add item into sorted list because an item was not unique

	eSTATUS_QUEUE_EMPTY,							///< Attempt to get an item from the empty queue

	eSTATUS_TIME_LINE_FINISHED,						///< Last event was reached

	eSTATUS_STATION_PACKET_NOT_ACCEPTED,			///< Packet did not pass destination filter
	eSTATUS_STATION_PACKET_COLLISION,               ///< Packet did not received due to collision

	eSTATUS_SIMULATOR_STATION_NOT_FOUND,			///< Referred station was not found
	eSTATUS_SIMULATOR_FINISHED,						///< Simulation finished

	eSTATUS_SCHEDULER_NO_PACKETS,					///< No ready packets found

	// terminator
	eSTATUS_LAST
} EStatus;

extern EStatus s_lastStatus;

const char* StatusGetMessage(EStatus status);

#endif // _WIFI_MESH_STATUS_H
