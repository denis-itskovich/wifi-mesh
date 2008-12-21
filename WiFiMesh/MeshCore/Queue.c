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
	List	list;
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
	return ListInit(&pThis->list);
}

EStatus QueueDestroy(Queue* pThis)
{
	return ListDestroy(&pThis->list);
}

EStatus QueuePush(Queue* pThis, void* pItem)
{
	ListPosition* pTail;
	CHECK(ListGetTail(&pThis->list, &pTail));
	return ListInsertAfter(&pThis->list, pTail, pItem);
}

EStatus QueuePop(Queue* pThis, void** pItem)
{
	ListPosition* pHead;
	CHECK(QueuePeek(pThis, pItem));
	CHECK(ListGetHead(&pThis->list, &pIter));
	return ListRemove(pThis, pHead);
}

EStatus QueuePeek(Queue* pThis, void** pItem)
{
	ListPosition* pHead;
	CHECK(ListGetHead(&pThis->list, &pIter));
	if (!pHead) return eSTATUS_QUEUE_EMPTY;

	return ListGetValue(pIter, pHead);
}
