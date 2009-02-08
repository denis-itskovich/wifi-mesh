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
	void*			pUserArg;
};

EStatus SortedListNew(SortedList** ppThis, ItemComparator comparator, void* pUserArg)
{
	CONSTRUCT(ppThis, SortedList, comparator, pUserArg);
}

EStatus SortedListDelete(SortedList** ppThis)
{
	DESTRUCT(ppThis, SortedList);
}

EStatus SortedListInit(SortedList* pThis, ItemComparator comparator, void* pUserArg)
{
	VALIDATE_ARGUMENTS(pThis && comparator);
	CLEAR(pThis);
	pThis->comparator = comparator;
	pThis->pUserArg = pUserArg;
	return ListNew(&pThis->pList);
}

EStatus SortedListDestroy(SortedList* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	return ListDelete(&pThis->pList);
}

EStatus SortedListAdd(SortedList* pThis, void* pValue, Boolean unique)
{
	ListEntry* pEntry;
	void* pRight;
	Comparison rel;

	VALIDATE_ARGUMENTS(pThis && pValue);

	CHECK(SortedListGetHead(pThis, &pEntry));
	while (pEntry)
	{
		CHECK(SortedListGetValue(pEntry, &pRight));
		rel = pThis->comparator(pValue, pRight, pThis->pUserArg);
		if (rel != GREAT)
		{
		    if (unique && (rel == EQUAL)) return eSTATUS_SORTED_LIST_ALREADY_EXISTS;
			return ListInsertBefore(pThis->pList, pEntry, pValue);
		}
		CHECK(SortedListGetNext(&pEntry));
	}

	return ListPushBack(pThis->pList, pValue);
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

EStatus SortedListGetValue(ListEntry* pEntry, void* ppValue)
{
	return ListGetValue(pEntry, ppValue);
}

EStatus SortedListCleanUp(SortedList* pThis, ItemFilter filter, void* pUserArg)
{
	VALIDATE_ARGUMENTS(pThis);
	return ListCleanUp(pThis->pList, filter, pUserArg);
}

EStatus SortedListFind(SortedList* pThis, ListEntry** ppEntry, ItemComparator comparator, const void* pValue, void* pUserArg)
{
	VALIDATE_ARGUMENTS(pThis);
	return ListFind(pThis->pList, ppEntry, comparator, pValue, pUserArg);
}

EStatus SortedListEnumerate(SortedList* pThis, ItemEnumerator enumerator, void* pUserArg)
{
	VALIDATE_ARGUMENTS(pThis);
	return ListEnumerate(pThis->pList, enumerator, pUserArg);
}
