#include "TimeLine.h"
#include "Macros.h"

struct _TimeLine
{
	List* 		pEvents;
	ListItem*	pCurrent;
};

EStatus TimeLineCreate(TimeLine** ppThis)
{
	CONSTRUCT(ppThis, TimeLine, TRUE);
	CHECK_STATUS_RETURN(ListCreate(&(*ppThis->pEvents)));

	return eSTATUS_COMMON_OK;
}

EStatus TimeLineDestroy(TimeLine** ppThis)
{
	VALIDATE_ARGUMENTS(ppThis && *ppThis);
	CHECK_STATUS_RETURN(ListDestroy(&(*ppThis->pEvents)));

	DELETE(*ppThis);
	return eSTATUS_COMMON_OK;
}

EStatus TimeLineAddEvent(TimeLine* pThis, Event* pEvent)
{
	VALIDATE_ARGUMENTS(pThis && pEvent);
	CHECK_STATUS_RETURN(ListInsert(pThis->pEvents, pEvent));

	return eSTATUS_COMMON_OK;
}

EStatus TimeLineGetNextEvent(TimeLine* pThis, Event** ppEvent)
{
	VALIDATE_ARGUMENTS(pThis && ppEvent);
	*ppEvent = NULL;

	if (pThis->pCurrent) CHECK_STATUS_RETURN(ListGetNext(&pThis->pCurrent));
	else CHECK_STATUS_RETURN(ListGetBegin(pThis->pEvents, &pThis->pCurrent));

	VALIDATE(pThis->pCurrent, eSTATUS_TIME_LINE_FINISHED);
	CHECK_STATUS_RETURN(ListGetValue(pThis->pCurrent, (void**)ppEvent));

	return eSTATUS_COMMON_OK;
}
