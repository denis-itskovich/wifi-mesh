#ifndef _WIFI_MESH_STATUS_H
#define _WIFI_MESH_STATUS_H

#include <wchar.h>

typedef enum
{
	eSTATUS_COMMON_OK = 0,
	eSTATUS_COMMON_NO_MEMORY,
	eSTATUS_COMMON_INVALID_ARGUMENT,

	eSTATUS_LIST_INVALID_HEADER,
	eSTATUS_LIST_INVALID_ITEM,
	eSTATUS_LIST_NOT_FOUND,

	eSTATUS_TIME_LINE_FINISHED,

	eSTATUS_SIMULATOR_STATION_NOT_FOUND,
	eSTATUS_SIMULATOR_FINISHED,

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
