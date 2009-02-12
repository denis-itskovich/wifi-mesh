/*********************************************************************************
WiFi Mesh Simulator PRO
Copyright (C) 2009 Denis Itskovich

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*********************************************************************************/

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
typedef struct _ListEntry ListEntry;

/// forward declaration of _List (declared in List.c)
typedef struct _List List;

/** Compares between 2 values
 * \param pValueLeft [in] pointer to first item
 * \param pValueRight [in] pointer to second item
 * \param pUserArg [in] user defined argument
 * \return value indicates relationship between items
 * \return LESS if *pValueLeft < *pValueRight
 * \return EQUAL if *pValueLeft == *pValueRight
 * \return GREAT if *pValueLeft > *pValueRight
 * \sa ListFind
 */
typedef Comparison (*ItemComparator)(const void* pValueLeft, const void* pValueRight, void* pUserArg);

/** Decides whether to leave to remove an item from list
 * \param pValue [in] pointer to value
 * \param pUserArg [in] user defined argument
 * \return TRUE if an item should be leaved
 * \return FALSE if an item should be removed
 * \sa ListCleanUp
 */
typedef Boolean (*ItemFilter) (void* pValue, void* pUserArg);

/** Used for items enumeration
 * \param pValue [in] pointer to value
 * \param pUserArg [in] user defined argument
 * \return TRUE if an enumeration should be continued
 * \return FALSE if an enumeration should be stopped
 * \sa ListEnumerate
 */
typedef Boolean (*ItemEnumerator) (const void* pValue, void* pUserArg);

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



/** Inserts new item after the tail
 * \param pThis [in] pointer to instance
 * \param pValue [in] pointer to abstract value
 */
EStatus ListPushBack(List* pThis, void* pValue);

/** Inserts new item before the head
 * \param pThis [in] pointer to instance
 * \param pValue [in] pointer to abstract value
 */
EStatus ListPushFront(List* pThis, void* pValue);

/** Inserts new item \a after specified position
 * \param pThis [in] pointer to instance
 * \param pEntry [in] points to position in the list
 * \param pValue [in] pointer to abstract value
 */
EStatus ListInsertAfter(List* pThis, ListEntry* pEntry, void* pValue);

/** Inserts new item \a before specified position
 * \param pThis [in] pointer to instance
 * \param pEntry [in] points to position in the list
 * \param pValue [in] pointer to abstract value
 */
EStatus ListInsertBefore(List* pThis, ListEntry* pEntry, void* pValue);

/** Removes tail item from the list
 * \param pThis [in] pointer to instance
 */
EStatus ListPopBack(List* pThis);

/** Removes head item from the list
 * \param pThis [in] pointer to instance
 */
EStatus ListPopFront(List* pThis);

/** Removes an item from specified position
 * \param pThis [in] pointer to instance
 * \param pEntry [in] points to position of an item to be removed
 */
EStatus ListRemove(List* pThis, ListEntry* pEntry);

/** Gets a \a head position
 * \param pThis [in] pointer to instance
 * \param ppEntry [out] head position will be stored at *ppEntry
 * \return NULL if a list is empty, otherwise proper list entry
 */
EStatus ListGetHead(List* pThis, ListEntry** ppEntry);

/** Gets a \a tail entry
 * \param pThis [in] pointer to instance
 * \param ppEntry [out] tail position will be stored at *ppEntry
 * \return NULL if a list is empty, otherwise proper list entry
 */
EStatus ListGetTail(List* pThis, ListEntry** ppEntry);

/** Gets a \a next to specified position
 * \param ppEntry [in, out] *ppEntry will be initialized with position next to *ppEntry
 */
EStatus ListGetNext(ListEntry** ppEntry);

/** Gets a \a previous to specified position
 * \param ppEntry [in, out] *ppEntry will be initialized with position previous to *ppEntry
 */
EStatus ListGetPrevious(ListEntry** ppEntry);

/** Gets a value of specified position
 * \param pEntry [in] position in a list
 * \param ppValue [out] *ppValue will be initialized with a pointer to value
 */
EStatus ListGetValue(ListEntry* pEntry, void* ppValue);

/** Looks for an item in a list
 * \param pThis [in] pointer to instance
 * \param ppEntry [out] found item position will be stored at *ppEntry
 * \param comparator [in] pointer to item comparator function
 * \param pValue [in] item to look for (pValueRight argument of ItemComparator)
 * \param pUserArg [in] user defined argument of comparator function
 * \sa ItemComparator
 */
EStatus ListFind(List* pThis, ListEntry** ppEntry, ItemComparator comparator, const void* pValue, void* pUserArg);

/** Cleans a list according to filter function
 * \param pThis [in] pointer to instance
 * \param filter [in] pointer to item filter function
 * \param pUserArg [in] user defined argument of filter function
 * \sa ItemFilter
 */
EStatus ListCleanUp(List* pThis, ItemFilter filter, void* pUserArg);

/** Cleans a list according to filter function
 * \param pThis [in] pointer to instance
 * \param enumerator [in] pointer to item enumerator function
 * \param pUserArg [in] user defined argument of filter function
 * \sa ItemEnumerator
 */
EStatus ListEnumerate(List* pThis, ItemEnumerator enumerator, void* pUserArg);

#endif // _WIFI_MESH_LIST_H
