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
	struct
	{
		RoutingHandler	callback;
		void*			pArg;
	}			handler;
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

/** Invokes routing handler (if exists)
 * \param pThis [in] pointer to instance
 * \param pEntry [in] pointer to routing entry
 * \param flag [in] route entry update flag
 */
void RoutingInvokeHandler(Routing* pThis, RoutingEntry* pEntry, ERouteEntryUpdate flag)
{
	if (pThis->handler.callback)
	{
		pThis->handler.callback(pEntry->dstId, pEntry->transitId, pEntry->expires, pEntry->length, flag, pThis->handler.pArg);
	}
}

Boolean RoutingCleaner(RoutingEntry* pEntry, Routing* pThis)
{
	double time;
	TimeLineGetTime(pThis->pTimeLine, &time);
	if (pEntry->expires > time) return TRUE;
	RoutingInvokeHandler(pThis, pEntry, eROUTE_REMOVE);
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
	CHECK(ListCleanUp(pThis->pEntries, (ItemFilter)&RoutingCleaner, NULL));
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

	CHECK(ListGetValue(pListEntry, &pRouteEntry));

	if (pRouteEntry->length >= pMessage->nodesCount)
	{
		pRouteEntry->length = pMessage->nodesCount;
		pRouteEntry->transitId = pMessage->transitSrcId;
		CHECK(RoutingGetExpirationTime(pThis, &pRouteEntry->expires));
		RoutingInvokeHandler(pThis, pRouteEntry, eROUTE_UPDATE);
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
	RoutingInvokeHandler(pThis, pEntry, eROUTE_ADD);
	return ListPushBack(pThis->pEntries, pEntry);
}

EStatus RoutingLookFor(Routing* pThis, StationId dstId, StationId* pTransitId)
{
	ListEntry* pEntry;
	VALIDATE_ARGUMENTS(pThis && pTransitId);
	return ListFind(pThis->pEntries, &pEntry, (ItemComparator)&RoutingFinder, &dstId, pTransitId);
}

EStatus RoutingSynchronize(Routing* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	return ListCleanUp(pThis->pEntries, (ItemFilter)&RoutingCleaner, pThis);
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

EStatus RoutingClear(Routing* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	CHECK(ListCleanUp(pThis->pEntries, (ItemFilter)&RoutingCleaner, NULL));
	return eSTATUS_COMMON_OK;
}

EStatus RoutingRegisterHandler(Routing* pThis, RoutingHandler handler, void* pUserArg)
{
	VALIDATE_ARGUMENTS(pThis);
	pThis->handler.callback = handler;
	pThis->handler.pArg = pUserArg;

	return eSTATUS_COMMON_OK;
}
