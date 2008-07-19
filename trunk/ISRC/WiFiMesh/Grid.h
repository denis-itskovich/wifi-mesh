#ifndef _WIFI_MESH_GRID_H
#define _WIFI_MESH_GRID_H

#include "Defs.h"
#include "Status.h"
#include "Station.h"

// forward declaration
struct _GridItem;
struct _Grid;

typedef struct _GridItem GridItem;
typedef struct _Grid Grid;

EStatus	GridCreate(Grid** ppThis, Size size);
EStatus	GridDestroy(Grid** ppThis);

EStatus GridAddStation(Grid* pThis, Position position, Velocity velocity, Station station);

EStatus GridRemoveItem(Grid* pThis, GridItem* pItem);
EStatus GridAddItem(Grid* pThis, GridItem* pItem);

EStatus	GridIterate(Grid* pThis);

EStatus GridNextItem(Grid* pThis, GridItem** ppItem);
EStatus GridFirstItem(Grid* pThis, GridItem** ppItem);

EStatus GridGetSize(Grid* pThis, Size* pSize);
EStatus GridGetItemsCount(Grid* pThis, int* pItems);
EStatus GridIsValid(Grid* pThis);

EStatus GridItemGetPosition(GridItem* pItem, Position* pPosition);
EStatus GridItemGetVelocity(GridItem* pItem, Velocity* pVelocity);
EStatus GridItemGetStation(GridItem* pItem, Station* pStation);

#endif // _WIFI_MESH_GRID_H
