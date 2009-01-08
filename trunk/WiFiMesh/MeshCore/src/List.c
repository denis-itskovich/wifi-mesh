#include "List.h"
#include "Macros.h"

struct _ListEntry
{
	ListEntry*	pNext;
	ListEntry*	pPrev;
	void*			pValue;
};

struct _List
{
	ListEntry*	pHead;
	ListEntry*	pTail;
	unsigned	count;
};


/** Used for items enumeration
 * \param pThis [in] pointer to instance
 * \param pEntry [in] item position in a list
 * \param pUserArg1 [in] user defined argument
 * \param pUserArg2 [in] user defined argument
 * \param pUserArg3 [in] user defined argument
 */
typedef Boolean (*EntryEnumerator) (List* pThis, ListEntry* pEntry, void* pUserArg1, void* pUserArg2, void* pUserArg3);

typedef struct _FindEnumeratorArgs
{
	ItemComparator	comparator;
	const void*		pValue;
	void*			pUserArg;
} FindEnumeratorArgs;

EStatus ListNew(List** ppThis)
{
	CONSTRUCT(ppThis, List);
}

EStatus ListDelete(List** ppThis)
{
	DESTRUCT(ppThis, List);
}

EStatus ListInit(List* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	CLEAR(pThis);
	return eSTATUS_COMMON_OK;
}

EStatus ListDestroy(List* pThis)
{
	ListEntry* pItem;
	VALIDATE_ARGUMENTS(pThis);

	while ((pItem = pThis->pTail))
	{
		CHECK(ListRemove(pThis, pItem));
	}

	return eSTATUS_COMMON_OK;
}

EStatus ListGetCount(List* pThis, unsigned* pCount)
{
	GET_MEMBER(pCount, pThis, count);
}

EStatus ListInsert(List* pThis, void* pValue)
{
	ListEntry* pTail;
	CHECK(ListGetTail(pThis, &pTail));
	return ListInsertAfter(pThis, pTail, pValue);
}

EStatus ListInsertAt(List* pThis, ListEntry* pPrev, ListEntry* pNext, void* pValue)
{
	ListEntry* pNewItem;
	VALIDATE_ARGUMENTS(pThis && pValue);
	pNewItem = NEW(ListEntry);

	pNewItem->pValue = pValue;
	pNewItem->pPrev = pPrev;
	pNewItem->pNext = pNext;

	if (pNewItem->pPrev) pPrev->pNext = pNewItem;
	if (pNewItem->pNext) pNext->pPrev = pNewItem;

	if (pNewItem->pNext == pThis->pHead) pThis->pHead = pNewItem;
	if (pNewItem->pPrev == pThis->pTail) pThis->pTail = pNewItem;

	++pThis->count;

	return eSTATUS_COMMON_OK;
}

EStatus ListInsertAfter(List* pThis, ListEntry* pItem, void* pValue)
{
	return ListInsertAt(pThis, pItem, (pItem) ? pItem->pNext : pThis->pHead, pValue);
}

EStatus ListInsertBefore(List* pThis, ListEntry* pItem, void* pValue)
{
	return ListInsertAt(pThis, (pItem) ? pItem->pPrev : pThis->pTail, pItem, pValue);
}

EStatus ListRemove(List* pThis, ListEntry* pEntry)
{
	VALIDATE_ARGUMENTS(pThis && pEntry);

	if (pEntry->pNext) pEntry->pNext->pPrev = pEntry->pPrev;
	else pThis->pTail = pEntry->pPrev;

	if (pEntry->pPrev) pEntry->pPrev->pNext = pEntry->pNext;
	else pThis->pHead = pEntry->pNext;

	DELETE(pEntry);

	--pThis->count;

	return eSTATUS_COMMON_OK;
}

EStatus ListGetBegin(List* pThis, ListEntry** ppIterator)
{
	VALIDATE_ARGUMENTS(pThis && ppIterator);
	*ppIterator = pThis->pHead;

	return eSTATUS_COMMON_OK;
}

EStatus ListGetEnd(List* pThis, ListEntry** ppIterator)
{
	VALIDATE_ARGUMENTS(pThis && ppIterator);
	*ppIterator = pThis->pTail;

	return eSTATUS_COMMON_OK;
}

