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

typedef struct _Routing Routing; ///< forward declaration

/** Allocates & initializes a new instance
 * \param ppThis [out] pointer to new instance will be stored at *ppThis
 */
EStatus RoutingNew(Routing** ppThis);

/** Destroys and deallocates an instance
 * \param ppThis [in, out] *ppThis should point to valid instance
 */
EStatus RoutingDelete(Routing** ppThis);

/** Initializes an instance
 * \param pThis [in] pointer to instance
 */
EStatus RoutingInit(Routing* pThis);

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

#endif /* ROUTING_H_ */
