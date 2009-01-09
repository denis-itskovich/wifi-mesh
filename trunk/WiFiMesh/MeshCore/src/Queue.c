/*
 * Queue.c
 *
 *  Created on: 12/12/2008
 *      Author: denis
 */

#include "Queue.h"
#include "List.h"
#include "Macros.h"

struct _Queue
{
	List*	pList;
};

EStatus QueueNew(Queue** ppThis)
{
	CONSTRUCT(ppThis, Queue);
}

EStatus QueueDelete(Queue** ppThis)
{
	DESTRUCT(ppThis, Queue);
}

EStatus QueueInit(Queue* pThis)
{
	return ListNew(&pThis->pList);
}

EStatus QueueDestroy(Queue* pThis)
{
	return ListDelete(&pThis->pList);
}

EStatus QueuePush(Queue* pThis, void* pItem)
{
	ListEntry* pTail;
	CHECK(ListGetTail(pThis->pList, &pTail));
	return ListInsertAfter(pThis->pList, pTail, pItem);
}

EStatus QueuePeek(Queue* pThis, void* pItem)
{
	ListEntry* pHead;
	CHECK(ListGetHead(pThis->pList, &pHead));
	if (!pHead) return eSTATUS_QUEUE_EMPTY;

	return ListGetValue(pHead, pItem);
}

EStatus QueuePop(Queue* pThis, void* pItem)
{
	ListEntry* pHead;
	CHECK(QueuePeek(pThis, pItem));
	CHECK(ListGetHead(pThis->pList, &pHead));
	return ListRemove(pThis->pList, pHead);
}
