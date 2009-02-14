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

/**
 * @file Scheduler.c
 *
 * Project: MeshCore
 * (C) Copyright 2008 Denis Itskovich
 *
 * @date 22/12/2008
 * @author Denis Itskovich
 */

#include "Scheduler.h"
#include "SortedList.h"
#include "Macros.h"

typedef struct _SchedulerEntry
{
    double      time;
    Packet*     pPacket;
    Boolean		bWasIssued;
    Boolean		bWasDelivered;
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

EStatus SchedulerInvokeHandler(Scheduler* pThis, const SchedulerEntry* pEntry, ESchedulerEvent event);
Boolean SchedulerCleaner(SchedulerEntry* pEntry, Scheduler* pThis);
Boolean SchedulerResetter(SchedulerEntry* pEntry, Scheduler* pThis);
Comparison SchedulerComparator(const SchedulerEntry* pLeft, const SchedulerEntry* pRight, void* pUserArg);

Comparison SchedulerComparator(const SchedulerEntry* pLeft, const SchedulerEntry* pRight, void* pUserArg)
{
	return (pLeft->time < pRight->time) ? LESS :
		   (pLeft->time > pRight->time) ? GREAT : EQUAL;
}

Boolean SchedulerResetter(SchedulerEntry* pEntry, Scheduler* pThis)
{
	pEntry->bWasIssued = FALSE;
	pEntry->bWasDelivered = FALSE;
	TimeLineEvent(pThis->pTimeLine, pEntry->time, pEntry->pPacket);
	return TRUE;
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

EStatus SchedulerInvokeHandler(Scheduler* pThis, const SchedulerEntry* pEntry, ESchedulerEvent event)
{
	VALIDATE_ARGUMENTS(pThis && pEntry);
	if (pThis->handler.callback)
	{
		pThis->handler.callback(pEntry->time, pEntry->pPacket, event, pThis->handler.pArg);
	}
	return eSTATUS_COMMON_OK;
}

EStatus SchedulerPutPacket(Scheduler* pThis, Packet* pPacket, double time)
{
	SchedulerEntry* pEntry;
	VALIDATE_ARGUMENTS(pThis && pPacket && (time >= 0));

	pEntry = NEW(SchedulerEntry);
	pEntry->time = time;
	pEntry->pPacket = pPacket;
	CHECK(SortedListAdd(pThis->pEntries, pEntry, FALSE));
	CHECK(SortedListGetHead(pThis->pEntries, &pThis->pCurrent));
	CHECK(SchedulerInvokeHandler(pThis, pEntry, eSCHEDULE_ADDED));
	return TimeLineEvent(pThis->pTimeLine, time, pPacket);
}

EStatus SchedulerGetPacket(Scheduler* pThis, Packet** ppPacket)
{
	SchedulerEntry* pSchedulerEntry;
	double time;
	VALIDATE_ARGUMENTS(pThis && ppPacket);

	CHECK(TimeLineGetTime(pThis->pTimeLine, &time));

	*ppPacket = NULL;
	if (!pThis->pCurrent) return eSTATUS_SCHEDULER_NO_PACKETS;

	CHECK(SortedListGetValue(pThis->pCurrent, &pSchedulerEntry));
	if (pSchedulerEntry->time > time) return eSTATUS_SCHEDULER_NO_PACKETS;

	CHECK(PacketClone(ppPacket, pSchedulerEntry->pPacket));
	CHECK(SortedListGetNext(&pThis->pCurrent));

	pSchedulerEntry->bWasIssued = TRUE;

	return eSTATUS_COMMON_OK;
}

Boolean SchedulerCleaner(SchedulerEntry* pEntry, Scheduler* pThis)
{
    CHECK(SchedulerInvokeHandler(pThis, pEntry, eSCHEDULE_REMOVED));
	PacketDelete(&pEntry->pPacket);
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
	CHECK(SortedListEnumerate(pThis->pEntries, (ItemEnumerator)&SchedulerResetter, pThis));
	return eSTATUS_COMMON_OK;
}

EStatus SchedulerRegisterHandler(Scheduler* pThis, SchedulerHandler handler, void* pUserArg)
{
	VALIDATE_ARGUMENTS(pThis);
	pThis->handler.callback = handler;
	pThis->handler.pArg = pUserArg;
	return eSTATUS_COMMON_OK;
}

Boolean SchedulerDumpEntry(const SchedulerEntry* pEntry, void* pArg)
{
    DUMP_PRINT("SchedulerEntry: [time=%f]", pEntry->time);
    return TRUE;
}

EStatus SchedulerDump(const Scheduler* pThis)
{
    CHECK(SortedListEnumerate(pThis->pEntries, (ItemEnumerator)&SchedulerDumpEntry, NULL));
    return eSTATUS_COMMON_OK;
}
