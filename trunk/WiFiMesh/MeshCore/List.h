#ifndef _WIFI_MESH_LIST_H
#define _WIFI_MESH_LIST_H

#include "Status.h"

typedef struct _ListItem ListItem;
typedef struct _List List;

typedef Boolean (*ListComparator)(const void* arg1, const void* arg2);

EStatus ListCreate(List** pThis);
EStatus ListDestroy(List** pThis);

EStatus ListGetCount(List* pThis, unsigned* pCount);
EStatus ListInsert(List* pThis, void* pValue);
EStatus ListRemove(List* pThis, ListItem** ppIterator);

EStatus ListFind(List* pThis, ListItem** ppIterator, ListComparator comparator, const void* arg);
EStatus ListGetBegin(List* pThis, ListItem** ppIterator);
EStatus ListGetEnd(List* pThis, ListItem** ppIterator);
EStatus ListGetNext(ListItem** ppIterator);
EStatus ListGetPrevious(ListItem** ppIterator);
EStatus ListGetValue(ListItem* pIterator, void** ppValue);

#endif // _WIFI_MESH_LIST_H
