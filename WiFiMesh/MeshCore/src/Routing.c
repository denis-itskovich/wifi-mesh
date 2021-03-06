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
 * @file Routing.c
 *
 * Project: MeshCore
 * (C) Copyright 2008
 * @date 20/12/2008
 * @author Denis Itskovich
 */

#include "../inc/Routing.h"
#include "../inc/Macros.h"
#include "../inc/List.h"

int __counter_Routing = 0;
int __counter_RoutingEntry = 0;

typedef struct _RoutingEntry
{
	StationId  dstId;
	StationId  transitId;
	double     expires;
	unsigned   length;
	int        retriesLeft;
} RoutingEntry;

struct _Routing
{
	List*      pEntries;
	Settings*  pSettings;
	TimeLine*  pTimeLine;
	StationId  deprecatedId;
	struct
	{
		RoutingHandler	callback;
		void*			pArg;
	}			handler;
};

/** Adds a route
 * @param pThis [in] pointer to instance
 * @param dstId [in] destination station id
 * @param transitId [in] adjacent transit station id
 * @param length [in] estimated route length
 */
EStatus RoutingAddRoute(Routing* pThis, StationId dstId, StationId transitId, unsigned length);

/** Calculates entry expiration time
 * @param pThis [in] pointer to instance
 * @param pTime [out] routing entry expiration time will be stored at *pTime
 */
EStatus RoutingGetExpirationTime(Routing* pThis, double* pTime);

/** Calculates retry time
 * @param pThis [in] pointer to instance
 * @param pTimeout [out] routing entry expiration time will be stored at *pTimeout
 */
EStatus RoutingGetRetryTime(Routing* pThis, double* pTime);

/** Invokes routing handler (if exists)
 * @param pThis [in] pointer to instance
 * @param pEntry [in] pointer to routing entry
 * @param flag [in] route entry update flag
 */
void RoutingInvokeHandler(Routing* pThis, RoutingEntry* pEntry, ERouteEntryUpdate flag)
{
	if (pThis->handler.callback)
	{
		pThis->handler.callback(pEntry->dstId,
		                        pEntry->transitId,
		                        pEntry->expires,
		                        pEntry->length,
		                        pEntry->retriesLeft,
		                        flag,
		                        pThis->handler.pArg);
	}
}

Boolean RoutingEraser(RoutingEntry* pEntry, Routing* pThis);
Boolean RoutingTransitCleaner(RoutingEntry* pEntry, Routing* pThis);
Boolean RoutingCleaner(RoutingEntry* pEntry, Routing* pThis);
EStatus RoutingEraseLRUEntry(Routing* pThis);
EStatus RoutingFind(Routing* pThis, StationId dstId, RoutingEntry** ppEntry);

Boolean RoutingTransitCleaner(RoutingEntry* pEntry, Routing* pThis)
{
    if (pEntry->transitId != pThis->deprecatedId) return TRUE;
    return RoutingEraser(pEntry, pThis);
}

Boolean RoutingCleaner(RoutingEntry* pEntry, Routing* pThis)
{
	double time;
	if (pEntry->transitId == INVALID_STATION_ID) return TRUE;
	TimeLineGetTime(pThis->pTimeLine, &time);
	if (pEntry->expires > time) return TRUE;
	return RoutingEraser(pEntry, pThis);
}

Boolean RoutingEraser(RoutingEntry* pEntry, Routing* pThis)
{
	RoutingInvokeHandler(pThis, pEntry, eROUTE_REMOVE);
	DELETE(RoutingEntry, pEntry);
	return FALSE;
}

Comparison RoutingFinder(RoutingEntry* pEntry, StationId* pId, Routing* pThis)
{
	return (pEntry->dstId == *pId) ? EQUAL : LESS;
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
	RoutingClear(pThis);
	return ListDelete(&pThis->pEntries);
}

