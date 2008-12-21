#include "List.h"
#include "Macros.h"

struct _ListEntry
{
	ListPosition*	pNext;
	ListPosition*	pPrev;
	void*			pValue;
};

struct _List
{
	ListPosition*	pHead;
	ListPosition*	pTail;
	unsigned	count;
};


/** Used for items enumeration
 * \param pThis [in] pointer to instance
 * \param pPosition [in] item position in a list
 * \param pUserArg1 [in] user defined argument
 * \param pUserArg2 [in] user defined argument
 * \param pUserArg3 [in] user defined argument
 */
typedef Boolean (*PositionEnumerator) (List* pThis, ListPosition* pPosition, void* pUserArg1, void* pUserArg2, void* pUserArg3);

typedef struct _FindEnumeratorArgs
{
	ItemComparator	comparator;
	const void*		pValue;
	void*			pUserArg;
} FindEnumeratorArgs;

EStatus ListCreate(List** ppThis)
{
	CONSTRUCT(ppThis, List, TRUE);

	return eSTATUS_COMMON_OK;
}

EStatus ListDestroy(List** ppThis)
{
	ListPosition* pItem;
	VALIDATE_ARGUMENTS(ppThis && *ppThis);

	while (pItem = *ppThis->pTail)
	{
		CHECK(ListRemove(*ppThis, &pItem));
	}

	DELETE(*ppThis);

	return eSTATUS_COMMON_OK;
}

EStatus ListGetCount(List* pThis, unsigned* pCount)
{
	GET_MEMBER(pCount, pThis, count);
}

EStatus ListInsert(List* pThis, void* pValue)
{
	ListPosition* pTail;
	CHECK(ListGetTail(pThis, &pTail));
	return ListInsertAfter(pThis, pTail, pValue);
}

EStatus ListInsertAt(List* pThis, ListPosition* pPrev, ListPosition* pNext, void* pValue)
{
	ListPosition* pNewItem;
	VALIDATE_ARGUMENTS(pThis && pValue);
	NEW(pNewItem);

	pNewItem->pValue = pValue;
	pNewItem->pPrev = pPrev;
	pNewItem->pNext = pNext;

	if (pItem->pPrev) pPrev->pNext = pItem;
	if (pItem->pNext) pNext->pPrev = pItem;

	if (pItem->pNext == pThis->pHead) pHead = pItem;
	if (pItem->pPrev == pThis->pTail) pTail = pItem;

	if (!pHead || !pTail)
	{
		pHead = pTail = pItem;
	}

	++pThis->count;

	return eSTATUS_COMMON_OK;
}

EStatus ListInsertAfter(List* pThis, ListPosition* pItem, void* pValue)
{
	return ListInsertAt(pThis, pItem, (pItem) ? pItem->pNext : pThis->pHead);
}

EStatus ListInsertBefore(List* pThis, ListPosition* pItem, void* pValue)
{
	return ListInsertAt(pThis, (pItem) ? pItem->pPrev : pThis->pTail, pItem);
}

EStatus ListRemove(List* pThis, ListPosition* pIterator)
{
	VALIDATE_ARGUMENTS(pThis && pIterator);

	if (pIterator->pNext) pIterator->pNext->pPrev = pIterator->pPrev;
	else pThis->pTail = pIterator->pPrev;

	if (pIterator->pPrev) pIterator->pPrev->pPnext= pIterator->pNext;
	else pThis->pHead = pIterator->pHead;

	--pThis->count;

	return eSTATUS_COMMON_OK;
}

EStatus ListGetBegin(List* pThis, ListPosition** ppIterator)
{
	VALIDATE_ARGUMENTS(pThis && ppIterator);
	*ppIterator = pThis->pHead;

	return eSTATUS_COMMON_OK;
}

EStatus ListGetEnd(List* pThis, ListPosition** ppIterator)
{
	VALIDATE_ARGUMENTS(pThis && ppIterator);
	*ppIterator = pThis->pTail;

	return eSTATUS_COMMON_OK;
}

