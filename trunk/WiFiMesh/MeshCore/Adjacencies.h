/**
 * \file Adjacencies.h
 *
 * Adjacencies matrix
 *
 * Allows to check whether specified station is or not adjacent to another one
 *
 * \author Denis Itskovich
 * \date 07/12/2008
 */

#ifndef WIFI_MESH_ADJACENCIES_H
#define WIFI_MESH_ADJACENCIES_H

#include "Status.h"
#include "List.h"

typedef struct _Adjacencies Adjacencies; ///< Adjacencies forward declaration

/** Allocates and initializes new instance
 * \param ppThis [out] pointer to allocated instance will be stored at *ppThis
 * \param pStations [in] stations list
 */
EStatus AdjacenciesNew(Adjacencies** ppThis, List* pStations);

/** Destroys and deallocates an instance
 * \param ppThis [in, out] *ppThis must point to valid instance
 */
EStatus AdjacenciesDelete(Adjacencies** ppThis);

/** Initializes an instance
 * \param pThis [in] pointer to instance
 * \param pStations [in] stations list
 */
EStatus AdjacenciesInit(Adjacencies* pThis, List* pStations);

/** Destroys an instance
 * \param pThis [in] pointer to valid instance
 */
EStatus AdjacenciesDestroy(Adjacencies* pThis);

/** Checks whether 2 stations are adjacent
 * \param pThis [in] pointer to instance
 * \param pStationA [in] pointer to first station
 * \param pStationB [in] pointer to second station
 * \param pIsAdjacent [out] result will be stored at *pIsAdjacent
 */
EStatus AdjacenciesIsAdjacent(Adjacencies* pThis, Station* pStationA, Station* pStationB, Boolean* pIsAdjacent);

#endif // WIFI_MESH_ADJACENCIES_H
