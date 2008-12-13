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

#define NEW_STATION_ITEM(ptr) SAFE_OPERATION(NEW(ptr); CHECK_STATUS(ListCreate(&ptr->listHeader)))
#define STATION_FROM_LIST_ITEM(pListHdr) ((StationListItem*)pListHdr)->pStation

struct _Simulator
{
	List		stations;
	TimeLine	timeLine;
};

EStatus SimulatorProcessEvent(Simulator* pThis, Event* pEvent);
EStatus SimulatorGetStation(Simulator* pThis, StationId id, Station** ppStation);

EStatus SimulatorCreate(Simulator** ppThis)
{
	CONSTRUCT(ppThis, Simulator, TRUE);
	return SimulatorInit(*ppThis);
}

EStatus SimulatorInit(Simulator* pThis)
{
	CHECK_STATUS(ListInit(&pThis->stations));
	CHECK_STATUS(TimeLineInit(&pThis->timeLine));

	return eSTATUS_COMMON_OK;
}

EStatus SimulatorDispose(Simulator** ppThis)
{
	VALIDATE_ARGUMENTS(ppThis && *ppThis);
	CHECK_STATUS(SimulatorDestroy(*ppThis));
	DELETE(*ppThis);

	return eSTATUS_COMMON_OK;
}

EStatus SimulatorDestroy(Simulator* pThis)
{
	VALIDATE_ARGUMENTS(pThis);

	CHECK_STATUS(TimeLineDestroy(&ppThis->timeLine));
	CHECK_STATUS(ListDestroy(&pThis->stations));

	return eSTATUS_COMMON_OK;
}

EStatus SimulatorAddStation(Simulator* pThis, StationId id)
{
	Station* pStation;

	VALIDATE_ARGUMENTS(pThis);
	NEW_STATION_ITEM(pItem);

	CHECK_STATUS(StationCreate(&pStation));
	CHECK_STATUS(ListInsert(&pThis->stations, pStation));

	return eSTATUS_COMMON_OK;
}

Boolean SimulatorStationComparator(const void* pStation, const void* pId)
{
	return (((Station*)pStation)->id == *(StationId*)pId) ? TRUE : FALSE;
}

EStatus SimulatorGetStation(Simulator* pThis, StationId id, Station** ppStation)
{
	ListPosition* pIter;
	StationId curId;

	VALIDATE_ARGUMENTS(pThis && ppStation);

	CHECK_STATUS(ListFind(&pThis->stations, &pIter, SimulatorStationComparator, &id));
	CHECK_STATUS(ListGetValue(pIter, (void**)ppStation));

	return eSTATUS_COMMON_OK;
}

EStatus SimulatorProcess(Simulator* pThis)
{
	Event* pEvent;
	VALIDATE_ARGUMENTS(pThis);

	CHECK_STATUS(TimeLineGetNextEvent(&pThis->timeLine, &pEvent));
	SimulatorProcessEvent(pThis, pEvent);

	return eSTATUS_COMMON_OK;
}

EStatus SimulatorProcessEvent(Simulator* pThis, Event* pEvent)
{
	ListPosition* pIter;
	Station* pStation;
	unsigned long timeDelta, currentTime;

	VALIDATE_ARGUMENTS(pThis && pEvent);

	CHECK_STATUS(EventGetTime(pEvent, &currentTime));
	timeDelta = currentTime - pThis->lastTime;
	pThis->lastTime = currentTime;

	CHECK_STATUS(ListGetBegin(pThis->pStations, &pIter));

	while (pIter)
	{
		CHECK_STATUS(ListGetValue(pIter, (void**)&pStation));
		CHECK_STATUS(StationMove(pStation, timeDelta));
		CHECK_STATUS(ListGetNext(&pIter));
	}

	return eSTATUS_COMMON_OK;
}
