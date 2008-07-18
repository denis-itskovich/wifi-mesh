#ifndef _WIFI_MESH_LIST_H
#define _WIFI_MESH_LIST_H

#include "Defs.h"

typedef struct _ListHeader
{
	struct _ListHeader *pNext, *pPrev;
} ListHeader;

EStatus ListCreate(ListHeader* pHeader);
EStatus ListInsert(ListHeader* pAfter, ListHeader* pItem);
EStatus ListRemove(ListHeader* pItem);

#endif // _WIFI_MESH_LIST_H