EStatus ListGetNext(ListPosition** ppIterator)
{
	VALIDATE_ARGUMENTS(ppIterator && *ppIterator);
	*ppIterator = *ppIterator->pNext;

	return eSTATUS_COMMON_OK;
}

EStatus ListGetPrevious(ListPosition** ppIterator)
{
	VALIDATE_ARGUMENTS(ppIterator && *ppIterator);
	*ppIterator = *ppIterator->pPrev;

	return eSTATUS_COMMON_OK;
}

EStatus ListIsEmpty(List* pThis, Boolean* pIsEmpty)
{
	VALIDATE_ARGUMENTS(pThis && pIsEmpty);
	*pIsEmpty = (pThis->pHead != NULL) ? TRUE : FALSE;

	return eSTATUS_COMMON_OK;
}

EStatus ListGetValue(ListPosition* pIterator, void** ppValue)
{
	VALIDATE_ARGUMENTS(pIterator && ppValue);
	*ppValue = pIterator->pValue;

	return eSTATUS_COMMON_OK;
}

EStatus ListEnumerateEntries(List* pThis, ListPosition** ppPosition, PositionEnumerator enumerator, void* pUserArg1, void* pUserArg2, void* pUserArg3)
{
	ListPosition* pNextPosition;
	VALIDATE_ARGUMENTS(pThis && ppPosition);
	CHECK(ListGetBegin(pThis, ppPosition));

	while (*ppPosition)
	{
		pNextPosition = *ppPosition;
		CHECK(ListGetNext(pNextPosition));
		if (enumerator(pThis, *ppPosition, pUserArg1, pUserArg2, pUserArg3) == FALSE) break;
		*ppPosition = pNextPosition;
	}

	return eSTATUS_COMMON_OK;
}

Boolean ListEnumeratorFinder(List* pThis, ListPosition* pPosition, void* pComparator, void* pValueRight, void* pUserArg)
{
	ItemComparator comparator = (ItemComparator)pComparator;
	return comparator(pPosition->pValue, pValueRight, pUserArg) != EQUAL ? TRUE : FALSE;
}

EStatus ListFind(List* pThis, ListPosition** ppPosition, ItemComparator comparator, const void* pValue, void* pUserArg)
{
	VALIDATE_ARGUMENTS(pThis && ppPosition && comparator && pValue);

	CHECK(ListEnumerateEntries(pThis, ppPosition, &ListEnumeratorFinder, comparator, (void*)pValue, pUserArg));
	VALIDATE(*ppPosition, eSTATUS_LIST_NOT_FOUND);

	return eSTATUS_COMMON_OK;
}

Boolean ListEnumeratorCleaner(List* pThis, ListPosition* pPosition, void* pFilter, void* pUserArg, void* pUnused)
{
	ItemFilter filter = (ItemFilter)pFilter;
	if (filter(pPosition->pValue, pUserArg) == FALSE)
	{
		ListRemove(pThis, pPosition);
	}
	return TRUE;
}

EStatus ListCleanUp(List* pThis, ItemFilter filter, void* pUserArg)
{
	ListPosition* pPosition;
	VALIDATE_ARGUMENTS(pThis && filter);
	CHECK(ListEnumerateEntries(pThis, &pPosition, &ListEnumeratorCleaner, filter, pUserArg, NULL));

	return eSTATUS_COMMON_OK;
}

Boolean ListEnumeratorEnumerator(List* pThis, ListPosition* pPosition, void* pEnumerator, void* pUserArg, void* pUnunsed)
{
	ItemEnumerator enumerator = (ItemEnumerator)pEnumerator;
	return enumerator(pPosition->pValue, pUserArg);
}

EStatus ListEnumerate(List* pThis, ItemEnumerator enumerator, void* pUserArg)
{
	ListPosition* pPosition;
	VALIDATE_ARGUMENTS(pThis && enumerator);
	CHECK(ListEnumerateEntries(pThis, &pPosition, &ListEnumeratorEnumerator, enumerator, pUserArg, NULL));

	return eSTATUS_COMMON_OK;
}
