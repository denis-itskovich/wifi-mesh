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
 * @file Routing.h
 *
 * Project: MeshCore
 * (C) Copyright 2008
 *
 * @date 20/12/2008
 * @author Denis Itskovich
 */

#ifndef ROUTING_H_
#define ROUTING_H_

#include "Status.h"
#include "CommonTypes.h"
#include "Packet.h"
#include "Settings.h"
#include "TimeLine.h"

typedef struct _Routing Routing;    ///< forward declaration of Routing

/// Routing entry modifying flag
typedef enum _ERouteEntryUpdate
{
	eROUTE_ADD,			///< Entry is being added
	eROUTE_UPDATE,		///< Entry is being modified
	eROUTE_REMOVE		///< Entry is being removed
} ERouteEntryUpdate;


/** Route entry handler add/update/remove handler.
 * Is called each time a routing entry is being added, updated or removed (expired)
 * @param pStation [in] pointer to station
 * @param destId [in] destination station id
 * @param transId [in] transit station id
 * @param expirationTime [in] entry expiration time (or next retry time for pending entry)
 * @param length [in] route length
 * @param retriesLeft [in] retries left (for pending entry)
 * @param updateAction [in] true if entry is being added or updated
 * @param pUserArg [in] user defined argument
 */
typedef void (*RoutingHandler)( StationId destId,
                                StationId transId,
                                double expirationTime,
                                int length,
                                int retriesLeft,
                                ERouteEntryUpdate updateAction,
                                void *pUserArg);

/** Allocates & initializes a new instance
 * @param ppThis [out] pointer to new instance will be stored at *ppThis
 * @param pSettings [in] pointer to settings instance
 * @param pTimeLine [in] pointer to time line instance
 */
EStatus RoutingNew(Routing** ppThis, Settings* pSettings, TimeLine* pTimeLine);

/** Destroys and deallocates an instance
 * @param ppThis [in, out] *ppThis should point to valid instance
 */
EStatus RoutingDelete(Routing** ppThis);

/** Initializes an instance
 * @param pThis [in] pointer to instance
 * @param pSettings [in] pointer to settings instance
 * @param pTimeLine [in] pointer to time line instance
 */
EStatus RoutingInit(Routing* pThis, Settings* pSettings, TimeLine* pTimeLine);

/** Destroys an instance
 * @param pThis [in] pointer to valid instance
 */
EStatus RoutingDestroy(Routing* pThis);

/** Handles a packet.
 * Updates a routing table according to information found in a packet
 * @param pThis [in] pointer to instance
 * @param pPacket [in] pointer to packet
 */
EStatus RoutingHandlePacket(Routing* pThis, const Packet* pPacket);

/** Adds pending route
 * @param pThis [in] pointer to instance
 * @param dstId [in] destination id
 */
EStatus RoutingAddPending(Routing* pThis, StationId dstId);

/** Looks for a route to destination id
 * @param pThis [in] pointer to instance
 * @param dstId [in] destination station id
 * @param pTransitId [out] transit station id will be stored in *pTransitId
 * @param pHopsCount [out] route length
 * @return eSTATUS_COMMON_OK if the route was found and *pTransit was filled with valid station id
 * @return eSTATUS_LIST_NOT_FOUND if the route was not found
 */
EStatus RoutingLookFor(Routing* pThis, StationId dstId, StationId* pTransitId, unsigned* pHopsCount);

/** Synchronizes a routing table with current time
 * Actually removes expired entries
 * @param pThis [in] pointer to instance
 */
EStatus RoutingSynchronize(Routing* pThis);

/** Removes all entries, whose transit is a specified one
 * @param pThis [in] pointer to instance
 * @param transit [in] transit station id
 */
EStatus RoutingRemoveTransit(Routing* pThis, StationId transit);

/** Clear routing table
 * @param pThis [in] pointer to instance
 */
EStatus RoutingClear(Routing* pThis);

/** Sets routing handler
 * @param pThis [in] pointer to instance
 * @param handler [in] handler
 * @param pUserArg [in] user defined argument
 */
EStatus RoutingRegisterHandler(Routing* pThis, RoutingHandler handler, void* pUserArg);

/** Dumps an instance
 * @param pThis [in] pointer to instance
 */
EStatus RoutingDump(const Routing* pThis);

#endif /* ROUTING_H_ */
