#include "TimeLine.h"
#include "Macros.h"
#include "SortedList.h"

struct _TimeLine
{
	SortedList*	pMilestones;
	double		time;
};

Comparison TimeLineComparator(const double* pLeft, const double* pRight, const void* pUserArg)
{
	return (*pLeft < *pRight) ? LESS :
		   (*pLeft > *pRight) ? GREAT : EQUAL;
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
	return SortedListNew(&pThis->pMilestones, (ItemComparator)&TimeLineComparator, NULL);
}

EStatus TimeLineDestroy(TimeLine* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	return SortedListDelete(&pThis->pMilestones);
}

EStatus TimeLineMilestone(TimeLine* pThis, double time)
{
	double* pTime;
	VALIDATE_ARGUMENTS(pThis && (time >= 0));
	pTime = NEW(double);
	*pTime = time;
	return SortedListAdd(pThis->pMilestones, pTime);
}

EStatus TimeLineRelativeMilestone(TimeLine* pThis, double timeDelta)
{
	VALIDATE_ARGUMENTS(pThis && (timeDelta > 0));
	return TimeLineMilestone(pThis, pThis->time + timeDelta);
}

EStatus TimeLineNext(TimeLine* pThis)
{
	double* pTime;
	ListEntry* pEntry;
	VALIDATE_ARGUMENTS(pThis);

	do
	{
		CHECK(SortedListGetHead(pThis->pMilestones, &pEntry));
		SortedListGetValue(pEntry, (void**)&pTime);
	} while (pEntry && (*pTime == pThis->time));

	if (!pEntry) return eSTATUS_TIME_LINE_FINISHED;
	pThis->time = *pTime;

	DELETE(pTime);
	return eSTATUS_COMMON_OK;
}

EStatus TimeLineGetTime(TimeLine* pThis, double* pTime)
{
	GET_MEMBER(pTime, pThis, time);
}
