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

typedef struct _SortedList SortedList; ///< forward declaration of SortedList

/** Allocates & initializes new SortedList instance
 * \param ppThis [out] pointer to new instance will be placed at *ppThis
 * \param comparator [in] pointer to comparison function
 */
EStatus SortedListNew(SortedList** ppThis, ItemComparator comparator);

/** Deallocates & destroys SortedList instance
 * \param ppThis [in, out] *ppThis must point to valid instance
 */
EStatus SortedListDelete(SortedList** ppThis);

/** Initializes previously allocated instance
 * \param pThis [in] pointer to instance
 * \param comparator [in] pointer to comparison function
 */
EStatus SortedListInit(SortedList* pThis, ItemComparator comparator);

/** Deinitializes an instance without deallocating
 * \param pThis [in] pointer to instance
 */
EStatus SortedListDestroy(SortedList* pThis);

/** Adds a new item into the sorted list
 * \param pThis [in] pointer to instance
 * \param pValue [in] pointer to new item
 */
EStatus SortedListAdd(SortedList* pThis, void* pValue);

/** Gets a position of the head
 * \param pThis [in] pointer to instance
 * \param ppPosition [out] head position will be stored in *ppPosition
 */
EStatus SortedListGetHead(SortedList* pThis, ListPosition** ppPosition);

/** Gets a position of the tail
 * \param pThis [in] pointer to instance
 * \param ppPosition [out] tail position will be stored in *ppPosition
 */
EStatus SortedListGetTail(SortedList* pThis, ListPosition** ppPosition);

/** Gets a position of the next to specified item
 * \param ppPosition [in, out] next position will be stored in *ppPosition
 */
EStatus SortedListGetNext(ListPosition** ppPosition);

/** Gets a position of the previous to specified item
 * \param ppPosition [in, out] previous position will be stored in *ppPosition
 */
EStatus SortedListGetPrevious(ListPosition** ppPosition);

/** Gets a value by specified position
 * \param pPosition [in] list position
 * \param ppValue [out] value will be stored at *ppValue
 */
EStatus SortedListGetValue(ListPosition* pPosition, void** ppValue);

#endif /* WIFI_MESH_SORTEDLIST_H_ */
