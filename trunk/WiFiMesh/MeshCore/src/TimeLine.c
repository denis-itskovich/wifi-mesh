/*********************************************************************************
WiFi Mesh Simulator PRO
Copyright (C) 2009 Denis Itskovich

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*********************************************************************************/

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
	CHECK(SortedListNew(&pThis->pEvents, (ItemComparator)&TimeLineComparator, NULL));
	return TimeLineEvent(pThis, 0.0, NULL);
}

EStatus TimeLineDestroy(TimeLine* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
    CHECK(SortedListCleanUp(pThis->pEvents, (ItemFilter)&TimeLineCleaner, pThis));
	return SortedListDelete(&pThis->pEvents);
}

EStatus TimeLineEvent(TimeLine* pThis, double time, const Packet* pPacket)
{
	Event* pEvent;
	EStatus ret;
	VALIDATE_ARGUMENTS(pThis && (time >= 0));
	pEvent = NEW(Event);

	if (pThis->tracker.callback) pThis->tracker.callback(time, pPacket, TRUE, pThis->tracker.pArg);

	pEvent->time = time;
	pEvent->pPacket = pPacket;

	ret = SortedListInsert(pThis->pEvents, pThis->pCurrent, pEvent, TRUE);
	if (ret == eSTATUS_SORTED_LIST_ALREADY_EXISTS) return eSTATUS_COMMON_OK;
	return ret;
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

    CHECK(SortedListGetNext(&pThis->pCurrent));
    if (pThis->pCurrent)
    {
        CHECK(SortedListGetValue(pThis->pCurrent, &pEvent));
        time = pEvent->time;
    }
	pThis->time = time;

	return eSTATUS_COMMON_OK;
}

EStatus TimeLineGetTime(const TimeLine* pThis, double* pTime)
{
	GET_MEMBER(pTime, pThis, time);
}

EStatus TimeLineClear(TimeLine* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	CHECK(SortedListCleanUp(pThis->pEvents, (ItemFilter)&TimeLineCleaner, pThis));
	pThis->pCurrent = NULL;
	pThis->time = 0;
	CHECK(TimeLineEvent(pThis, 0.0, NULL));
    CHECK(SortedListGetHead(pThis->pEvents, &pThis->pCurrent));
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

EStatus TimeLineSetEventTracker(TimeLine* pThis, EventTracker tracker, void* pUserArg)
{
	VALIDATE_ARGUMENTS(pThis);
	pThis->tracker.callback = tracker;
	pThis->tracker.pArg = pUserArg;
	return eSTATUS_COMMON_OK;
}
