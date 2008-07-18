#ifndef _WIFI_MESH_GRID_H
#define _WIFI_MESH_GRID_H

#include "Defs.h"
#include "List.h"
#include "Station.h"

typedef struct _GridItem
{
	ListHeader			list;
	Position			position;
	Velocity			velocity;
	Station				station;
} GridItem;

typedef struct _Grid
{
	Size			size;
	Size			gridStep;
	unsigned long	items;
	ListHeader*		pArray;
} Grid;

EStatus	GridCreate(Grid* pThis, Size size, Size gridStep);
EStatus	GridDestroy(Grid* pThis);
EStatus GridAddStation(Grid* pThis, Position position, Velocity velocity, Station station);
EStatus GridRemoveItem(Grid* pThis, GridItem* pItem);
EStatus GridAddItem(Grid* pThis, GridItem* pItem);
EStatus	GridIterate(Grid* pThis);
EStatus GridNextItem(Grid* pThis, GridItem** ppItem);
EStatus GridFirstItem(Grid* pThis, GridItem** ppItem);

#endif // _WIFI_MESH_GRID_H