EStatus RoutingRemoveTransit(Routing* pThis, StationId transit)
{
    VALIDATE_ARGUMENTS(pThis);
    pThis->deprecatedId = transit;
    return ListCleanUp(pThis->pEntries, (ItemFilter)&RoutingTransitCleaner, pThis);
}

EStatus RoutingFind(Routing* pThis, StationId dstId, RoutingEntry** ppEntry)
{
    ListEntry* pEntry;
    VALIDATE_ARGUMENTS(pThis);
    CHECK(ListFind(pThis->pEntries, &pEntry, (ItemComparator)&RoutingFinder, &dstId, pThis));
    CHECK(ListGetValue(pEntry, ppEntry));
    CHECK(ListMoveToHead(pThis->pEntries, pEntry));
    return eSTATUS_COMMON_OK;
}

EStatus RoutingUpdateDestination(Routing* pThis, StationId dst, StationId transit, unsigned length)
{
    RoutingEntry* pRouteEntry = NULL;

    VALIDATE_ARGUMENTS(pThis);
    RoutingFind(pThis, dst, &pRouteEntry);
    if (!pRouteEntry) CHECK(RoutingAddRoute(pThis, dst, transit, length));
    else
    {
        if (pRouteEntry->length > length ||
            (pRouteEntry->length == length && pRouteEntry->transitId != transit))
        {
            pRouteEntry->length = length;
            pRouteEntry->transitId = transit;
            CHECK(RoutingGetExpirationTime(pThis, &pRouteEntry->expires));
            RoutingInvokeHandler(pThis, pRouteEntry, eROUTE_UPDATE);
        }
    }
    return eSTATUS_COMMON_OK;
}

EStatus RoutingHandlePacket(Routing* pThis, const Packet* pPacket)
{
	unsigned i;
	StationId transitId;

	VALIDATE_ARGUMENTS(pThis && pPacket);

	transitId = pPacket->header.transitSrcId;
	for (i = 0; i < pPacket->routing.length; ++i)
	{
	    CHECK(RoutingUpdateDestination(pThis, pPacket->routing.path[i], transitId, pPacket->routing.length - i));
	}

	if (pPacket->header.hopsCount > pPacket->routing.length)
	    CHECK(RoutingUpdateDestination(pThis, pPacket->header.originalSrcId, transitId, pPacket->header.hopsCount));

	return eSTATUS_COMMON_OK;
}

EStatus RoutingAddPending(Routing* pThis, StationId dstId)
{
    RoutingEntry* pEntry = NULL;
    RoutingFind(pThis, dstId, &pEntry);
    if (pEntry)
    {
        ASSERT(pEntry->retriesLeft > 0);
        --pEntry->retriesLeft;
        CHECK(RoutingGetRetryTime(pThis, &pEntry->expires));
        CHECK(TimeLineEvent(pThis->pTimeLine, pEntry->expires, NULL));
        RoutingInvokeHandler(pThis, pEntry, eROUTE_UPDATE);
    }
    else return RoutingAddRoute(pThis, dstId, INVALID_STATION_ID, (unsigned)-1);
    return eSTATUS_COMMON_OK;
}

EStatus RoutingAddRoute(Routing* pThis, StationId dstId, StationId transitId, unsigned length)
{
	RoutingEntry* pEntry;
	int tableSize, maxTableSize;

	VALIDATE_ARGUMENTS(pThis);

    CHECK(SettingsGetRoutingTableSize(pThis->pSettings, &maxTableSize));
    CHECK(ListGetCount(pThis->pEntries, &tableSize));
    while (tableSize-- >= maxTableSize)
    {
        CHECK(RoutingEraseLRUEntry(pThis));
    }

	pEntry = NEW(RoutingEntry);
	pEntry->dstId = dstId;
	pEntry->transitId = transitId;
	pEntry->length = length;
	pEntry->retriesLeft = 0;

    if (transitId == INVALID_STATION_ID)
    {
        CHECK(RoutingGetRetryTime(pThis, &pEntry->expires));
        CHECK(TimeLineEvent(pThis->pTimeLine, pEntry->expires, NULL));
        CHECK(SettingsGetRouteRetryThreshold(pThis->pSettings, &pEntry->retriesLeft));
    }
    else CHECK(RoutingGetExpirationTime(pThis, &pEntry->expires));

    RoutingInvokeHandler(pThis, pEntry, eROUTE_ADD);
    return ListPushFront(pThis->pEntries, pEntry);
}

