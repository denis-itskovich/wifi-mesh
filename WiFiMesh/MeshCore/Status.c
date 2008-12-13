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

	// Simulator
	STATUS_MESSAGE("Station not found"),
	STATUS_MESSAGE("Simulation finished"),

	// Terminator
	STATUS_MESSAGE("Unknown error")
};

const wchar_t* GetStatusMessageW(EStatus status)
{
	return s_statusMessages[_MIN(status, eSTATUS_LAST)].wmsg;
}

const char* GetStatusMessageA(EStatus status)
{
	return s_statusMessages[_MIN(status, eSTATUS_LAST)].amsg;
}
