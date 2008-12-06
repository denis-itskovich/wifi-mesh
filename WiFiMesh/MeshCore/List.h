#ifndef _WIFI_MESH_LIST_H
#define _WIFI_MESH_LIST_H

#include "Status.h"

typedef struct _ListHeader
{
	struct _ListHeader *pNext, *pPrev;
} ListHeader;

EStatus ListCreate(ListHeader* pHeader);
EStatus ListInsert(ListHeader* pAfter, ListHeader* pItem);
EStatus ListRemove(ListHeader* pItem);

#define FOR_EACH(pHdr, operation) \
	while (pHdr) \
	{ \
		{ operation; } \
		pHdr = pHdr->pNext; \
	}

#endif // _WIFI_MESH_LIST_H