EStatus RoutingLookFor(Routing* pThis, StationId dstId, StationId* pTransitId, unsigned* pHopsCount)
{
	RoutingEntry* pRouteEntry;
	double time;
	CHECK(RoutingFind(pThis, dstId, &pRouteEntry));
	CHECK(TimeLineGetTime(pThis->pTimeLine, &time));
	if ((pRouteEntry->transitId == INVALID_STATION_ID) &&
	    (pRouteEntry->retriesLeft > 0) &&
	    (pRouteEntry->expires <= time))
	    return eSTATUS_LIST_NOT_FOUND;
	if (pHopsCount) *pHopsCount = pRouteEntry->length;
	if (pTransitId) *pTransitId = pRouteEntry->transitId;
	return eSTATUS_COMMON_OK;
}

EStatus RoutingEraseLRUEntry(Routing* pThis)
{
    RoutingEntry* pRouteEntry;
    ListEntry* pListEntry;
    VALIDATE_ARGUMENTS(pThis);
    CHECK(ListGetTail(pThis->pEntries, &pListEntry));
    CHECK(ListGetValue(pListEntry, &pRouteEntry));
    RoutingEraser(pRouteEntry, pThis);
    return ListRemove(pThis->pEntries, pListEntry);
}

EStatus RoutingSynchronize(Routing* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	return ListCleanUp(pThis->pEntries, (ItemFilter)&RoutingCleaner, pThis);
}

EStatus RoutingGetRetryTime(Routing* pThis, double* pTime)
{
	double time;
	double timeout;
	VALIDATE_ARGUMENTS(pThis && pTime);
	CHECK(TimeLineGetTime(pThis->pTimeLine, &time));
	CHECK(SettingsGetRouteRetryTimeout(pThis->pSettings, &timeout));

    // VARIANCE(timeout, 0.1);

	*pTime = time + timeout;
	return eSTATUS_COMMON_OK;
}

EStatus RoutingGetExpirationTime(Routing* pThis, double* pTime)
{
	double time;
	double timeout;
	VALIDATE_ARGUMENTS(pThis && pTime);
	CHECK(TimeLineGetTime(pThis->pTimeLine, &time));
	CHECK(SettingsGetRouteExpirationTimeout(pThis->pSettings, &timeout));

	VARIANCE(timeout, 0.1);

	*pTime = time + timeout;
	return eSTATUS_COMMON_OK;
}

EStatus RoutingClear(Routing* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	CHECK(ListCleanUp(pThis->pEntries, (ItemFilter)&RoutingEraser, pThis));
	return eSTATUS_COMMON_OK;
}

EStatus RoutingRegisterHandler(Routing* pThis, RoutingHandler handler, void* pUserArg)
{
	VALIDATE_ARGUMENTS(pThis);
	pThis->handler.callback = handler;
	pThis->handler.pArg = pUserArg;

	return eSTATUS_COMMON_OK;
}

Boolean RoutingDumpEntry(const RoutingEntry* pEntry, void* pArg)
{
    DUMP_PRINT("RoutingEntry: [dst=%d, transit=%d, length=%d, expires=%f]",
               pEntry->dstId,
               pEntry->transitId,
               pEntry->length,
               pEntry->expires);
    return TRUE;
}

EStatus RoutingDump(const Routing* pThis)
{
    CHECK(ListEnumerate(pThis->pEntries, (ItemEnumerator)&RoutingDumpEntry, NULL));
    return eSTATUS_COMMON_OK;
}
