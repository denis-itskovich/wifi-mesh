/*********************************************************************************
WiFi Mesh Simulator PRO
Copyright (C) 2009 Denis Itskovich

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*********************************************************************************/

/**
 * @file Station.h
 *
 * Station module
 *
 * Represents a Station
 *
 * @date 1/12/2008
 * @author Denis Itskovich
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

typedef enum
{
    ePKT_EVENT_ADDED,
    ePKT_EVENT_UPDATE,
    ePKT_EVENT_REMOVED_DELIVERED,
    ePKT_EVENT_REMOVED_BROADCAST,
    ePKT_EVENT_REMOVED_TIMEOUT,
    ePKT_EVENT_REMOVED_OUTDATED,
    ePKT_EVENT_REMOVED_CLEAN
} EOutboxEvent;

/** Packet scheduler add/remove handler.
 * Is called each time a packet is scheduled/removed
 * @param pStation [in] pointer to station
 * @param time [in] time, when a packet should be issued
 * @param pPacket [in] pointer to packet instance
 * @param bAdded [in] if TRUE packet is being added, otherwise removed
 * @param pUserArg [in] user defined argument
 */
typedef void (*StationSchedulerHandler)(const Station* pStation,
										const SchedulerEntry* pEntry,
										void* pUserArg);

/** Routing entry update handler.
 * Is called each time routing entry is being added, updated or removed (expired)
 * @param pStation [in] pointer to station
 * @param destId [in] destination station id
 * @param transId [in] transit station id
 * @param expirationTime [in] entry expiration time (or next retry time for pending entry)
 * @param length [in] route length
 * @param retriesLeft [in] retries left (for pending entry)
 * @param updateAction [in] routing event
 * @param pUserArg [in] user defined argument
 */
typedef void (*StationRoutingHandler)(const Station* pStation,
									  StationId destId,
									  StationId transId,
									  double expirationTime,
									  int length,
									  int retriesLeft,
									  ERouteEntryUpdate updateAction,
									  void *pUserArg);

/** Output box events handler
 * @param pStation [in] pointer to station
 * @param pPacket [in] packet being added/removed
 * @param event [in] outbox event
 * @param retriesCount [in] updated retries count
 * @param nextRetryTime [in] next retry time
 * @param pUserArg [in] user-defined argument
 */
typedef void (*StationOutboxHandler)(const Station* pStation,
                                     const Packet* pPacket,
                                     EOutboxEvent event,
                                     int retriesCount,
                                     double nextRetryTime,
                                     void* pUserArg);


/** Allocates and initializes new instance.
 * @param ppThis [out] pointer to new instance will be stored at *ppThis
 * @param velocity [in] station velocity vector
 * @param location [in] station location vector
 * @param pTimeLine [in] pointer to TimeLine instance
 * @param pSettings [in] pointer to simulation settings
 */
EStatus StationNew(Station** ppThis, Velocity velocity, Location location, TimeLine* pTimeLine, Settings* pSettings);

/** Destroys and deallocates an instance.
 * @param ppThis [in, out] *ppThis should point to valid instance
 */
EStatus StationDelete(Station** ppThis);

/** Initializes an instance.
 * @param pThis [in] pointer to instance to initialize
 * @param velocity [in] station velocity vector
 * @param location [in] station location vector
 * @param pTimeLine [in] pointer to TimeLine instance
 * @param pSettings [in] pointer to simulation settings
 */
EStatus StationInit(Station* pThis, Velocity velocity, Location location, TimeLine* pTimeLine, Settings* pSettings);

/** Destroys an instance
 * @param pThis [in] must point to valid instance
 */
EStatus StationDestroy(Station* pThis);

/** Synchronizes a station with current time
 * @param pThis [in] pointer to instance
 * @param ppDeliveredPacket [out] packet, that was recently delivered (if any)
 */
EStatus StationSynchronize(Station* pThis, Packet** ppDeliveredPacket);

/** Advances a station
 * @param pThis [in] pointer to instance
 */
EStatus StationAdvance(Station* pThis);

/** Sets a new station location
 * @param pThis [in] pointer to instance
 * @param newLocation [in] new location
 */
EStatus StationSetLocation(Station* pThis, Location newLocation);

/** Sets a new velocity
 * @param pThis [in] pointer to instance
 * @param newVelocity [in] new velocity
 */
EStatus StationSetVelocity(Station* pThis, Velocity newVelocity);

