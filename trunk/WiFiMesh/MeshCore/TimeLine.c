#include "TimeLine.h"
#include "Macros.h"

struct _TimeLine
{
	List 		events;
	ListPosition*	pCurrent;
};

EStatus TimeLineCreate(TimeLine** ppThis)
{
	CONSTRUCT(ppThis, TimeLine, TRUE);
	return TimeLineInit(*pThis);
}

EStatus TimeLineInit(TimeLine* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	CLEAR(pThis);
	CHECK_STATUS(ListInit(&pThis->events));

	return eSTATUS_COMMON_OK;
}

EStatus TimeLineDispose(TimeLine** ppThis)
{
	VALIDATE_ARGUMENTS(ppThis && *ppThis);
	TimeLineDestroy(*ppThis);
	DELETE(*ppThis);
	return eSTATUS_COMMON_OK;
}

EStatus TimeLineDestroy(TimeLine* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	CHECK_STATUS(ListDestroy(&pThis->events));
	return eSTATUS_COMMON_OK;
}

EStatus TimeLineAddEvent(TimeLine* pThis, Event* pEvent)
{
	VALIDATE_ARGUMENTS(pThis && pEvent);
	CHECK_STATUS(ListInsert(&pThis->events, pEvent));

	return eSTATUS_COMMON_OK;
}

EStatus TimeLineGetNextEvent(TimeLine* pThis, Event** ppEvent)
{
	VALIDATE_ARGUMENTS(pThis && ppEvent);
	*ppEvent = NULL;

	if (pThis->pCurrent) CHECK_STATUS(ListGetNext(&pThis->pCurrent));
	else CHECK_STATUS(ListGetHead(pThis->pEvents, &pThis->pCurrent));

	VALIDATE(pThis->pCurrent, eSTATUS_TIME_LINE_FINISHED);
	CHECK_STATUS(ListGetValue(pThis->pCurrent, (void**)ppEvent));

	return eSTATUS_COMMON_OK;
}
