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

#include "CommonTypes.h"
#include "Message.h"
#include "Settings.h"
#include "TimeLine.h"

typedef struct _Station Station; 	///< Station forward declaration

/** Allocates and initializes new instance
 * \param ppThis [out] pointer to new instance will be stored at *ppThis
 * \param velocity [in] station velocity vector
 * \param location [in] station location vector
 * \param pTimeLine [in] pointer to TimeLine instance
 * \param pSettings [in] pointer to simulation settings
 */
EStatus StationNew(Station** ppThis, Velocity velocity, Location location, TimeLine* pTimeLine, Settings* pSettings);

/** Destroys and deallocates an instance
 * \param ppThis [in, out] *ppThis should point to valid instance
 */
EStatus StationDelete(Station** ppThis);

/** Initializes an instance
 * \param pThis [in] pointer to instance to initialize
 * \param velocity [in] station velocity vector
 * \param location [in] station location vector
 * \param pTimeLine [in] pointer to TimeLine instance
 * \param pSettings [in] pointer to simulation settings
 */
EStatus StationInit(Station* pThis, Velocity velocity, Location location, TimeLine* pTimeLine, Settings* pSettings);

/** Destroys an instance
 * \param pThis [in] must point to valid instance
 */
EStatus StationDestroy(Station* pThis);

/** Synchronizes a station with current time
 * Moves the station
 * \param pThis [in] pointer to instance
 * \param timeDelta [in] time, passed since last update
 */
EStatus StationSynchronize(Station* pThis, double timeDelta);

/** Sets a new station location
 * \param pThis [in] pointer to instance
 * \param newLocation [in] new location
 */
EStatus StationSetLocation(Station* pThis, Location newLocation);

/** Sets a new velocity
 * \param pThis [in] pointer to instance
 * \param newVelocity [in] new velocity
 */
EStatus StationSetVelocity(Station* pThis, Velocity newVelocity);

/** Retrieves a station id
 * \param pThis [in] pointer to instance
 * \param pId [out] pointer to id
 */
EStatus StationGetId(const Station* pThis, StationId* pId);

/** Retrieves next outgoing message
 * \param pThis [in] pointer to instance
 * \param ppMessage [out] pointer to message will be stored at *ppMessage. NULL will be stored if no outgoing messages are available
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
EStatus StationIsAdjacent(const Station* pThis, const Station* pStation, Boolean* pIsAdjacent);

/** Checks whether a station is active
 * \param pThis [in] pointer to instance
 * \param pIsActive [out] result will be stored at *pIsActive
 */
EStatus StationIsActive(const Station* pThis, Boolean* pIsActive);

/** Checks whether a station is transmitting
 * \param pThis [in] pointer to instance
 * \param pIsTxing [in] result will be stored at *pIsTxing
 */
EStatus StationIsTransmitting(const Station* pThis, Boolean* pIsTxing);

/** Enqueues a message for delayed transmit
 * \param pThis [in] pointer to instance
 * \param pMessage [in] pointer to message to be sent
 * \param time [in] time, when the message should be sent
 */
EStatus StationScheduleMessage(Station* pThis, Message* pMessage, double time);

/** Retrieves station location
 * \param pThis [in] pointer to instance
 * \param pLocation [out] location will be stored at *pLocation
 */
EStatus StationGetLocation(const Station* pThis, Location* pLocation);

/** Retrieves station velocity
 * \param pThis [in] pointer to instance
 * \param pVelocity [out] velocity will be stored at *pVelocity
 */
EStatus StationGetVelocity(const Station* pThis, Velocity* pVelocity);

/** Resets station state
 * \param pThis [in] pointer to instance
 */
EStatus StationReset(Station* pThis);

#endif //_WIFI_MESH_STATION_H
