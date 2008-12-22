/**
 * \file SortedList.c
 *
 * Project: MeshCore
 * (C) Copyright 2008 Denis Itskovich
 *
 * \date 22/12/2008
 * \author Denis Itskovich
 */

#include "SortedList.h"
#include "Macros.h"

struct _SortedList
{
	List*			pList;
	ItemComparator	comparator;
};

EStatus SortedListNew(SortedList** ppThis, ItemComparator comparator)
{
	CONSTRUCT(ppThis, SortedList, comparator);
}

EStatus SortedListDelete(SortedList** ppThis)
{
	DESTRUCT(ppThis, SortedList);
}

EStatus SortedListInit(SortedList* pThis, ItemComparator comparator)
{
	VALIDATE_ARGUMENTS(pThis && comparator);
	CLEAR(pThis);
	pThis->comparator = comparator;
	return ListNew(&pThis->pList);
}

EStatus SortedListDestroy(SortedList* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	return ListDelete(&pThis->pList);
}

EStatus SortedListAdd(SortedList* pThis, void* pValue)
{
	ListEntry* pEntry;
	void* pRight;

	VALIDATE_ARGUMENTS(pThis && pValue);

	CHECK(SortedListGetHead(pThis, &pEntry));
	while (pEntry)
	{
		CHECK(SortedListGetValue(pEntry, &pRight));
		if (pThis->comparator(pRight, pValue) != GREAT)
		{
			return ListInsertBefore(pThis->pList, pEntry, pValue);
		}
		CHECK(SortedListGetNext(&pEntry));
	}

	return ListInsert(pThis->pList, pValue);
}

EStatus SortedListRemove(SortedList* pThis, ListEntry* pEntry)
{
	VALIDATE_ARGUMENTS(pThis && pEntry);
	return ListRemove(pThis->pList, pEntry);
}

EStatus SortedListGetHead(SortedList* pThis, ListEntry** ppEntry)
{
	VALIDATE_ARGUMENTS(pThis && ppEntry);
	return ListGetHead(pThis->pList, ppEntry);
}

EStatus SortedListGetTail(SortedList* pThis, ListEntry** ppEntry)
{
	VALIDATE_ARGUMENTS(pThis && ppEntry);
	return ListGetTail(pThis->pList, ppEntry);
}

EStatus SortedListGetNext(ListEntry** ppEntry)
{
	return ListGetNext(ppEntry);
}

EStatus SortedListGetPrevious(ListEntry** ppEntry)
{
	return ListGetPrevious(ppEntry);
}

EStatus SortedListGetValue(ListEntry* pEntry, void** ppValue)
{
	return ListGetValue(pEntry, ppValue);
}
