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

typedef struct _StationListItem
{
	ListHeader	listHeader;
	Station*	pStation;
} StationListItem;

struct _Simulator
{
	ListHeader*	pStations;
	TimeLine*	pTimeLine;
};

EStatus SimulatorCreate(Simulator** ppThis)
{
	CONSTRUCT(ppThis, Simulator, TRUE);

	CHECK_STATUS_RETURN(TimeLineCreate(&(*ppThis->pTimeLine)));

	return eSTATUS_COMMON_OK;
}

EStatus SimulatorDestroy(Simulator** ppThis)
{
	VALIDATE_ARGUMENTS(ppThis && *ppThis);

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

EStatus SimulatorGetStation(Simulator* pThis, StationId id, Station** ppStation)
{
	ListHeader* pHdr;
	StationId curId;
	VALIDATE_ARGUMENTS(pThis && ppStation);

	pHdr = pThis->pStations;
	FOR_EACH(pHdr,
			*ppStation = STATION_FROM_LIST_ITEM(pHdr);
			CHECK_STATUS_RETURN(StationGetId(*ppStation, &curId));
			if (curId == id) return eSTATUS_COMMON_OK;
	)

	return eSTATUS_SIMULATOR_STATION_NOT_FOUND;
}

EStatus SimulatorProcess(Simulator* pThis)
{
	Event* pEvent;

	TimeLineGetNextEvent
}
