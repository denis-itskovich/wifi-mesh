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

/** Synchonizes a station with current time
 * Moves the station
 * \param pThis [in] pointer to instance
 * \param time [in] current time
 */
EStatus StationSynchronize(Station* pThis, double time);

/** Moves a station to new location
 * \param pThis [in] pointer to instance
 * \param newLocation [in] new location
 */
EStatus StationMoveTo(Station* pThis, Location newLocation);

/** Retrieves a station id
 * \param pThis [in] pointer to instance
 * \param pId [out] pointer to id
 */
EStatus StationGetId(Station* pThis, StationId* pId);

/** Retrieves next outbox messages
 * \param pThis [in] pointer to instance
 * \param ppMessage [out] pointer to message will be stored at *ppMessage
 */
EStatus StationGetMessage(Station* pThis, Message** ppMessage);

/** Deliveries a message
 * \param pThis [in] pointer to instance
 * \param pMessage [in] pointer to message
 */
EStatus StationPutMessage(Station* pThis, Message* pMessage);

/** Checks whether a station is adjacent to another one
 * \param pThis [in] pointer to instance
 * \param pStation [in] pointer to station to check
 * \param pIsAdjacent [out] result will be stored at *pIsAdjacent
 */
EStatus StationIsAdjacent(Station* pThis, Station* pStation, Boolean* pIsAdjacent);

#endif //_WIFI_MESH_STATION_H
