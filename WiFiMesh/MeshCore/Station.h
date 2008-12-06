#ifndef _WIFI_MESH_STATION_H
#define _WIFI_MESH_STATION_H

#include "Protocol.h"

typedef struct _Position
{
	double x, y;
} Location, Velocity;

typedef struct _Station Station;

EStatus StationCreate(Station** ppThis, Velocity velocity, Location location);
EStatus StationDestroy(Station** ppThis);
EStatus StationMove(Station* pThis, unsigned timeDelta);
EStatus StationGetId(Station* pThis, StationId* pId);

#endif //_WIFI_MESH_STATION_H
