#ifndef _WIFI_MESH_DEFS_H
#define _WIFI_MESH_DEFS_H


typedef struct _Position
{
	double x, y;
} Position, Velocity;

typedef struct _Size
{
	unsigned long x, y;
} Size;

typedef enum
{
	eSTATUS_COMMON_OK = 0,
	eSTATUS_COMMON_NO_MEMORY,
	eSTATUS_COMMON_INVALID_ARGUMENT,
	eSTATUS_GRID_INVALID_PTR,
	eSTATUS_GRID_INVALID_ARRAY,
	eSTATUS_GRID_EMPTY,
	eSTATUS_GRID_LAST_ITEM,
	eSTATUS_GRID_OUT_OF_RANGE,
	eSTATUS_LIST_INVALID_HEADER,
	eSTATUS_LIST_INVALID_ITEM,
} EStatus;

#endif // _WIFI_MESH_DEFS_H
