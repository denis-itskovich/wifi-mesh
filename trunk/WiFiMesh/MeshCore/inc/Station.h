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
#include "Packet.h"
#include "Settings.h"
#include "TimeLine.h"
#include "Routing.h"
#include "Scheduler.h"

typedef struct _Station Station; 	///< Station forward declaration

/** Packet scheduler add/remove handler.
 * Is called each time a packet is scheduled/removed
 * \param pStation [in] pointer to station
 * \param time [in] time, when a packet should be issued
 * \param pPacket [in] pointer to packet instance
 * \param bAdded [in] if TRUE packet is being added, otherwise removed
 * \param pUserArg [in] user defined argument
 */
typedef void (*StationSchedulerHandler)(const Station* pStation,
										double time,
										const Packet* pPacket,
										ESchedulerEvent event,
										void* pUserArg);

/** Routing entry update handler.
 * Is called each time routing entry is being added, updated or removed (expired)
 * \param pStation [in] pointer to station
 * \param destId [in] destination station id
 * \param transId [in] transit station id
 * \param expirationTime [in] entry expiration time
 * \param updateAction [in] true if entry is being added or updated
 * \param pUserArg [in] user defined argument
 */
typedef void (*StationRoutingHandler)(const Station* pStation,
									  StationId destId,
									  StationId transId,
									  double expirationTime,
									  int length,
									  ERouteEntryUpdate updateAction,
									  void *pUserArg);

/** Allocates and initializes new instance.
 * \param ppThis [out] pointer to new instance will be stored at *ppThis
 * \param velocity [in] station velocity vector
 * \param location [in] station location vector
 * \param pTimeLine [in] pointer to TimeLine instance
 * \param pSettings [in] pointer to simulation settings
 */
EStatus StationNew(Station** ppThis, Velocity velocity, Location location, TimeLine* pTimeLine, Settings* pSettings);

/** Destroys and deallocates an instance.
 * \param ppThis [in, out] *ppThis should point to valid instance
 */
EStatus StationDelete(Station** ppThis);

/** Initializes an instance.
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

/** Sets an id
 * \param pThis [in] pointer to instance
 * \param id [in] new id
 */
EStatus StationSetId(Station* pThis, StationId id);

/** Retrieves next outgoing packet
 * \param pThis [in] pointer to instance
 * \param ppPacket [out] pointer to packet will be stored at *ppPacket. NULL will be stored if no outgoing packets are available
 */
EStatus StationGetPacket(Station* pThis, Packet** ppPacket);

/** Deliveries a packet
 * \param pThis [in] pointer to instance
 * \param pPacket [in] pointer to packet
 * \return eSTATUS_PACKET_NOT_ACCEPTED if the massage was not handled by station
 * \return eSTATUS_COMMON_OK if the packet was handled
 */
EStatus StationPutPacket(Station* pThis, const Packet* pPacket);

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

/** Enqueues a packet for delayed transmit
 * \param pThis [in] pointer to instance
 * \param pPacket [in] pointer to packet to be sent
 * \param time [in] time, when the packet should be sent
 */
EStatus StationSchedulePacket(Station* pThis, Packet* pPacket, double time);

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

/** Registers scheduler handler.
 * \param pThis [in] pointer to instance
 * \param handler [in] pointer to handler
 * \param pUserArg [in] user defined argument
 */
EStatus StationRegisterSchedulerHandler(Station* pThis, StationSchedulerHandler handler, void* pUserArg);

/** Registers routing handler
 * \param pThis [in] pointer to instance
 * \param handler [in] pointer to handler
 * \param pUserArg [in] user defined argument
 */
EStatus StationRegisterRoutingHandler(Station* pThis, StationRoutingHandler handler, void* pUserArg);

#endif //_WIFI_MESH_STATION_H
