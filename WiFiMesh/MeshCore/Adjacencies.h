/*
 * Adjacencies.h
 *
 *  Created on: 07/12/2008
 *      Author: denis
 */

#ifndef WIFI_MESH_ADJACENCIES_H
#define WIFI_MESH_ADJACENCIES_H

#include "Status.h"
#include "List.h"

typedef struct _Adjacencies Adjacencies;

EStatus AdjacenciesCreate(Adjacencies** ppThis, List* pStations);
EStatus AdjacenciesDestroy(Adjacencies** ppThis, List* pStations);
EStatus AdjacenciesIsAdjacent(Adjacencies* pThis, Station* pStationA, Station* pStationB, Boolean* pIsAdjacent);

#endif // WIFI_MESH_ADJACENCIES_H
