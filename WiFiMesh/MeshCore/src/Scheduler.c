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
	Message* pMessage;
	ListEntry* pEntry;
	VALIDATE_ARGUMENTS(pThis);

	do
	{
		CHECK(SortedListGetHead(pThis->pEntries, &pEntry));
		if (pEntry)
		{
			CHECK(SortedListGetValue(pEntry, &pMessage));
			CHECK(MessageDelete(&pMessage));
			CHECK(SortedListRemove(pThis->pEntries, pEntry));
		}
	} while (pEntry);

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
	return TimeLineMilestone(pThis->pTimeLine, time);
}

EStatus SchedulerGetMessage(Scheduler* pThis, Message** ppMessage)
{
	SchedulerEntry* pSchedulerEntry;
	ListEntry* pListEntry;
	double time;
	VALIDATE_ARGUMENTS(pThis && ppMessage);

	CHECK(TimeLineGetTime(pThis->pTimeLine, &time));

	*ppMessage = NULL;
	CHECK(SortedListGetHead(pThis->pEntries, &pListEntry));
	if (!pListEntry) return eSTATUS_SCHEDULER_NO_MESSAGES;

	CHECK(SortedListGetValue(pListEntry, &pSchedulerEntry));
	if (pSchedulerEntry->time > time) return eSTATUS_SCHEDULER_NO_MESSAGES;

	*ppMessage = pSchedulerEntry->pMessage;
	DELETE(pSchedulerEntry);

	return SortedListRemove(pThis->pEntries, pListEntry);
}

Boolean SchedulerCleaner(Message* pMessage, Scheduler* pScheduler)
{
	MessageDelete(&pMessage);
	return FALSE;
}

EStatus SchedulerClear(Scheduler* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	return SortedListCleanUp(pThis->pEntries, (ItemFilter)&SchedulerCleaner, pThis);
}