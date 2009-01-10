/**
 * \file SortedList.h
 *
 * Sorted Linked List module
 *
 * Represents a sorted list
 * \see List.h, Queue.h
 *
 * \author Denis Itskovich
 * \date   13/12/2008
 */

#ifndef WIFI_MESH_SORTEDLIST_H_
#define WIFI_MESH_SORTEDLIST_H_

#include "Status.h"
#include "List.h"

typedef struct _SortedList SortedList; ///< forward declaration of SortedList

/** Allocates & initializes new SortedList instance
 * \param ppThis [out] pointer to new instance will be placed at *ppThis
 * \param comparator [in] pointer to comparison function
 * \param pUserArg [in] pointer to user-defined argument of comparison function
 */
EStatus SortedListNew(SortedList** ppThis, ItemComparator comparator, void* pUserArg);

/** Deallocates & destroys SortedList instance
 * \param ppThis [in, out] *ppThis must point to valid instance
 */
EStatus SortedListDelete(SortedList** ppThis);

/** Initializes previously allocated instance
 * \param pThis [in] pointer to instance
 * \param comparator [in] pointer to comparison function
 * \param pUserArg [in] pointer to user-defined argument of comparison function
 */
EStatus SortedListInit(SortedList* pThis, ItemComparator comparator, void* pUserArg);

/** Deinitializes an instance without deallocating
 * \param pThis [in] pointer to instance
 */
EStatus SortedListDestroy(SortedList* pThis);

/** Adds a new item into the sorted list
 * \param pThis [in] pointer to instance
 * \param pValue [in] pointer to new item
 */
EStatus SortedListAdd(SortedList* pThis, void* pValue);

/** Removes an item from the list
 * \param pThis [in] pointer to instance
 * \param pEntry [in] pointer to entry, which should be removed
 */
EStatus SortedListRemove(SortedList* pThis, ListEntry* pEntry);

/** Gets an entry of the head
 * \param pThis [in] pointer to instance
 * \param ppEntry [out] head entry will be stored in *ppEntry
 */
EStatus SortedListGetHead(SortedList* pThis, ListEntry** ppEntry);

/** Gets an entry of the tail
 * \param pThis [in] pointer to instance
 * \param ppEntry [out] tail entry will be stored in *ppEntry
 */
EStatus SortedListGetTail(SortedList* pThis, ListEntry** ppEntry);

/** Gets an entry of the next to specified item
 * \param ppEntry [in, out] next entry will be stored in *ppEntry
 */
EStatus SortedListGetNext(ListEntry** ppEntry);

/** Gets an entry of the previous to specified item
 * \param ppEntry [in, out] previous entry will be stored in *ppEntry
 */
EStatus SortedListGetPrevious(ListEntry** ppEntry);

/** Gets a value by specified entry
 * \param pEntry [in] list entry
 * \param ppValue [out] value will be stored at *ppValue
 */
EStatus SortedListGetValue(ListEntry* pEntry, void* ppValue);

/** Cleans a list according to filter function
 * \param pThis [in] pointer to instance
 * \param filter [in] pointer to item filter function
 * \param pUserArg [in] user defined argument of filter function
 * \sa ItemFilter
 */
EStatus SortedListCleanUp(SortedList* pThis, ItemFilter filter, void* pUserArg);

#endif /* WIFI_MESH_SORTEDLIST_H_ */