EStatus ListGetNext(ListEntry** ppIterator)
{
	VALIDATE_ARGUMENTS(ppIterator && *ppIterator);
	*ppIterator = (*ppIterator)->pNext;

	return eSTATUS_COMMON_OK;
}

EStatus ListGetPrevious(ListEntry** ppIterator)
{
	VALIDATE_ARGUMENTS(ppIterator && *ppIterator);
	*ppIterator = (*ppIterator)->pPrev;

	return eSTATUS_COMMON_OK;
}

EStatus ListIsEmpty(List* pThis, Boolean* pIsEmpty)
{
	VALIDATE_ARGUMENTS(pThis && pIsEmpty);
	*pIsEmpty = (pThis->pHead != NULL) ? TRUE : FALSE;

	return eSTATUS_COMMON_OK;
}

EStatus ListGetValue(ListEntry* pEntry, void* ppValue)
{
	void** pptr = (void**)ppValue;
	VALIDATE_ARGUMENTS(pEntry && ppValue);
	*pptr = pEntry->pValue;

	return eSTATUS_COMMON_OK;
}

EStatus ListEnumerateEntries(List* pThis, ListEntry** ppEntry, EntryEnumerator enumerator, void* pUserArg1, void* pUserArg2, void* pUserArg3)
{
	ListEntry* pNextEntry;
	VALIDATE_ARGUMENTS(pThis && ppEntry);
	CHECK(ListGetBegin(pThis, ppEntry));

	while (*ppEntry)
	{
		pNextEntry = *ppEntry;
		CHECK(ListGetNext(&pNextEntry));
		if (enumerator(pThis, *ppEntry, pUserArg1, pUserArg2, pUserArg3) == FALSE) break;
		*ppEntry = pNextEntry;
	}

	return eSTATUS_COMMON_OK;
}

Boolean ListEnumeratorFinder(List* pThis, ListEntry* pEntry, void* pComparator, void* pValueRight, void* pUserArg)
{
	ItemComparator comparator = (ItemComparator)pComparator;
	return comparator(pEntry->pValue, pValueRight, pUserArg) != EQUAL ? TRUE : FALSE;
}

EStatus ListFind(List* pThis, ListEntry** ppEntry, ItemComparator comparator, const void* pValue, void* pUserArg)
{
	VALIDATE_ARGUMENTS(pThis && ppEntry && comparator && pValue);

	CHECK(ListEnumerateEntries(pThis, ppEntry, &ListEnumeratorFinder, comparator, (void*)pValue, pUserArg));
	VALIDATE(*ppEntry, eSTATUS_LIST_NOT_FOUND);

	return eSTATUS_COMMON_OK;
}

Boolean ListEnumeratorCleaner(List* pThis, ListEntry* pEntry, void* pFilter, void* pUserArg, void* pUnused)
{
	ItemFilter filter = (ItemFilter)pFilter;
	if (filter(pEntry->pValue, pUserArg) == FALSE)
	{
		ListRemove(pThis, pEntry);
	}
	return TRUE;
}

EStatus ListCleanUp(List* pThis, ItemFilter filter, void* pUserArg)
{
	ListEntry* pEntry;
	VALIDATE_ARGUMENTS(pThis && filter);
	CHECK(ListEnumerateEntries(pThis, &pEntry, &ListEnumeratorCleaner, filter, pUserArg, NULL));

	return eSTATUS_COMMON_OK;
}

Boolean ListEnumeratorEnumerator(List* pThis, ListEntry* pEntry, void* pEnumerator, void* pUserArg, void* pUnunsed)
{
	ItemEnumerator enumerator = (ItemEnumerator)pEnumerator;
	return enumerator(pEntry->pValue, pUserArg);
}

EStatus ListEnumerate(List* pThis, ItemEnumerator enumerator, void* pUserArg)
{
	ListEntry* pEntry;
	VALIDATE_ARGUMENTS(pThis && enumerator);
	CHECK(ListEnumerateEntries(pThis, &pEntry, &ListEnumeratorEnumerator, enumerator, pUserArg, NULL));

	return eSTATUS_COMMON_OK;
}