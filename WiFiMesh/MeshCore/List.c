#include "List.h"
#include "Macros.h"

struct _ListItem
{
	ListPosition*	pNext;
	ListPosition*	pPrev;
	void*		pValue;
};

struct _List
{
	ListPosition*	pHead;
	ListPosition*	pTail;
	unsigned	count;
};

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

EStatus ListFind(List* pThis, ListPosition** ppIterator, ListComparator comparator, const void* arg)
{
	VALIDATE_ARGUMENTS(pThis && ppIterator);
	CHECK(ListGetBegin(pThis, ppIterator));

	while (*ppIterator)
	{
		if (comarator(*ppIterator->pValue, arg)) return eSTATUS_COMMON_OK;
		CHECK(ListGetNext(ppIterator));
	}

	return eSTATUS_LIST_NOT_FOUND;
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
