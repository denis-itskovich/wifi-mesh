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

EStatus SimulatorNew(Simulator** ppThis, Settings* pSettings)
{
	CONSTRUCT(ppThis, Simulator, pSettings);
}

EStatus SimulatorInit(Simulator* pThis, Settings* pSettings)
{
	VALIDATE_ARGUMENTS(pThis && pSettings);
	CHECK(ListNew(&pThis->pStations));
	CHECK(TimeLineNew(&pThis->pTimeLine));
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

	CHECK(TimeLineDelete(&pThis->pTimeLine));
	CHECK(ListDelete(&pThis->pStations));

	return eSTATUS_COMMON_OK;
}

EStatus SimulatorAddStation(Simulator* pThis, Station* pStation)
{
	VALIDATE_ARGUMENTS(pThis && pStation);
	return ListInsert(pThis->pStations, pStation);
}

Comparison SimulatorStationFinder(const Station* pStation, const StationId* pId, void* pUserArg)
{
	StationId id;
	StationGetId(pStation, &id);

	return (id == *pId) ? EQUAL :
		   (id < *pId) ? LESS : GREAT;
}

EStatus SimulatorGetStation(Simulator* pThis, StationId id, Station** ppStation)
{
	ListEntry* pEntry;

	VALIDATE_ARGUMENTS(pThis && ppStation);

	CHECK(ListFind(pThis->pStations, &pEntry, (ItemComparator)&SimulatorStationFinder, &id, NULL));
	CHECK(ListGetValue(pEntry, (void**)ppStation));

	return eSTATUS_COMMON_OK;
}

EStatus SimulatorProcess(Simulator* pThis)
{
	ListEntry* pEntry;
	Station* pStation;
	double oldTime;
	double newTime;
	VALIDATE_ARGUMENTS(pThis);

	CHECK(TimeLineGetTime(pThis->pTimeLine, &oldTime));
	CHECK(TimeLineNext(pThis->pTimeLine));
	CHECK(TimeLineGetTime(pThis->pTimeLine, &newTime));

	CHECK(ListGetHead(pThis->pStations, &pEntry));

	while (pEntry)
	{
		CHECK(ListGetValue(pEntry, (void**)&pStation));
		CHECK(StationSynchronize(pStation, newTime-oldTime));
		CHECK(SimulatorDispatchMessages(pThis, pStation));
		CHECK(ListGetNext(&pEntry));
	}

	return eSTATUS_COMMON_OK;
}

EStatus SimulatorDispatchMessages(Simulator* pThis, Station* pStation)
{
	Message* pMessage;
	ListEntry* pEntry;
	Boolean bIsAdjacent;
	Station* pCurrent;

	VALIDATE_ARGUMENTS(pThis && pStation);
	CHECK(StationGetMessage(pStation, &pMessage));

	if (!pMessage) return eSTATUS_COMMON_OK;

	CHECK(ListGetHead(pThis->pStations, &pEntry));
	while (pEntry)
	{
		CHECK(ListGetValue(pEntry, (void**)&pCurrent));
		CHECK(StationIsAdjacent(pStation, pCurrent, &bIsAdjacent));
		if (bIsAdjacent)
		{
			CHECK(StationPutMessage(pCurrent, pMessage));
		}
		CHECK(ListGetNext(&pEntry));
	}

	return eSTATUS_COMMON_OK;
}
