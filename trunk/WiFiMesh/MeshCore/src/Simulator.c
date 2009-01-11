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
	List*		pStations;
	TimeLine*	pTimeLine;
	Settings*	pSettings;
	struct
	{
		Sniffer callback;
		void*	pArg;
	} sniffer;
};

/** Looks for a station with specified id
 * \param pThis [in] pointer to instance
 * \param id [in] station id to look for
 * \param ppStation [out] pointer to station will be stored at *ppStation
 */
EStatus SimulatorGetStation(Simulator* pThis, StationId id, Station** ppStation);

/** Dispatches outbound messages for specified station
 * \param pThis [in] pointer to instance
 * \param pStation [in] pointer to station
 */
EStatus SimulatorDispatchMessages(Simulator* pThis, Station* pStation);

/** Retrieves station list entry by id
 * \param pThis [in] pointer to instance
 * \param id [in] station id
 * \param ppEntry [out] pointer to entry will be stored at *ppEntry
 */
EStatus SimulatorGetStationEntry(Simulator* pThis, StationId id, ListEntry** ppEntry);

Boolean SimulatorCleaner(Station* pStation, void* pUserArg)
{
	StationReset(pStation);
	return TRUE;
}

EStatus SimulatorNew(Simulator** ppThis, Settings* pSettings, TimeLine* pTimeLine)
{
	CONSTRUCT(ppThis, Simulator, pSettings, pTimeLine);
}

EStatus SimulatorInit(Simulator* pThis, Settings* pSettings, TimeLine* pTimeLine)
{
	VALIDATE_ARGUMENTS(pThis && pSettings);
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
	return ListInsert(pThis->pStations, pStation);
}

EStatus SimulatorRemoveStation(Simulator* pThis, Station* pStation)
{
	StationId id;
	ListEntry* pEntry;

	VALIDATE_ARGUMENTS(pThis && pStation);

	CHECK(StationGetId(pStation, &id));
	CHECK(SimulatorGetStationEntry(pThis, id, &pEntry));
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

EStatus SimulatorProcess(Simulator* pThis)
{
	ListEntry* pEntry;
	Station* pStation;
	double oldTime;
	double newTime;

	BEGIN_FUNCTION;

	VALIDATE_ARGUMENTS(pThis);

	CHECK(TimeLineGetTime(pThis->pTimeLine, &oldTime));
	CHECK(TimeLineNext(pThis->pTimeLine));
	CHECK(TimeLineGetTime(pThis->pTimeLine, &newTime));

	CHECK(ListGetHead(pThis->pStations, &pEntry));

	while (pEntry)
	{
		CHECK(ListGetValue(pEntry, &pStation));
		CHECK(StationSynchronize(pStation, newTime-oldTime));
		CHECK(SimulatorDispatchMessages(pThis, pStation));
		CHECK(ListGetNext(&pEntry));
	}

	END_FUNCTION;
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorDispatchMessages(Simulator* pThis, Station* pStation)
{
	Message* pMessage = NULL;
	Message* pNewMessage = NULL;
	ListEntry* pEntry = NULL;
	Boolean bIsAdjacent = FALSE;
	Station* pCurrent = NULL;
	double time;

	VALIDATE_ARGUMENTS(pThis && pStation);
	CHECK(StationGetMessage(pStation, &pMessage));
	CHECK(TimeLineGetTime(pThis->pTimeLine, &time));

	if (!pMessage) return eSTATUS_COMMON_OK;

	CHECK(ListGetHead(pThis->pStations, &pEntry));
	while (pEntry)
	{
		CHECK(ListGetValue(pEntry, &pCurrent));
		CHECK(StationIsAdjacent(pStation, pCurrent, &bIsAdjacent));
		if (bIsAdjacent)
		{
			CHECK(MessageClone(&pNewMessage, pMessage));
			CHECK(StationPutMessage(pCurrent, pNewMessage));
			if (pThis->sniffer.callback) pThis->sniffer.callback(time, pMessage, pCurrent, pThis->sniffer.pArg);
		}
		CHECK(ListGetNext(&pEntry));
	}

	CHECK(MessageDelete(&pMessage));
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorSetSniffer(Simulator* pThis, Sniffer sniffer, void* pUserArg)
{
	VALIDATE_ARGUMENTS(pThis);
	pThis->sniffer.callback = sniffer;
	pThis->sniffer.pArg = pUserArg;
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorReset(Simulator* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	CHECK(ListEnumerate(pThis->pStations, (ItemEnumerator)&SimulatorCleaner, NULL));
	CHECK(TimeLineClear(pThis->pTimeLine));
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorEnumerateStations(Simulator* pThis, StationsEnumerator enumerator, void* pUserArg)
{
	VALIDATE_ARGUMENTS(pThis && enumerator);
	return ListEnumerate(pThis->pStations, (ItemEnumerator)enumerator, pUserArg);
}
