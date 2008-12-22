#include "TimeLine.h"
#include "Macros.h"
#include "List.h"

struct _TimeLine
{
	List* 		pEvents;
	ListEntry*	pCurrent;
};

EStatus TimeLineNew(TimeLine** ppThis)
{
	CONSTRUCT(ppThis, TimeLine);
}

EStatus TimeLineInit(TimeLine* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	CLEAR(pThis);
	CHECK(ListNew(&pThis->pEvents));

	return eSTATUS_COMMON_OK;
}

EStatus TimeLineDelete(TimeLine** ppThis)
{
	DESTRUCT(ppThis, TimeLine);
}

EStatus TimeLineDestroy(TimeLine* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	CHECK(ListDelete(&pThis->pEvents));
	return eSTATUS_COMMON_OK;
}

EStatus TimeLineAddEvent(TimeLine* pThis, Event* pEvent)
{
	VALIDATE_ARGUMENTS(pThis && pEvent);
	CHECK(ListInsert(pThis->pEvents, pEvent));

	return eSTATUS_COMMON_OK;
}

EStatus TimeLineGetNextEvent(TimeLine* pThis, Event** ppEvent)
{
	VALIDATE_ARGUMENTS(pThis && ppEvent);
	*ppEvent = NULL;

	if (pThis->pCurrent) CHECK(ListGetNext(&pThis->pCurrent));
	else CHECK(ListGetHead(pThis->pEvents, &pThis->pCurrent));

	VALIDATE(pThis->pCurrent, eSTATUS_TIME_LINE_FINISHED);
	CHECK(ListGetValue(pThis->pCurrent, (void**)ppEvent));

	return eSTATUS_COMMON_OK;
}
