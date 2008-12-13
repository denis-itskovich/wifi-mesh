/**
 * \file List.h
 *
 * Linked List module
 *
 * Represents generic linked list data structure
 * allows to insert, remove and look for items
 *
 * \see Queue.h, SortedList.h
 *
 * \author Denis Itskovich
 *   \date 1/12/2008
 */

#ifndef _WIFI_MESH_LIST_H
#define _WIFI_MESH_LIST_H

#include "Status.h"
#include "CommonTypes.h"

/// forward declaration of _ListItem (declared in List.c)
typedef struct _ListItem ListPosition;

/// forward declaration of _List (declared in List.c)
typedef struct _List List;

/** Compares between 2 values
 * \param arg1 [in] pointer to first item
 * \param arg2 [in] pointer to second item
 * \return value indicates relationship between items
 */
typedef Comparison (*ItemComparator)(const void* arg1, const void* arg2);

/** Allocates & initializes new List instance
 * \param ppThis [out] pointer to new instance will be placed at *ppThis
 */
EStatus ListNew(List** ppThis);

/** Deallocates & destroys List instance
 * \param ppThis [in, out] *ppThis must point to valid instance
 */
EStatus ListDelete(List** ppThis);

/** Initializes previously allocated instance
 * \param pThis [in] pointer to instance
 */
EStatus ListInit(List* pThis);

/** Deinitializes an instance without deallocating
 * \param pThis [in] pointer to instance
 */
EStatus ListDestroy(List* pThis);

/** Gets items count
 * \param pThis [in] pointer to instance
 * \param pCount [out] pointer to count
 */
EStatus ListGetCount(List* pThis, unsigned* pCount);

/** Inserts new item to the tail
 * \param pThis [in] pointer to instance
 * \param pValue [in] pointer to abstract value
 */
EStatus ListInsert(List* pThis, void* pValue);

/** Inserts new item \a after specified position
 * \param pThis [in] pointer to instance
 * \param pPosition [in] points to position in the list
 * \param pValue [in] pointer to abstract value
 */
EStatus ListInsertAfter(List* pThis, ListPosition* pPosition, void* pValue);

/** Inserts new item \a before specified position
 * \param pThis [in] pointer to instance
 * \param pPosition [in] points to position in the list
 * \param pValue [in] pointer to abstract value
 */
EStatus ListInsertBefore(List* pThis, ListPosition* pPosition, void* pValue);

/** Removes an item from specified position
 * \param pThis [in] pointer to instance
 * \param pPosition [in] points to position of an item to be removed
 */
EStatus ListRemove(List* pThis, ListPosition* pPosition);

/** Looks for an item in a list
 * \param pThis [in] pointer to instance
 * \param ppPosition [out] found item position will be stored at *ppPosition
 * \param comparator [in] pointer to item comparator function \sa ListComparator
 * \param pValue [in] item to look for (second argument of ListComparator)
 */
EStatus ListFind(List* pThis, ListPosition** ppPosition, ListComparator comparator, const void* pValue);

/** Gets a \a head position
 * \param pThis [in] pointer to instance
 * \param ppPosition [out] head position will be stored at *ppPosition
 */
EStatus ListGetHead(List* pThis, ListPosition** ppPosition);

/** Gets a \a tail position
 * \param pThis [in] pointer to instance
 * \param ppPosition [out] tail position will be stored at *ppPosition
 */
EStatus ListGetTail(List* pThis, ListPosition** ppPosition);

/** Gets a \a next to specified position
 * \param ppPosition [in, out] *ppPosition will be initialized with position next to *ppPosition
 */
EStatus ListGetNext(ListPosition** ppPosition);

/** Gets a \a previous to specified position
 * \param ppPosition [in, out] *ppPosition will be initialized with position previous to *ppPosition
 */
EStatus ListGetPrevious(ListPosition** ppPosition);

/** Gets a value of specified position
 * \param pPosition [in] position in a list
 * \param ppValue [out] *ppValue will be initialized with a pointer to value
 */
EStatus ListGetValue(ListPosition* pPosition, void** ppValue);

#endif // _WIFI_MESH_LIST_H
