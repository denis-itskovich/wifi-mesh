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
 * @file Simulator.c
 *
 * @date 06/12/2008
 * @author Denis Itskovich
 */

#include "Simulator.h"
#include "Station.h"
#include "TimeLine.h"
#include "Macros.h"
#include "List.h"

#define NEW_STATION_ITEM(ptr) SAFE_OPERATION(ptr = NEW(*ptr); CHECK(ListCreate(&ptr->listHeader)))
#define STATION_FROM_LIST_ITEM(pListHdr) ((StationListItem*)pListHdr)->pStation

struct _Simulator
{
	List*          pStations;
	TimeLine*      pTimeLine;
	Settings*      pSettings;
	StationId      minId;
	StationId      maxId;
	Boolean        bDupBroadcast;
	Boolean        bUpdateRequired;
	double         lastUpdateTime;
	double         timeDelta;

	struct
	{
		PacketSniffer callback;
		void*         pArg;
	} 				sniffer;
	struct
	{
		StationTracker    callback;
		void*		      pArg;
	} 				tracker;
};

/** Looks for a station with specified id
 * @param pThis [in] pointer to instance
 * @param id [in] station id to look for
 * @param ppStation [out] pointer to station will be stored at *ppStation
 */
EStatus SimulatorGetStation(Simulator* pThis, StationId id, Station** ppStation);

/** Dispatches outbound packets for specified station
 * @param pThis [in] pointer to instance
 * @param pStation [in] pointer to station
 */
EStatus SimulatorDispatchPacket(Simulator* pThis, Station* pStation);

/** Retrieves station list entry by id
 * @param pThis [in] pointer to instance
 * @param id [in] station id
 * @param ppEntry [out] pointer to entry will be stored at *ppEntry
 */
EStatus SimulatorGetStationEntry(Simulator* pThis, StationId id, ListEntry** ppEntry);

/** Assigns unique station id
 * @param pThis [in] pointer to instance
 */
StationId SimulatorAssignId(Simulator* pThis);

/** Frees station id
 * @param pThis [in] pointer to instance
 * @param id [in] id to free
 */
void SimulatorFreeId(Simulator* pThis, StationId id);

/** Invokes sniffer callback (if exists)
 * @param pThis [in] pointer to instance
 * @param pPacket [in] pointer to packet
 * @param pSrc [in] pointer to source station
 * @param pDst [in] pointer to destination station
 */
EStatus SimulatorInvokeSniffer(Simulator* pThis, const Packet* pPacket, const Station* pSrc, const Station* pDst, EPacketStatus status);

/** Invokes tracker callback (if exists)
 * @param pThis [in] pointer to instance
 * @param pStation [in] pointer to station being updated
 * @param event [in] event type /see EStationEvent
 */
EStatus SimulatorInvokeTracker(Simulator* pThis, Station* pStation, EStationEvent event);

Boolean SimulatorCleaner(Station* pStation, Simulator* pThis)
{
	StationId id;

	SimulatorInvokeTracker(pThis, pStation, eSTATION_REMOVED);
	StationGetId(pStation, &id);
	StationDelete(&pStation);
	SimulatorFreeId(pThis, id);

	return FALSE;
}

Boolean SimulatorResetter(Station* pStation, Simulator* pThis)
{
	StationReset(pStation);
	SimulatorInvokeTracker(pThis, pStation, eSTATION_UPDATED);
	return TRUE;
}

EStatus SimulatorNew(Simulator** ppThis, Settings* pSettings, TimeLine* pTimeLine)
{
	CONSTRUCT(ppThis, Simulator, pSettings, pTimeLine);
}

EStatus SimulatorInit(Simulator* pThis, Settings* pSettings, TimeLine* pTimeLine)
{
	VALIDATE_ARGUMENTS(pThis && pSettings);
	CLEAR(pThis);

	CHECK(ListNew(&pThis->pStations));
	pThis->pTimeLine = pTimeLine;
	pThis->pSettings = pSettings;
	pThis->bDupBroadcast = TRUE;

	return eSTATUS_COMMON_OK;
}

EStatus SimulatorDelete(Simulator** ppThis)
{
	DESTRUCT(ppThis, Simulator);
}

EStatus SimulatorDestroy(Simulator* pThis)
{
	VALIDATE_ARGUMENTS(pThis);

	CHECK(ListDelete(&pThis->pStations));

	return eSTATUS_COMMON_OK;
}

