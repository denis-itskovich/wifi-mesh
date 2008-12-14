/**
 * \file Station.h
 *
 * Station module
 *
 * Represents a Station
 *
 * \date 1/12/2008
 * \author Denis Itskovich
 */

#ifndef _WIFI_MESH_STATION_H
#define _WIFI_MESH_STATION_H

#include "Protocol.h"
#include "Status.h"

typedef struct _Station Station; 	///< Station forward declaration

/** Allocates and initializes new instance
 * \param ppThis [out] pointer to new instance will be stored at *ppThis
 * \param velocity [in] station velocity vector
 * \param location [in] station location vector
 */
EStatus StationNew(Station** ppThis, Velocity velocity, Location location);

/** Destroys and deallocates an instance
 * \param ppThis [in, out] *ppThis should point to valid instance
 */
EStatus StationDelete(Station** ppThis);

/** Initializes an instance
 * \param pThis [in] pointer to instance to initialize
 * \param velocity [in] station velocity vector
 * \param location [in] station location vector
 */
EStatus StationInit(Station* pThis, Velocity velocity, Location location);

/** Destroys an instance
 * \param pThis [in] must point to valid instance
 */
EStatus StationDestroy(Station* pThis);

/** Moves a station according to a time, passed since last move
 * \param pThis [in] pointer to instance
 * \param timeDelta [in] time, passed since last update
 */
EStatus StationMove(Station* pThis, unsigned timeDelta);

/** Retrieves a station id
 * \param pThis [in] pointer to instance
 * \param pId [out] pointer to id
 */
EStatus StationGetId(Station* pThis, StationId* pId);

#endif //_WIFI_MESH_STATION_H
