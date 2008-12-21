/**
 * \file Routing.c
 *
 * Project: MeshCore
 * (C) Copyright 2008
 * \date 20/12/2008
 * \author Denis Itskovich
 */

#include "Routing.h"
#include "Macros.h"
#include "Globals.h"

typedef struct _RoutingEntry
{
	StationId	dstId;
	StationId	transitId;
	double		expires;
	unsigned	length;
} RoutingEntry;

struct _Routing
{
	List	entries;
};

/** Adds a route
 * \param pThis [in] pointer to instance
 * \param dstId [in] destination station id
 * \param transitId [in] adjacent transit station id
 * \param length [in] estimated route length
 */
EStatus RoutingAddRoute(Routing* pThis, StationId dstId, StationId transitId, unsigned length);

Boolean RoutingCleaner(RoutingEntry* pEntry, double* pTime)
{
	if (pEntry->expires > *pTime) return TRUE;
	DELETE(pEntry);
	return FALSE;
}

Comparison RoutingFinder(RoutingEntry* pEntry, StationId* pId, StationId* pTransitId)
{
	if (pEntry->dstId == *pId)
	{
		pTransitId = pEntry->transitId;
		return EQUAL;
	}

	return (pEntry->dstId < *pId) ? LESS : GREAT;
}

EStatus RoutingNew(Routing** ppThis)
{
	CONSTRUCT(ppThis, Routing);
}

EStatus RoutingDelete(Routing** ppThis)
{
	DESTRUCT(ppThis, Routing);
}

EStatus RoutingInit(Routing* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	CLEAR(pThis);
	return ListInit(&pThis->entries);
}

EStatus RoutingDestroy(Routing* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	ListDestroy(&pThis->entries);
}

EStatus RoutingHandleMessage(Routing* pThis, Message* pMessage)
{
	StationId dstId, transitId;
	ListPosition* pPosition;
	RoutingEntry* pEntry;

	VALIDATE_ARGUMENTS(pThis && pMessage);

	dstId = pMessage->originalSrcId;

	if (eSTATUS_LIST_NOT_FOUND == ListFind(pThis, &pPosition, (ItemComparator)RoutingFinder, &dstId, &transitId))
	{
		transitId = pMessage->transitSrcId;
		return RoutingAddRoute(pThis, dstId, transitId);
	}

	CHECK(ListGetValue(&pPosition, (void**)&pEntry));

	if (pEntry->length >= pMessage->nodesCount)
	{
		pEntry->length = pMessage->nodesCount;
		pEntry->transitId = pMessage->transitSrcId;
		pEntry->expires = GlobalsGetRoutingEntryExpirationTime();
	}

	return eSTATUS_COMMON_OK;
}

EStatus RoutingAddRoute(Routing* pThis, StationId dstId, StationId transitId, unsigned length)
{
	RoutingEntry* pEntry;
	VALIDATE_ARGUMENTS(pThis);
	pEntry = NEW(RoutingEntry);
	pEntry->dstId = dstId;
	pEntry->transitId = transitId;
	pEntry->length = length;
	pEntry->expires = GlobalsGetRoutingEntryExpirationTime();
	return ListInsert(&pThis->entries, pEntry);
}

EStatus RoutingLookFor(Routing* pThis, StationId dstId, StationId* pTransitId)
{
	ListPosition* pPosition;
	VALIDATE_ARGUMENTS(pThis && pTransitId);
	return ListFind(pThis, &pPosition, (ItemComparator)RoutingFinder, &dstId, pTransitId);
}

EStatus RoutingSynchronize(Routing* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	double time = GlobalsGetTime();
	return ListCleanUp(&pThis->entries, (ItemFilter)&RoutingCleaner, &time);
}
