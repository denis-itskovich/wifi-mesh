/**
 * \file Routing.h
 *
 * Project: MeshCore
 * (C) Copyright 2008
 * \date 20/12/2008
 * \author denis
 */

#ifndef ROUTING_H_
#define ROUTING_H_

#include "Status.h"
#include "CommonTypes.h"
#include "Message.h"
#include "Settings.h"
#include "TimeLine.h"

typedef struct _Routing Routing; ///< forward declaration

/// Routing entry modifying flag
typedef enum _ERouteEntryUpdate
{
	eROUTE_ADD,			///< Entry is being added
	eROUTE_UPDATE,		///< Entry is being modified
	eROUTE_REMOVE		///< Entry is being removed
} ERouteEntryUpdate;

/** Route entry handler add/update/remove handler.
 * Is called each time a routing entry is being added, updated or removed (expired)
 * \param destId [in] destination station id
 * \param transId [in] transit station id
 * \param expirationTime [in] entry expiration time
 * \param updateAction [in] true if entry is being added or updated
 * \param pUserArg [in] user defined argument
 */
typedef void (*RoutingHandler)(StationId destId, StationId transId, double expirationTime, int length, ERouteEntryUpdate updateAction, void *pUserArg);

/** Allocates & initializes a new instance
 * \param ppThis [out] pointer to new instance will be stored at *ppThis
 * \param pSettings [in] pointer to settings instance
 * \param pTimeLine [in] pointer to time line instance
 */
EStatus RoutingNew(Routing** ppThis, Settings* pSettings, TimeLine* pTimeLine);

/** Destroys and deallocates an instance
 * \param ppThis [in, out] *ppThis should point to valid instance
 */
EStatus RoutingDelete(Routing** ppThis);

/** Initializes an instance
 * \param pThis [in] pointer to instance
 * \param pSettings [in] pointer to settings instance
 * \param pTimeLine [in] pointer to time line instance
 */
EStatus RoutingInit(Routing* pThis, Settings* pSettings, TimeLine* pTimeLine);

/** Destroys an instance
 * \param pThis [in] pointer to valid instance
 */
EStatus RoutingDestroy(Routing* pThis);

/** Handles a message.
 * Updates a routing table according to information found in a message
 * \param pThis [in] pointer to instance
 * \param pMessage [in] pointer to message
 */
EStatus RoutingHandleMessage(Routing* pThis, Message* pMessage);

/** Looks for a route to destination id
 * \param pThis [in] pointer to instance
 * \param dstId [in] destination station id
 * \param pTransitId [out] transit station id will be stored in *pTransitId
 * \return eSTATUS_COMMON_OK if the route was found and *pTransit was filled with valid station id
 * \return eSTATUS_LIST_NOT_FOUND if the route was not found
 */
EStatus RoutingLookFor(Routing* pThis, StationId dstId, StationId* pTransitId);

/** Synchronizes a routing table with current time
 * Actually removes expired entries
 * \param pThis [in] pointer to instance
 */
EStatus RoutingSynchronize(Routing* pThis);

/** Clear routing table
 * \param pThis [in] pointer to instance
 */
EStatus RoutingClear(Routing* pThis);

/** Sets routing handler
 * \param pThis [in] pointer to instance
 * \param handler [in] handler
 * \param pUserArg [in] user defined argument
 */
EStatus RoutingRegisterHandler(Routing* pThis, RoutingHandler handler, void* pUserArg);

#endif /* ROUTING_H_ */
