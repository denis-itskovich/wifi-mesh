#include "Status.h"

#define STATUS_MESSAGE(msg) { msg, L ## msg }
#define DECLARE_STATUS_MESSAGES(name, count)	\
	struct										\
	{											\
		const char* amsg;						\
		const wchar_t* wmsg;					\
	} name[(count) + 1] =

#define _MIN(a,b) ((a) < (b) ? (a) : (b))

DECLARE_STATUS_MESSAGES(gStatusMessages, eSTATUS_LAST)
{
	STATUS_MESSAGE("Success"),
	STATUS_MESSAGE("Out of memory"),
	STATUS_MESSAGE("Grid pointer is invalid"),
	STATUS_MESSAGE("Grid array is corrupted or uninitialized"),
	STATUS_MESSAGE("Grid is empty"),
	STATUS_MESSAGE("Last grid item reached"),
	STATUS_MESSAGE("Position is out of grid's dimensions"),
	STATUS_MESSAGE("List header is invalid"),
	STATUS_MESSAGE("List item is invalid"),
	STATUS_MESSAGE("Unknown error")
};

const wchar_t* GetStatusMessageW(EStatus status)
{
	return gStatusMessages[_MIN(status, eSTATUS_LAST)].wmsg;
}

const char* GetStatusMessageA(EStatus status)
{
	return gStatusMessages[_MIN(status, eSTATUS_LAST)].amsg;
}
