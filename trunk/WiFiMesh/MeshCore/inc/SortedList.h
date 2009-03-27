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
 * @file SortedList.h
 *
 * Sorted Linked List module
 *
 * Represents a sorted list
 * @see List.h, Queue.h
 *
 * @author Denis Itskovich
 * @date   13/12/2008
 */

#ifndef WIFI_MESH_SORTEDLIST_H_
#define WIFI_MESH_SORTEDLIST_H_

#include "Status.h"
#include "List.h"

typedef struct _SortedList SortedList; ///< forward declaration of SortedList

/** Allocates & initializes new SortedList instance
 * @param ppThis [out] pointer to new instance will be placed at *ppThis
 * @param comparator [in] pointer to comparison function
 * @param pUserArg [in] pointer to user-defined argument of comparison function
 */
EStatus SortedListNew(SortedList** ppThis, ItemComparator comparator, void* pUserArg);

/** Deallocates & destroys SortedList instance
 * @param ppThis [in, out] *ppThis must point to valid instance
 */
EStatus SortedListDelete(SortedList** ppThis);

/** Initializes previously allocated instance
 * @param pThis [in] pointer to instance
 * @param comparator [in] pointer to comparison function
 * @param pUserArg [in] pointer to user-defined argument of comparison function
 */
EStatus SortedListInit(SortedList* pThis, ItemComparator comparator, void* pUserArg);

/** Deinitializes an instance without deallocating
 * @param pThis [in] pointer to instance
 */
EStatus SortedListDestroy(SortedList* pThis);

/** Inserts a new item, while starting search from specified point
 * @param pThis [in] pointer to instance
 * @param pEntry [in] list entry to start the search from (NULL for the first item)
 * @param pValue [in] pointer to new item
 * @param unique [in] if TRUE adding non-unique item will fail with status eSTATUS_SORTED_LIST_ALREADY_EXISTS (see EStatus)
 */
EStatus SortedListInsert(SortedList* pThis, ListEntry* pEntry, void* pValue, Boolean unique);

/** Adds a new item into the sorted list
 * @param pThis [in] pointer to instance
 * @param pValue [in] pointer to new item
 * @param unique [in] if TRUE adding non-unique item will fail with status eSTATUS_SORTED_LIST_ALREADY_EXISTS (see EStatus)
 */
EStatus SortedListAdd(SortedList* pThis, void* pValue, Boolean unique);

/** Removes an item from the list
 * @param pThis [in] pointer to instance
 * @param pEntry [in] pointer to entry, which should be removed
 */
EStatus SortedListRemove(SortedList* pThis, ListEntry* pEntry);

/** Gets an entry of the head
 * @param pThis [in] pointer to instance
 * @param ppEntry [out] head entry will be stored in *ppEntry
 */
EStatus SortedListGetHead(SortedList* pThis, ListEntry** ppEntry);

/** Gets an entry of the tail
 * @param pThis [in] pointer to instance
 * @param ppEntry [out] tail entry will be stored in *ppEntry
 */
EStatus SortedListGetTail(SortedList* pThis, ListEntry** ppEntry);

/** Gets an entry of the next to specified item
 * @param ppEntry [in, out] next entry will be stored in *ppEntry
 */
EStatus SortedListGetNext(ListEntry** ppEntry);

/** Gets an entry of the previous to specified item
 * @param ppEntry [in, out] previous entry will be stored in *ppEntry
 */
EStatus SortedListGetPrevious(ListEntry** ppEntry);

/** Gets entries count
 * @param pThis [in] pointer to instance
 * @param pCount [out] count will be stored at *pCount
 */
EStatus SortedListGetCount(SortedList* pThis, unsigned* pCount);

/** Retrieves top value and removes it from the list
 * @param pThis [in] pointer to instance
 * @param ppValue [out] value
 */
EStatus SortedListPopFront(SortedList* pThis, void* ppValue);

/** Retrieves bottom value and removes it from the list
 * @param pThis [in] pointer to instance
 * @param ppValue [out] value
 */
EStatus SortedListPopBack(SortedList* pThis, void* ppValue);

/** Gets a value by specified entry
 * @param pEntry [in] list entry
 * @param ppValue [out] value will be stored at *ppValue
 */
EStatus SortedListGetValue(ListEntry* pEntry, void* ppValue);

/** Cleans a list according to filter function
 * @param pThis [in] pointer to instance
 * @param filter [in] pointer to item filter function
 * @param pUserArg [in] user defined argument of filter function
 * @see ItemFilter
 */
EStatus SortedListCleanUp(SortedList* pThis, ItemFilter filter, void* pUserArg);

/** Looks for an item in a list
 * @param pThis [in] pointer to instance
 * @param ppEntry [out] found item position will be stored at *ppEntry
 * @param comparator [in] pointer to item comparator function
 * @param pValue [in] item to look for (pValueRight argument of ItemComparator)
 * @param pUserArg [in] user defined argument of comparator function
 * @see ItemComparator
 */
EStatus SortedListFind(SortedList* pThis, ListEntry** ppEntry, ItemComparator comparator, const void* pValue, void* pUserArg);

/** Cleans a list according to filter function
 * @param pThis [in] pointer to instance
 * @param enumerator [in] pointer to item enumerator function
 * @param pUserArg [in] user defined argument of filter function
 * @see ItemEnumerator
 */
EStatus SortedListEnumerate(SortedList* pThis, ItemEnumerator enumerator, void* pUserArg);

#endif /* WIFI_MESH_SORTEDLIST_H_ */
