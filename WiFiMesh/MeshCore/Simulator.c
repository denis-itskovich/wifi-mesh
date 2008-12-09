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

#define NEW_STATION_ITEM(ptr) SAFE_OPERATION(NEW(ptr); CHECK_STATUS_RETURN(ListCreate(&ptr->listHeader)))
#define STATION_FROM_LIST_ITEM(pListHdr) ((StationListItem*)pListHdr)->pStation

struct _Simulator
{
	List*		pStations;
	TimeLine*	pTimeLine;
};

EStatus SimulatorProcessEvent(Simulator* pThis, Event* pEvent);
EStatus SimulatorGetStation(Simulator* pThis, StationId id, Station** ppStation);

EStatus SimulatorCreate(Simulator** ppThis)
{
	CONSTRUCT(ppThis, Simulator, TRUE);

	CHECK_STATUS_RETURN(ListCreate(&(*ppThis->pStations)));
	CHECK_STATUS_RETURN(TimeLineCreate(&(*ppThis->pTimeLine)));

	return eSTATUS_COMMON_OK;
}

EStatus SimulatorDestroy(Simulator** ppThis)
{
	VALIDATE_ARGUMENTS(ppThis && *ppThis);

	CHECK_STATUS_RETURN(TimeLineDestroy(&(*ppThis->pTimeLine)));
	CHECK_STATUS_RETURN(ListDestroy(&(*ppThis->pStations)));

	DELETE(*ppThis);
}

EStatus SimulatorAddStation(Simulator* pThis, StationId id)
{
	StationListItem* pItem;

	VALIDATE_ARGUMENTS(pThis);
	NEW_STATION_ITEM(pItem);

	CHECK_STATUS_RETURN(StationCreate(&pItem->pStation));
	CHECK_STATUS_RETURN(ListInsert(pThis->pStations, pItem));
	pThis->pStations = &pItem->listHeader;

	return eSTATUS_COMMON_OK;
}

Boolean SimulatorStationComparator(const void* pStation, const void* pId)
{
	return (((Station*)pStation)->id == *(StationId*)pId) ? TRUE : FALSE;
}

EStatus SimulatorGetStation(Simulator* pThis, StationId id, Station** ppStation)
{
	ListItem* pIter;
	StationId curId;

	VALIDATE_ARGUMENTS(pThis && ppStation);

	CHECK_STATUS_RETURN(ListFind(pThis->pStations, &pIter, SimulatorStationComparator, &id));
	CHECK_STATUS_RETURN(ListGetValue(pIter, (void**)ppStation));

	return eSTATUS_COMMON_OK;
}

EStatus SimulatorProcess(Simulator* pThis)
{
	Event* pEvent;
	VALIDATE_ARGUMENTS(pThis);

	CHECK_STATUS_RETURN(TimeLineGetNextEvent(pThis->pTimeLine, &pEvent));
	SimulatorProcessEvent(pThis, pEvent);

	return eSTATUS_COMMON_OK;
}

EStatus SimulatorProcessEvent(Simulator* pThis, Event* pEvent)
{
	ListItem* pIter;
	Station* pStation;
	unsigned long timeDelta, currentTime;

	VALIDATE_ARGUMENTS(pThis && pEvent);

	CHECK_STATUS_RETURN(EventGetTime(pEvent, &currentTime));
	timeDelta = currentTime - pThis->lastTime;
	pThis->lastTime = currentTime;

	CHECK_STATUS_RETURN(ListGetBegin(pThis->pStations, &pIter));

	while (pIter)
	{
		CHECK_STATUS_RETURN(ListGetValue(pIter, (void**)&pStation));
		CHECK_STATUS_RETURN(StationMove(pStation, timeDelta));
		CHECK_STATUS_RETURN(ListGetNext(&pIter));
	}

	return eSTATUS_COMMON_OK;
}
