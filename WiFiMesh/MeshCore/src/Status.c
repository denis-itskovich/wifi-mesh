#include "Status.h"

#define STATUS_MESSAGE(msg) { msg, L ## msg }
#define DECLARE_STATUS_MESSAGES(name, count)	\
	struct										\
	{											\
		const char* amsg;						\
		const wchar_t* wmsg;					\
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

	// Queue
	STATUS_MESSAGE("Queue is empty"),

	// TimeLine
	STATUS_MESSAGE("Time line finished"),

	// Station
	STATUS_MESSAGE("Message was not accepted"),

	// Simulator
	STATUS_MESSAGE("Station not found"),
	STATUS_MESSAGE("Simulation finished"),

	// Scheduler
	STATUS_MESSAGE("No available messages"),

	// Terminator
	STATUS_MESSAGE("Unknown error")
};

EStatus s_lastStatus;

const char* StatusGetMessage(EStatus status)
{
	return s_statusMessages[_MIN(status, eSTATUS_LAST)].amsg;
}

const wchar_t* GetStatusMessageW(EStatus status)
{
	return s_statusMessages[_MIN(status, eSTATUS_LAST)].wmsg;
}

const char* GetStatusMessageA(EStatus status)
{
	return s_statusMessages[_MIN(status, eSTATUS_LAST)].amsg;
}
