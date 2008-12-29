/*
 * Simulator.c
 *
 *  Created on: 06/12/2008
 *      Author: denis
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
};

EStatus SimulatorGetStation(Simulator* pThis, StationId id, Station** ppStation);

EStatus SimulatoreNew(Simulator** ppThis)
{
	CONSTRUCT(ppThis, Simulator);
}

EStatus SimulatorInit(Simulator* pThis)
{
	CHECK(ListNew(&pThis->pStations));
	CHECK(TimeLineNew(&pThis->pTimeLine));

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
//	Event* pEvent;
//	VALIDATE_ARGUMENTS(pThis);
//
//	CHECK(TimeLineGetEvent(pThis->pTimeLine, &pEvent));
//	SimulatorProcessEvent(pThis, pEvent);
//
	return eSTATUS_COMMON_OK;
}