EStatus SimulatorAddStation(Simulator* pThis, Station* pStation)
{
	VALIDATE_ARGUMENTS(pThis && pStation);
	CHECK(StationSetId(pStation, SimulatorAssignId(pThis)));
	SimulatorInvokeTracker(pThis, pStation, eSTATION_ADDED);
	return ListPushBack(pThis->pStations, pStation);
}

EStatus SimulatorRemoveStation(Simulator* pThis, Station* pStation)
{
	StationId id;
	ListEntry* pEntry;

	VALIDATE_ARGUMENTS(pThis && pStation);

	CHECK(StationGetId(pStation, &id));
	CHECK(SimulatorGetStationEntry(pThis, id, &pEntry));
	SimulatorCleaner(pStation, pThis);

	return ListRemove(pThis->pStations, pEntry);
}

Comparison SimulatorStationFinder(const Station* pStation, const StationId* pId, void* pUserArg)
{
	StationId id;
	StationGetId(pStation, &id);

	return (id == *pId) ? EQUAL :
		   (id < *pId) ? LESS : GREAT;
}

EStatus SimulatorGetStationEntry(Simulator* pThis, StationId id, ListEntry** ppEntry)
{
	VALIDATE_ARGUMENTS(pThis && ppEntry);
	CHECK(ListFind(pThis->pStations, ppEntry, (ItemComparator)&SimulatorStationFinder, &id, NULL));
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorGetStation(Simulator* pThis, StationId id, Station** ppStation)
{
	ListEntry* pEntry;
	CHECK(SimulatorGetStationEntry(pThis, id, &pEntry));
	CHECK(ListGetValue(pEntry, ppStation));
	return eSTATUS_COMMON_OK;
}

Boolean SimulatorSynchronizer(Station* pStation, Simulator* pThis)
{
    Packet* pDeliveredPacket;
	Boolean isActive;
	StationIsActive(pStation, &isActive);
	if (isActive)
	{
		StationSynchronize(pStation, pThis->timeDelta, &pDeliveredPacket);
		StationIsActive(pStation, &isActive);
		if (pDeliveredPacket)
        {
		    SimulatorInvokeSniffer(pThis, pDeliveredPacket, NULL, pStation, ePKT_STATUS_DELIVERED);
		    PacketDelete(&pDeliveredPacket);
        }
        if (!isActive) SimulatorInvokeTracker(pThis, pStation, eSTATION_UPDATED);
	}
	return TRUE;
}

Boolean SimulatorDispatcher(Station* pStation, Simulator* pThis)
{
	Boolean isActive;
	StationIsActive(pStation, &isActive);
	if (isActive)
	{
		SimulatorDispatchPacket(pThis, pStation);
	    if (pThis->bUpdateRequired) SimulatorInvokeTracker(pThis, pStation, eSTATION_UPDATED);
	}
	return TRUE;
}

EStatus SimulatorProcess(Simulator* pThis)
{
	double oldTime;
	double curTime;

	BEGIN_FUNCTION;

	VALIDATE_ARGUMENTS(pThis);

	INFO_PRINT("Performing simulation step: [time delta: %.2f]", timeDelta);

    CHECK(TimeLineGetTime(pThis->pTimeLine, &oldTime));
    CHECK(TimeLineNext(pThis->pTimeLine));
    CHECK(TimeLineGetTime(pThis->pTimeLine, &curTime));

    pThis->bUpdateRequired = (curTime - pThis->lastUpdateTime > 0.01) ? TRUE : FALSE;
    if (pThis->bUpdateRequired) pThis->lastUpdateTime = curTime;

    pThis->timeDelta = curTime - oldTime;

	CHECK(ListEnumerate(pThis->pStations, (ItemEnumerator)&SimulatorSynchronizer, pThis));
    CHECK(ListEnumerate(pThis->pStations, (ItemEnumerator)&SimulatorDispatcher, pThis));

	END_FUNCTION;
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorInvokeSniffer(Simulator* pThis, const Packet* pPacket, const Station* pSrc, const Station* pDst, EPacketStatus status)
{
	if (pThis->sniffer.callback)
	{
		if ((pThis->bDupBroadcast && pDst) ||
			(!pThis->bDupBroadcast && !pDst))
		{
			pThis->sniffer.callback(pPacket, pSrc, pDst, status, pThis->sniffer.pArg);
		}
	}
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorInvokeTracker(Simulator* pThis, Station* pStation, EStationEvent event)
{
    if (pThis->tracker.callback)
    {
        pThis->tracker.callback(pStation, event, pThis->tracker.pArg);
    }
    return eSTATUS_COMMON_OK;
}

EStatus SimulatorDispatchPacket(Simulator* pThis, Station* pStation)
{
	Packet* pPacket = NULL;
	Packet* pAbortedPacket;
	ListEntry* pEntry = NULL;
	Boolean isAdjacent = FALSE;
	Boolean isActive = FALSE;
	Station* pCurrent = NULL;
	EStatus ret;
	StationId id;

	VALIDATE_ARGUMENTS(pThis && pStation);
	CHECK(StationGetId(pStation, &id));
	CHECK(StationGetPacket(pStation, &pPacket));

	if (!pPacket) return eSTATUS_COMMON_OK;
	pPacket->routing.path[pPacket->routing.length++] = id;
    pPacket->header.hopsCount++;

	CHECK(ListGetHead(pThis->pStations, &pEntry));
	while (pEntry)
	{
        CHECK(ListGetValue(pEntry, &pCurrent));
        CHECK(StationGetId(pCurrent, &id));
	    CHECK(StationIsActive(pCurrent, &isActive));
	    if (isActive)
	    {
	        CHECK(StationIsAdjacent(pStation, pCurrent, &isAdjacent));
	        if (isAdjacent)
	        {
	            ret = StationPutPacket(pCurrent, pPacket, &pAbortedPacket);
	            switch (ret)
	            {
	            case eSTATUS_COMMON_OK:
	                CHECK(SimulatorInvokeSniffer(pThis, pPacket, pStation, pCurrent, ePKT_STATUS_PENDING));
	                break;
                case eSTATUS_STATION_PACKET_COLLISION:
                    if (pAbortedPacket)
                    {
                        CHECK(SimulatorInvokeSniffer(pThis, pAbortedPacket, pStation, pCurrent, ePKT_STATUS_COLLISION));
                        PacketDelete(&pAbortedPacket);
                    }
                    CHECK(SimulatorInvokeSniffer(pThis, pPacket, pStation, pCurrent, ePKT_STATUS_PENDING));
                    CHECK(SimulatorInvokeSniffer(pThis, pPacket, pStation, pCurrent, ePKT_STATUS_COLLISION));
                    break;
	            case eSTATUS_STATION_PACKET_NOT_ACCEPTED:
	                break;
	            default:
	                return ret;
	            }
	        }
	        else
	        {
	            if (pPacket->header.transitDstId == id) SimulatorInvokeSniffer(pThis, pPacket, pStation, pCurrent, ePKT_STATUS_OUT_OF_RANGE);
	        }
	    }

		CHECK(ListGetNext(&pEntry));
	}

	CHECK(PacketDelete(&pPacket));
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorSetPacketSniffer(Simulator* pThis, PacketSniffer sniffer, void* pUserArg)
{
	VALIDATE_ARGUMENTS(pThis);
	pThis->sniffer.callback = sniffer;
	pThis->sniffer.pArg = pUserArg;
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorSetStationTracker(Simulator* pThis, StationTracker tracker, void* pUserArg)
{
	VALIDATE_ARGUMENTS(pThis);
	pThis->tracker.callback = tracker;
	pThis->tracker.pArg = pUserArg;
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorReset(Simulator* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
    CHECK(TimeLineClear(pThis->pTimeLine));
	CHECK(ListEnumerate(pThis->pStations, (ItemEnumerator)&SimulatorResetter, pThis));
	pThis->lastUpdateTime = 0;
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorClear(Simulator* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	CHECK(ListCleanUp(pThis->pStations, (ItemFilter)&SimulatorCleaner, pThis));
	CHECK(TimeLineClear(pThis->pTimeLine));
    pThis->lastUpdateTime = 0;
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorEnumerateStations(Simulator* pThis, StationsEnumerator enumerator, void* pUserArg)
{
	VALIDATE_ARGUMENTS(pThis && enumerator);
	return ListEnumerate(pThis->pStations, (ItemEnumerator)enumerator, pUserArg);
}

StationId SimulatorAssignId(Simulator* pThis)
{
	if (pThis->minId > 0) return --pThis->minId;
	return ++pThis->maxId;
}

void SimulatorFreeId(Simulator* pThis, StationId id)
{
	int count = 0;
	if (pThis->minId == id) ++pThis->minId;
	if (pThis->maxId == id) --pThis->maxId;
	ListGetCount(pThis->pStations, &count);
	if (count == 1) pThis->minId = pThis->maxId = 0;
}

EStatus SimulatorSetSniffingMode(Simulator* pThis, Boolean bSingle)
{
	SET_MEMBER(bSingle, pThis, bDupBroadcast);
}

EStatus SimulatorGetSniffingMode(Simulator* pThis, Boolean* pSingle)
{
	GET_MEMBER(pSingle, pThis, bDupBroadcast);
}
