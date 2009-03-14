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

#include "../inc/TimeLine.h"
#include "../inc/Macros.h"
#include "../inc/SortedList.h"

int __counter_TimeLine = 0;
int __counter_Event = 0;

typedef struct _Event
{
	double 			time;
} Event;

struct _TimeLine
{
	SortedList*    pEvents;
	double         nextTime;
	double         time;
	double         propMaxDelta;
	Boolean        propSmoothenMode;
	struct
	{
		EventTracker 	callback;
		void*			pArg;
	}				tracker;
};

IMPLEMENT_PROPERTY(TimeLine, MaxDelta, double, 0.1)
IMPLEMENT_PROPERTY(TimeLine, SmoothenMode, Boolean, TRUE);

Boolean TimeLineCleaner(Event *pEvent, TimeLine* pThis)
{
    DELETE(Event, pEvent);
	return FALSE;
}

Comparison TimeLineComparator(const Event* pLeft, const Event* pRight, const void* pUserArg)
{
	return (pLeft->time < pRight->time) ? LESS :
		   (pLeft->time > pRight->time) ? GREAT : EQUAL;
}

EStatus TimeLineNew(TimeLine** ppThis)
{
    BEGIN_FUNCTION;
	CONSTRUCT(ppThis, TimeLine);
	END_FUNCTION;
}

EStatus TimeLineDelete(TimeLine** ppThis)
{
    BEGIN_FUNCTION;
	DESTRUCT(ppThis, TimeLine);
	END_FUNCTION;
}

EStatus TimeLineInit(TimeLine* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	CLEAR(pThis);
	INIT_PROPERTY(TimeLine, MaxDelta, pThis);
	INIT_PROPERTY(TimeLine, SmoothenMode, pThis);
	return SortedListNew(&pThis->pEvents, (ItemComparator)&TimeLineComparator, NULL);
}

EStatus TimeLineDestroy(TimeLine* pThis)
{
    BEGIN_FUNCTION;
	VALIDATE_ARGUMENTS(pThis);
    CHECK(SortedListCleanUp(pThis->pEvents, (ItemFilter)&TimeLineCleaner, pThis));
    CHECK(SortedListDelete(&pThis->pEvents));
    END_FUNCTION;
	return eSTATUS_COMMON_OK;
}

EStatus TimeLineEvent(TimeLine* pThis, double time, const Packet* pPacket)
{
	Event* pEvent;
	EStatus ret;
	VALIDATE_ARGUMENTS(pThis && (time >= pThis->time));

	// Don't create events in the past
	if (pThis->time == time) return eSTATUS_COMMON_OK;

	if (pThis->nextTime > time) pThis->nextTime = time;

	pEvent = NEW(Event);

	if (pThis->tracker.callback) pThis->tracker.callback(time, pPacket, TRUE, pThis->tracker.pArg);

	pEvent->time = time;

	ret = SortedListAdd(pThis->pEvents, pEvent, TRUE);
	if (ret == eSTATUS_SORTED_LIST_ALREADY_EXISTS)
    {
	    DELETE(Event, pEvent);
	    return eSTATUS_COMMON_OK;
    }
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
	VALIDATE_ARGUMENTS(pThis);

	if (pThis->propSmoothenMode && (pThis->nextTime - pThis->time > pThis->propMaxDelta))
	{
	    pThis->time += pThis->propMaxDelta;
	    return eSTATUS_COMMON_OK;
	}

	if (pThis->time < pThis->nextTime)
	{
	    pThis->time = pThis->nextTime;
	    return eSTATUS_COMMON_OK;
	}

	SortedListPopFront(pThis->pEvents, &pEvent);
    if (pEvent)
    {
        pThis->nextTime = pEvent->time;

        if (pThis->propSmoothenMode && (pThis->nextTime - pThis->time > pThis->propMaxDelta)) pThis->time += pThis->propMaxDelta;
        else pThis->time = pThis->nextTime;

        DELETE(Event, pEvent);
        return eSTATUS_COMMON_OK;
    }
    return eSTATUS_TIME_LINE_FINISHED;
}

EStatus TimeLineGetTime(const TimeLine* pThis, double* pTime)
{
	GET_MEMBER(pTime, pThis, time);
}

EStatus TimeLineClear(TimeLine* pThis)
{
    Event* pEvent;
	VALIDATE_ARGUMENTS(pThis);
	CHECK(SortedListCleanUp(pThis->pEvents, (ItemFilter)&TimeLineCleaner, pThis));
	pThis->time = 0;
	pThis->nextTime = 0;
	pEvent = NEW(Event);
	pEvent->time = 0;
    return SortedListAdd(pThis->pEvents, pEvent, TRUE);
}

EStatus TimeLineGetLength(const TimeLine* pThis, double* pLength)
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
