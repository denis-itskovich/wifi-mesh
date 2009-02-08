/**
 * \file Simulator.c
 *
 * \date 06/12/2008
 * \author Denis Itskovich
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
	List*			pStations;
	TimeLine*		pTimeLine;
	Settings*		pSettings;
	StationId		minId;
	StationId		maxId;
	Boolean			bDupBroadcast;
	int				steps;
	double			timeDelta;

	struct
	{
		Sniffer callback;
		void*	    pArg;
	} 				sniffer;
	struct
	{
		StationTracker    callback;
		void*		      pArg;
	} 				tracker;

	struct
	{
	    SignalRadar        callback;
	    void*              pArg;
	}               radar;
};

/** Looks for a station with specified id
 * \param pThis [in] pointer to instance
 * \param id [in] station id to look for
 * \param ppStation [out] pointer to station will be stored at *ppStation
 */
EStatus SimulatorGetStation(Simulator* pThis, StationId id, Station** ppStation);

/** Dispatches outbound packets for specified station
 * \param pThis [in] pointer to instance
 * \param pStation [in] pointer to station
 * \param pDelivered [out] delivered packets count will be stored at *pDelivered
 */
EStatus SimulatorDispatchPackets(Simulator* pThis, Station* pStation, int* pDelivered);

/** Retrieves station list entry by id
 * \param pThis [in] pointer to instance
 * \param id [in] station id
 * \param ppEntry [out] pointer to entry will be stored at *ppEntry
 */
EStatus SimulatorGetStationEntry(Simulator* pThis, StationId id, ListEntry** ppEntry);

/** Assigns unique station id
 * \param pThis [in] pointer to instance
 */
StationId SimulatorAssignId(Simulator* pThis);

/** Frees station id
 * \param pThis [in] pointer to instance
 * \param id [in] id to free
 */
void SimulatorFreeId(Simulator* pThis, StationId id);

/** Invokes sniffer callback (if exists)
 * \param pThis [in] pointer to instance
 * \param pPacket [in] pointer to packet
 * \param pSrc [in] pointer to source station
 * \param pDst [in] pointer to destination station
 */
EStatus SimulatorInvokeSniffer(Simulator* pThis, const Packet* pPacket, const Station* pSrc, const Station* pDst);

/** Invokes tracker callback (if exists)
 * \param pThis [in] pointer to instance
 * \param pStation [in] pointer to station being updated
 * \param event [in] event type /see EStationEvent
 */
EStatus SimulatorInvokeTracker(Simulator* pThis, Station* pStation, EStationEvent event);

/** Invokes signal radar callback (if exists)
 * \param pThis [in] pointer to instance
 * \param pSrc [in] pointer to source station
 * \param pDst [in] pointer to destination station
 */
EStatus SimulatorInvokeRadar(Simulator* pThis, const Station* pSrc, const Station* pDst);

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
	Boolean isActive;
	Boolean isReceiveOver = FALSE;
	StationIsActive(pStation, &isActive);
	if (isActive)
	{
		StationSynchronize(pStation, pThis->timeDelta, &isReceiveOver);
		StationIsActive(pStation, &isActive);
		if (!isActive) SimulatorInvokeTracker(pThis, pStation, eSTATION_UPDATED);
		if (isReceiveOver) SimulatorInvokeRadar(pThis, NULL, pStation);
	}
	return TRUE;
}

Boolean SimulatorDispatcher(Station* pStation, Simulator* pThis)
{
	Boolean isActive;
	int delivered;
	StationIsActive(pStation, &isActive);
	if (isActive)
	{
		SimulatorDispatchPackets(pThis, pStation, &delivered);
		if (pThis->steps % 20 == 0) SimulatorInvokeTracker(pThis, pStation, eSTATION_UPDATED);
	}
	return TRUE;
}

