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

#include "Status.h"
#include <wchar.h>

#define STATUS_MESSAGE(msg) { msg, L ## msg }
#define DECLARE_STATUS_MESSAGES(name, count) \
	static struct \
	{ \
		const char* amsg; \
		const wchar_t* wmsg; \
	} name[(count) + 1] =

#define _MIN(a,b) ((a) < (b) ? (a) : (b))

DECLARE_STATUS_MESSAGES(s_statusMessages, eSTATUS_LAST)
{
	// Common
	STATUS_MESSAGE("Success"),
	STATUS_MESSAGE("Out of memory"),
	STATUS_MESSAGE("Invalid argument"),

	// List
	STATUS_MESSAGE("List position is invalid"),
	STATUS_MESSAGE("List item not found"),
	STATUS_MESSAGE("List enumeration was stopped"),

	STATUS_MESSAGE("Item was not added because was not unique"),
	// Queue
	STATUS_MESSAGE("Queue is empty"),

	// TimeLine
	STATUS_MESSAGE("Time line finished"),

	// Station
	STATUS_MESSAGE("Packet was not accepted"),
	STATUS_MESSAGE("Packet was not delivered due to collision"),

	// Simulator
	STATUS_MESSAGE("Station not found"),
	STATUS_MESSAGE("Simulation finished"),
	STATUS_MESSAGE("Simulation file is corrupted"),
	STATUS_MESSAGE("Failed to open simulation file"),

	// Scheduler
	STATUS_MESSAGE("No available packets"),

	// Terminator
	STATUS_MESSAGE("Unknown error")
};

EStatus s_lastStatus;

const char* StatusGetMessage(EStatus status)
{
	return s_statusMessages[_MIN(status, eSTATUS_LAST)].amsg;
}
