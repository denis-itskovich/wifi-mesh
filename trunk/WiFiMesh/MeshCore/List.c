#include "List.h"
#include <stdlib.h>

EStatus ListCreate(ListHeader* pHeader)
{
	if (!pHeader) return eSTATUS_LIST_INVALID_HEADER;
	pHeader->pNext = pHeader->pPrev = NULL;

	return eSTATUS_COMMON_OK;
}

EStatus ListInsert(ListHeader* pAfter, ListHeader* pItem)
{
	if (!pItem) return eSTATUS_LIST_INVALID_ITEM;

	pItem->pPrev = pAfter;
	if (pAfter)
	{
		pItem->pNext = pAfter->pNext;
		if (pItem->pNext) pItem->pNext->pPrev = pItem;
		pAfter->pNext = pItem;
	}

	return eSTATUS_COMMON_OK;
}

EStatus ListRemove(ListHeader* pItem)
{
	if (!pItem) return eSTATUS_LIST_INVALID_ITEM;

	if (pItem->pPrev) pItem->pPrev->pNext = pItem->pNext;
	if (pItem->pNext) pItem->pNext->pPrev = pItem->pPrev;
	pItem->pNext = pItem->pPrev = NULL;

	return eSTATUS_COMMON_OK;
}
