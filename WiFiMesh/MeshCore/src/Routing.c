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

typedef struct _RoutingEntry
{
	StationId  dstId;
	StationId  transitId;
	double     expires;
	unsigned   length;
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
		                        flag,
		                        pThis->handler.pArg);
	}
}

Boolean RoutingEraser(RoutingEntry* pEntry, Routing* pThis);
Boolean RoutingTransitCleaner(RoutingEntry* pEntry, Routing* pThis);
Boolean RoutingCleaner(RoutingEntry* pEntry, Routing* pThis);


Boolean RoutingTransitCleaner(RoutingEntry* pEntry, Routing* pThis)
{
    if (pEntry->transitId != pThis->deprecatedId) return TRUE;
    return RoutingEraser(pEntry, pThis);
}

Boolean RoutingCleaner(RoutingEntry* pEntry, Routing* pThis)
{
	double time;
	TimeLineGetTime(pThis->pTimeLine, &time);
	if (pEntry->expires > time) return TRUE;
	return RoutingEraser(pEntry, pThis);
}

Boolean RoutingEraser(RoutingEntry* pEntry, Routing* pThis)
{
	RoutingInvokeHandler(pThis, pEntry, eROUTE_REMOVE);
	DELETE(pEntry);
	return FALSE;
}

Comparison RoutingFinder(RoutingEntry* pEntry, StationId* pId, StationId* pTransitId)
{
	if (pEntry->dstId == *pId)
	{
		if (pTransitId) *pTransitId = pEntry->transitId;
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
	RoutingClear(pThis);
	return ListDelete(&pThis->pEntries);
}

EStatus RoutingRemoveTransit(Routing* pThis, StationId transit)
{
    VALIDATE_ARGUMENTS(pThis);
    pThis->deprecatedId = transit;
    return ListCleanUp(pThis->pEntries, (ItemFilter)&RoutingTransitCleaner, pThis);
}

EStatus RoutingUpdateDestination(Routing* pThis, StationId dst, StationId transit, unsigned length)
{
    ListEntry* pListEntry;
    RoutingEntry* pRouteEntry;

    if (eSTATUS_LIST_NOT_FOUND == ListFind(pThis->pEntries, &pListEntry, (ItemComparator)&RoutingFinder, &dst, NULL))
    {
        CHECK(RoutingAddRoute(pThis, dst, transit, length));
    }
    else
    {
        CHECK(ListGetValue(pListEntry, &pRouteEntry));

        if (pRouteEntry->length > length ||
            (pRouteEntry->length == length && pRouteEntry->transitId != transit))
        {
            pRouteEntry->length = length;
            pRouteEntry->transitId = transit;
            CHECK(RoutingGetExpirationTime(pThis, &pRouteEntry->expires));
            CHECK(TimeLineEvent(pThis->pTimeLine, pRouteEntry->expires, NULL));
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
	return RoutingAddRoute(pThis, dstId, INVALID_STATION_ID, (unsigned)-1);
}

EStatus RoutingAddRoute(Routing* pThis, StationId dstId, StationId transitId, unsigned length)
{
	RoutingEntry* pEntry;
	VALIDATE_ARGUMENTS(pThis);
	pEntry = NEW(RoutingEntry);
	pEntry->dstId = dstId;
	pEntry->transitId = transitId;
	pEntry->length = length;
    if (transitId == INVALID_STATION_ID) CHECK(RoutingGetRetryTime(pThis, &pEntry->expires));
    else CHECK(RoutingGetExpirationTime(pThis, &pEntry->expires));
    CHECK(TimeLineEvent(pThis->pTimeLine, pEntry->expires, NULL));
    RoutingInvokeHandler(pThis, pEntry, eROUTE_ADD);
    return ListPushBack(pThis->pEntries, pEntry);
}

EStatus RoutingLookFor(Routing* pThis, StationId dstId, StationId* pTransitId, unsigned* pHopsCount)
{
	ListEntry* pEntry;
	RoutingEntry* pRouteEntry;
	VALIDATE_ARGUMENTS(pThis);
	CHECK(ListFind(pThis->pEntries, &pEntry, (ItemComparator)&RoutingFinder, &dstId, pTransitId));
	CHECK(ListGetValue(pEntry, &pRouteEntry));
	if (pHopsCount) *pHopsCount = pRouteEntry->length;
	return eSTATUS_COMMON_OK;
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
