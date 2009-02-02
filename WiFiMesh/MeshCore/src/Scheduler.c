/**
 * \file Scheduler.c
 *
 * Project: MeshCore
 * (C) Copyright 2008 Denis Itskovich
 *
 * \date 22/12/2008
 * \author Denis Itskovich
 */

#include "Scheduler.h"
#include "SortedList.h"
#include "Macros.h"

typedef struct _SchedulerEntry
{
	double		time;
	Message*	pMessage;
} SchedulerEntry;

struct _Scheduler
{
	TimeLine* 	pTimeLine;
	SortedList*	pEntries;
	ListEntry*	pCurrent;
	struct
	{
		SchedulerHandler	callback;
		void*				pArg;
	}			handler;
};

Comparison SchedulerComparator(const SchedulerEntry* pLeft, const SchedulerEntry* pRight, void* pUserArg)
{
	return (pLeft->time < pRight->time) ? LESS :
		   (pLeft->time > pRight->time) ? GREAT : EQUAL;
}

EStatus SchedulerNew(Scheduler** ppThis, TimeLine* pTimeLine)
{
	CONSTRUCT(ppThis, Scheduler, pTimeLine);
}

EStatus SchedulerDelete(Scheduler** ppThis)
{
	DESTRUCT(ppThis, Scheduler);
}

EStatus SchedulerInit(Scheduler* pThis, TimeLine* pTimeLine)
{
	VALIDATE_ARGUMENTS(pThis && pTimeLine);
	CLEAR(pThis);
	pThis->pTimeLine = pTimeLine;
	return SortedListNew(&pThis->pEntries, (ItemComparator)&SchedulerComparator, NULL);
}

EStatus SchedulerDestroy(Scheduler* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	CHECK(SchedulerClear(pThis));
	return SortedListDelete(&pThis->pEntries);
}

EStatus SchedulerPutMessage(Scheduler* pThis, Message* pMessage, double time)
{
	SchedulerEntry* pEntry;
	VALIDATE_ARGUMENTS(pThis && pMessage && (time >= 0));
	pEntry = NEW(SchedulerEntry);
	pEntry->time = time;
	pEntry->pMessage = pMessage;
	CHECK(SortedListAdd(pThis->pEntries, pEntry));
	CHECK(SortedListGetHead(pThis->pEntries, &pThis->pCurrent));
	if (pThis->handler.callback)
	{
		pThis->handler.callback(time, pMessage, TRUE, pThis->handler.pArg);
	}
	return TimeLineEvent(pThis->pTimeLine, time, pMessage);
}

EStatus SchedulerGetMessage(Scheduler* pThis, Message** ppMessage)
{
	SchedulerEntry* pSchedulerEntry;
	double time;
	VALIDATE_ARGUMENTS(pThis && ppMessage);

	CHECK(TimeLineGetTime(pThis->pTimeLine, &time));

	*ppMessage = NULL;
	if (!pThis->pCurrent) return eSTATUS_SCHEDULER_NO_MESSAGES;

	CHECK(SortedListGetValue(pThis->pCurrent, &pSchedulerEntry));
	if (pSchedulerEntry->time > time) return eSTATUS_SCHEDULER_NO_MESSAGES;

	CHECK(MessageClone(ppMessage, pSchedulerEntry->pMessage));
	CHECK(SortedListGetNext(&pThis->pCurrent));

	return eSTATUS_COMMON_OK;
}

Boolean SchedulerCleaner(SchedulerEntry* pEntry, Scheduler* pThis)
{
	MessageDelete(&pEntry->pMessage);
	if (pThis->handler.callback)
	{
		pThis->handler.callback(pEntry->time, pEntry->pMessage, FALSE, pThis->handler.pArg);
	}
	DELETE(pEntry);
	return FALSE;
}

EStatus SchedulerClear(Scheduler* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	CHECK(SortedListCleanUp(pThis->pEntries, (ItemFilter)&SchedulerCleaner, pThis));
	pThis->pCurrent = NULL;
	return eSTATUS_COMMON_OK;
}

EStatus SchedulerReset(Scheduler* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	CHECK(SortedListGetHead(pThis->pEntries, &pThis->pCurrent));
	return eSTATUS_COMMON_OK;
}

EStatus SchedulerRegisterHandler(Scheduler* pThis, SchedulerHandler handler, void* pUserArg)
{
	VALIDATE_ARGUMENTS(pThis);
	pThis->handler.callback = handler;
	pThis->handler.pArg = pUserArg;
	return eSTATUS_COMMON_OK;
}