EStatus SimulatorProcess(Simulator* pThis)
{
	double oldTime;
	double timeDelta;

	BEGIN_FUNCTION;

	VALIDATE_ARGUMENTS(pThis);

	CHECK(TimeLineGetTime(pThis->pTimeLine, &oldTime));
	CHECK(TimeLineNext(pThis->pTimeLine));
	CHECK(TimeLineGetTime(pThis->pTimeLine, &timeDelta));
	timeDelta -= oldTime;
	pThis->timeDelta = timeDelta;

	INFO_PRINT("Performing simulation step: [time delta: %.2f]", timeDelta);

	CHECK(ListEnumerate(pThis->pStations, (ItemEnumerator)&SimulatorSynchronizer, pThis));
	CHECK(ListEnumerate(pThis->pStations, (ItemEnumerator)&SimulatorDispatcher, pThis));

	pThis->steps++;
	END_FUNCTION;
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorInvokeSniffer(Simulator* pThis, const Packet* pPacket, const Station* pSrc, const Station* pDst)
{
	if (pThis->sniffer.callback)
	{
		if ((pThis->bDupBroadcast && pDst) ||
			(!pThis->bDupBroadcast && !pDst))
		{
			pThis->sniffer.callback(pPacket, pSrc, pDst, pThis->sniffer.pArg);
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

EStatus SimulatorInvokeRadar(Simulator* pThis, const Station* pSrc, const Station* pDst)
{
    if (pThis->radar.callback)
    {
        pThis->radar.callback(pSrc, pDst, pThis->radar.pArg);
    }
    return eSTATUS_COMMON_OK;
}

EStatus SimulatorDispatchPackets(Simulator* pThis, Station* pStation, int* pDelivered)
{
	Packet* pPacket = NULL;
	ListEntry* pEntry = NULL;
	Boolean bIsAdjacent = FALSE;
	Station* pCurrent = NULL;
	EStatus ret;
	*pDelivered = 0;

	VALIDATE_ARGUMENTS(pThis && pStation);
	CHECK(StationGetPacket(pStation, &pPacket));

	if (!pPacket) return eSTATUS_COMMON_OK;
	++pPacket->nodesCount;

	CHECK(ListGetHead(pThis->pStations, &pEntry));
	while (pEntry)
	{
		CHECK(ListGetValue(pEntry, &pCurrent));
		CHECK(StationIsAdjacent(pStation, pCurrent, &bIsAdjacent));
		if (bIsAdjacent)
		{
			ret = StationPutPacket(pCurrent, pPacket);
			if (ret == eSTATUS_COMMON_OK)
			{
				CHECK(SimulatorInvokeSniffer(pThis, pPacket, pStation, pCurrent));
				CHECK(SimulatorInvokeRadar(pThis, pStation, pCurrent));
				(*pDelivered)++;
			}
			else if (ret != eSTATUS_STATION_PACKET_NOT_ACCEPTED) return ret;
		}
		CHECK(ListGetNext(&pEntry));
	}

	if (*pDelivered) CHECK(SimulatorInvokeSniffer(pThis, pPacket, pStation, NULL));
	CHECK(PacketDelete(&pPacket));
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorSetSniffer(Simulator* pThis, Sniffer sniffer, void* pUserArg)
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

EStatus SimulatorSetSignalRadar(Simulator* pThis, SignalRadar radar, void* pUserArg)
{
    VALIDATE_ARGUMENTS(pThis);
    pThis->radar.callback = radar;
    pThis->radar.pArg = pUserArg;
    return eSTATUS_COMMON_OK;
}

EStatus SimulatorReset(Simulator* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	CHECK(ListEnumerate(pThis->pStations, (ItemEnumerator)&SimulatorResetter, pThis));
	CHECK(TimeLineReset(pThis->pTimeLine));
	pThis->steps = 0;
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorClear(Simulator* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	CHECK(ListCleanUp(pThis->pStations, (ItemFilter)&SimulatorCleaner, pThis));
	CHECK(TimeLineClear(pThis->pTimeLine));
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
	if (pThis->maxId == id + 1) --pThis->maxId;
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
