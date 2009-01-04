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
#include "List.h"

typedef struct _RoutingEntry
{
	StationId	dstId;
	StationId	transitId;
	double		expires;
	unsigned	length;
} RoutingEntry;

struct _Routing
{
	List*		pEntries;
	Settings*	pSettings;
	TimeLine*	pTimeLine;
};

/** Adds a route
 * \param pThis [in] pointer to instance
 * \param dstId [in] destination station id
 * \param transitId [in] adjacent transit station id
 * \param length [in] estimated route length
 */
EStatus RoutingAddRoute(Routing* pThis, StationId dstId, StationId transitId, unsigned length);

/** Calculates entry expiration time
 * \param pThis [in] pointer to instance
 * \param pTime [out] routing entry expiration time will be stored at *pTime
 */
EStatus RoutingGetExpirationTime(Routing* pThis, double* pTime);

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
		*pTransitId = pEntry->transitId;
		return EQUAL;
	}

	return (pEntry->dstId < *pId) ? LESS : GREAT;
}

EStatus RoutingNew(Routing** ppThis, Settings* pSettings, TimeLine* pTimeLine)
{
	CONSTRUCT(ppThis, Routing, pSettings, pTimeLine);
}

EStatus RoutingDelete(Routing** ppThis)
{
	DESTRUCT(ppThis, Routing);
}

EStatus RoutingInit(Routing* pThis, Settings* pSettings, TimeLine* pTimeLine)
{
	VALIDATE_ARGUMENTS(pThis && pSettings && pTimeLine);
	CLEAR(pThis);
	pThis->pSettings = pSettings;
	pThis->pTimeLine = pTimeLine;
	return ListNew(&pThis->pEntries);
}

EStatus RoutingDestroy(Routing* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	return ListDelete(&pThis->pEntries);
}

EStatus RoutingHandleMessage(Routing* pThis, Message* pMessage)
{
	StationId dstId, transitId;
	ListEntry* pListEntry;
	RoutingEntry* pRouteEntry;

	VALIDATE_ARGUMENTS(pThis && pMessage);

	dstId = pMessage->originalSrcId;

	if (eSTATUS_LIST_NOT_FOUND == ListFind(pThis->pEntries, &pListEntry, (ItemComparator)&RoutingFinder, &dstId, &transitId))
	{
		transitId = pMessage->transitSrcId;
		return RoutingAddRoute(pThis, dstId, transitId, pMessage->nodesCount);
	}

	CHECK(ListGetValue(pListEntry, (void**)&pRouteEntry));

	if (pRouteEntry->length >= pMessage->nodesCount)
	{
		pRouteEntry->length = pMessage->nodesCount;
		pRouteEntry->transitId = pMessage->transitSrcId;
		CHECK(RoutingGetExpirationTime(pThis, &pRouteEntry->expires));
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
	CHECK(RoutingGetExpirationTime(pThis, &pEntry->expires));
	return ListInsert(pThis->pEntries, pEntry);
}

EStatus RoutingLookFor(Routing* pThis, StationId dstId, StationId* pTransitId)
{
	ListEntry* pEntry;
	VALIDATE_ARGUMENTS(pThis && pTransitId);
	return ListFind(pThis, &pEntry, (ItemComparator)&RoutingFinder, &dstId, pTransitId);
}

EStatus RoutingSynchronize(Routing* pThis)
{
	double time;
	VALIDATE_ARGUMENTS(pThis);
	TimeLineGetTime(pThis->pTimeLine, &time);
	return ListCleanUp(pThis->pEntries, (ItemFilter)&RoutingCleaner, &time);
}

EStatus RoutingGetExpirationTime(Routing* pThis, double* pTime)
{
	double time;
	double ttl;
	VALIDATE_ARGUMENTS(pThis && pTime);
	CHECK(TimeLineGetTime(pThis->pTimeLine, &time));
	CHECK(SettingsGetRoutingTTL(pThis->pSettings, &ttl));
	*pTime = time + ttl;
	return eSTATUS_COMMON_OK;
}