/** Retrieves a station id
 * @param pThis [in] pointer to instance
 * @param pId [out] pointer to id
 */
EStatus StationGetId(const Station* pThis, StationId* pId);

/** Sets an id
 * @param pThis [in] pointer to instance
 * @param id [in] new id
 */
EStatus StationSetId(Station* pThis, StationId id);

/** Retrieves next outgoing packet
 * @param pThis [in] pointer to instance
 * @param ppPacket [out] pointer to packet will be stored at *ppPacket. NULL will be stored if no outgoing packets are available
 */
EStatus StationTransmitPacket(Station* pThis, Packet** ppPacket);

/** Deliveries a packet
 * @param pThis [in] pointer to instance
 * @param pPacket [in] pointer to packet
 * @param ppAbortedPacket [out] aborted packet (in case of collision)
 * @return eSTATUS_PACKET_NOT_ACCEPTED if the packet was not handled by station
 * @return eSTATUS_PACKET_COLLISION if the packet was not delivered due to collision
 * @return eSTATUS_COMMON_OK if the packet was handled
 */
EStatus StationReceivePacket(Station* pThis, const Packet* pPacket, Packet** ppAbortedPacket);

/** Checks whether a station is adjacent to another one
 * @param pThis [in] pointer to instance
 * @param pStation [in] pointer to station to check
 * @param pIsAdjacent [out] result will be stored at *pIsAdjacent
 */
EStatus StationIsAdjacent(const Station* pThis, const Station* pStation, Boolean* pIsAdjacent);

/** Checks whether a station is active
 * @param pThis [in] pointer to instance
 * @param pIsActive [out] result will be stored at *pIsActive
 */
EStatus StationIsActive(const Station* pThis, Boolean* pIsActive);

/** Checks whether a station is transmitting
 * @param pThis [in] pointer to instance
 * @param pIsTxing [in] result will be stored at *pIsTxing
 */
EStatus StationIsTransmitting(const Station* pThis, Boolean* pIsTxing);

/** Enqueues a packet for delayed transmit
 * @param pThis [in] pointer to instance
 * @param pPacket [in] pointer to packet to be sent
 * @param time [in] time, when the packet should be sent
 */
EStatus StationSchedulePacket(Station* pThis, Packet* pPacket, double time);

/** Retrieves station location
 * @param pThis [in] pointer to instance
 * @param pLocation [out] location will be stored at *pLocation
 */
EStatus StationGetLocation(const Station* pThis, Location* pLocation);

/** Retrieves station velocity
 * @param pThis [in] pointer to instance
 * @param pVelocity [out] velocity will be stored at *pVelocity
 */
EStatus StationGetVelocity(const Station* pThis, Velocity* pVelocity);

/** Retrieves free buffer size
 * @param pThis [in] pointer to instance
 * @param pSize [out] size will be stored at *pSize
 */
EStatus StationGetFreeBuffer(const Station* pThis, int* pSize);

/** Resets station state
 * @param pThis [in] pointer to instance
 */
EStatus StationReset(Station* pThis);

/** Registers scheduler handler.
 * @param pThis [in] pointer to instance
 * @param handler [in] pointer to handler
 * @param pUserArg [in] user defined argument
 */
EStatus StationRegisterSchedulerHandler(Station* pThis, StationSchedulerHandler handler, void* pUserArg);

/** Registers routing handler
 * @param pThis [in] pointer to instance
 * @param handler [in] pointer to handler
 * @param pUserArg [in] user defined argument
 */
EStatus StationRegisterRoutingHandler(Station* pThis, StationRoutingHandler handler, void* pUserArg);

/** Registers outbox handler
 * @param pThis [in] pointer to instance
 * @param handler [in] outbox handler callback
 * @param pUserArg [in] user-defined argument
 */
EStatus StationRegisterOutboxHandler(Station* pThis, StationOutboxHandler handler, void* pUserArg);

/** Notifies a station about packet successful delivery
 * @param pThis [in] pointer to instance
 * @param pPacket [in] pointer to packet
 */
EStatus StationPacketDelivered(Station* pThis, const Packet* pPacket);

/** Exports a station into the file
 * @param pThis [in] pointer to instance
 * @param file [in] pointer to file
 */
EStatus StationExport(const Station* pThis, FILE* file);

/** Dumps an instance
 * @param pThis [in] pointer to instance
 */
EStatus StationDump(const Station* pThis);

#endif //_WIFI_MESH_STATION_H
