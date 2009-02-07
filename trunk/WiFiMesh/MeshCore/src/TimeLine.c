#include "TimeLine.h"
#include "Macros.h"
#include "SortedList.h"

typedef struct _Event
{
	double 			time;
	const Packet*	pPacket;
} Event;

struct _TimeLine
{
	SortedList*		pEvents;
	ListEntry*		pCurrent;
	double			time;
	struct
	{
		EventTracker 	callback;
		void*			pArg;
	}				tracker;
};

Boolean TimeLineCleaner(Event *pEvent, TimeLine* pThis)
{
	if (pEvent)
	{
		if (pThis->tracker.callback) pThis->tracker.callback(pEvent->time, pEvent->pPacket, FALSE, pThis->tracker.pArg);
		DELETE(pEvent);
	}
	return FALSE;
}

Comparison TimeLineComparator(const Event* pLeft, const Event* pRight, const void* pUserArg)
{
	return (pLeft->time < pRight->time) ? LESS :
		   (pLeft->time > pRight->time) ? GREAT : EQUAL;
}

EStatus TimeLineNew(TimeLine** ppThis)
{
	CONSTRUCT(ppThis, TimeLine);
}

EStatus TimeLineDelete(TimeLine** ppThis)
{
	DESTRUCT(ppThis, TimeLine);
}

EStatus TimeLineInit(TimeLine* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	CLEAR(pThis);
	return SortedListNew(&pThis->pEvents, (ItemComparator)&TimeLineComparator, NULL);
}

EStatus TimeLineDestroy(TimeLine* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	return SortedListDelete(&pThis->pEvents);
}

EStatus TimeLineEvent(TimeLine* pThis, double time, const Packet* pPacket)
{
	Event* pEvent;
	VALIDATE_ARGUMENTS(pThis && (time >= 0));
	pEvent = NEW(Event);

	if (pThis->tracker.callback) pThis->tracker.callback(time, pPacket, TRUE, pThis->tracker.pArg);

	pEvent->time = time;
	pEvent->pPacket = pPacket;

	CHECK(SortedListAdd(pThis->pEvents, pEvent));
	return eSTATUS_COMMON_OK;
}

EStatus TimeLineRelativeEvent(TimeLine* pThis, double timeDelta, const Packet* pPacket)
{
	VALIDATE_ARGUMENTS(pThis && (timeDelta > 0));
	return TimeLineEvent(pThis, pThis->time + timeDelta, pPacket);
}

EStatus TimeLineNext(TimeLine* pThis)
{
	Event* pEvent;
	double time = 0;
	VALIDATE_ARGUMENTS(pThis);

	if (!pThis->pCurrent) return eSTATUS_TIME_LINE_FINISHED;

	do
	{
		CHECK(SortedListGetNext(&pThis->pCurrent));
		if (pThis->pCurrent)
		{
			CHECK(SortedListGetValue(pThis->pCurrent, &pEvent));
			time = pEvent->time;
		}
	} while (pThis->pCurrent && (time == pThis->time));
	pThis->time = time;

	return eSTATUS_COMMON_OK;
}

EStatus TimeLineGetTime(TimeLine* pThis, double* pTime)
{
	GET_MEMBER(pTime, pThis, time);
}

EStatus TimeLineClear(TimeLine* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	CHECK(SortedListCleanUp(pThis->pEvents, (ItemFilter)&TimeLineCleaner, pThis));
	pThis->time = 0;
	pThis->pCurrent = 0;
	return eSTATUS_COMMON_OK;
}

EStatus TimeLineGetLength(TimeLine* pThis, double* pLength)
{
	double* pResult;
	ListEntry* pEntry;
	VALIDATE_ARGUMENTS(pThis && pLength);
	CHECK(SortedListGetTail(pThis->pEvents, &pEntry));
	*pLength = 0;
	if (pEntry)
	{
		CHECK(ListGetValue(pEntry, &pResult));
		if (pResult) *pLength = *pResult;
	}
	return eSTATUS_COMMON_OK;
}

EStatus TimeLineReset(TimeLine* pThis)
{
	Event* pEvent;
	VALIDATE_ARGUMENTS(pThis);
	CHECK(SortedListGetHead(pThis->pEvents, &pThis->pCurrent));

	if (pThis->pCurrent)
	{
		CHECK(SortedListGetValue(pThis->pCurrent, &pEvent));
		pThis->time = pEvent->time;
	}
	else pThis->time = 0.0;

	return eSTATUS_COMMON_OK;
}

EStatus TimeLineSetEventTracker(TimeLine* pThis, EventTracker tracker, void* pUserArg)
{
	VALIDATE_ARGUMENTS(pThis);
	pThis->tracker.callback = tracker;
	pThis->tracker.pArg = pUserArg;
	return eSTATUS_COMMON_OK;
}
