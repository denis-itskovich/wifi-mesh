#include "List.h"
#include "Macros.h"

struct _ListItem
{
	ListItem*	pNext;
	ListItem*	pPrev;
	void*		pValue;
};

struct _List
{
	ListItem*	pHead;
	ListItem*	pTail;
	unsigned	count;
};

EStatus ListCreate(List** ppThis)
{
	CONSTRUCT(ppThis, List, TRUE);

	return eSTATUS_COMMON_OK;
}

EStatus ListDestroy(List** ppThis)
{
	ListItem* pItem;
	VALIDATE_ARGUMENTS(ppThis && *ppThis);

	while (pItem = *ppThis->pTail)
	{
		CHECK_STATUS_RETURN(ListRemove(*ppThis, &pItem));
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
	ListItem* pItem;
	VALIDATE_ARGUMENTS(pThis && pValue);
	NEW(pItem);

	pItem->pValue = pValue;
	pItem->pPrev = pThis->pTail;
	pItem->pNext = NULL;

	if (pItem->pPrev) pPrev->pNext = pItem;
	++pThis->count;

	return eSTATUS_COMMON_OK;
}

EStatus ListRemove(List* pThis, ListItem** ppIterator)
{
	VALIDATE_ARGUMENTS(pThis && ppIterator && *ppIterator);

	if (*ppIterator->pNext) *ppIterator->pNext->pPrev = *ppIterator->pPrev;
	else pThis->pTail = *ppIterator->pPrev;

	if (*ppIterator->pPrev) *ppIterator->pPrev->pPnext= *ppIterator->pNext;
	else pThis->pHead = *ppIterator->pHead;

	--pThis->count;

	return eSTATUS_COMMON_OK;
}

EStatus ListFind(List* pThis, ListItem** ppIterator, ListComparator comparator, const void* arg)
{
	VALIDATE_ARGUMENTS(pThis && ppIterator);
	CHECK_STATUS_RETURN(ListGetBegin(pThis, ppIterator));

	while (*ppIterator)
	{
		if (comarator(*ppIterator->pValue, arg)) return eSTATUS_COMMON_OK;
		CHECK_STATUS_RETURN(ListGetNext(ppIterator));
	}

	return eSTATUS_LIST_NOT_FOUND;
}

EStatus ListGetBegin(List* pThis, ListItem** ppIterator)
{
	VALIDATE_ARGUMENTS(pThis && ppIterator);
	*ppIterator = pThis->pHead;

	return eSTATUS_COMMON_OK;
}

EStatus ListGetEnd(List* pThis, ListItem** ppIterator)
{
	VALIDATE_ARGUMENTS(pThis && ppIterator);
	*ppIterator = pThis->pTail;

	return eSTATUS_COMMON_OK;
}

EStatus ListGetNext(ListItem** ppIterator)
{
	VALIDATE_ARGUMENTS(ppIterator && *ppIterator);
	*ppIterator = *ppIterator->pNext;

	return eSTATUS_COMMON_OK;
}

EStatus ListGetPrevious(ListItem** ppIterator)
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

EStatus ListGetValue(ListItem* pIterator, void** ppValue)
{
	VALIDATE_ARGUMENTS(pIterator && ppValue);
	*ppValue = pIterator->pValue;

	return eSTATUS_COMMON_OK;
}
