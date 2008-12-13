/*
 * Queue.c
 *
 *  Created on: 12/12/2008
 *      Author: denis
 */

#include "Queue.h"
#include "List.h"

struct _Queue
{
	List	list;
};

EStatus QueueCreate(Queue** ppThis)
{
	return ListCreate((List**)ppThis);
}

EStatus QueueInit(Queue* pThis)
{
	return ListInit(&pThis->list);
}

EStatus QueueDispose(Queue** ppThis)
{
	return ListDispose((List**)ppThis);
}

EStatus QueueDestroy(Queue* pThis)
{
	return ListDestroy(&pThis->list);
}

EStatus QueuePush(Queue* pThis, void* pItem)
{
	ListPosition* pTail;
	CHECK_STATUS(ListGetTail(&pThis->list, &pTail));
	return ListInsertAfter(&pThis->list, pTail, pItem);
}

EStatus QueuePop(Queue* pThis, void** pItem)
{
	ListPosition* pHead;
	CHECK_STATUS(ListGetHead(&pThis->list, &pIter));
	CHECK_STATUS(ListGetValue(pIter, pHead));
	return ListRemove(pThis, pHead);
}
