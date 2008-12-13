/**
 * \file Queue.h
 *
 * Queue module
 *
 * Represents generic queue data structure
 * implementation based on List
 * \see List.h, SortedList.h

 * \author Denis Itskovich
 * \date   01/12/2008
 */

#ifndef WIFI_MESH_QUEUE_H_
#define WIFI_MESH_QUEUE_H_

#include "Status.h"
#include "List.h"

typedef struct _Queue Queue; ///< forward declaration of Queue

/** Allocates and initializes a new instance of Queue
 * \param ppThis [out] pointer to new instance will be stored at *ppThis
 */
EStatus QueueNew(Queue** ppThis);

/** Destroys and deallocates an instance
 * \param ppThis [in, out] *ppThis should point to valid instance
 */
EStatus QueueDelete(Queue** ppThis);

/** Initializes previously allocated instance
 * \param pThis [in] pointer to instance
 */
EStatus QueueInit(Queue* pThis);

/** Deinitializes an instance without deallocating
 * \param pThis [in] pointer to instance
 */
EStatus QueueDestroy(Queue* pThis);

/** Adds new item into the queue
 * \param pThis [in] pointer to instance
 * \param pItem [in] pointer to abstract item
 */
EStatus QueuePush(Queue* pThis, void* pItem);

/** Retrieves and removes top item from the queue
 * \param pThis [in] pointer to instance
 * \param ppItem [out] pointer to retrieved item will be stored at *ppItem
 */
EStatus QueuePop(Queue* pThis, void** ppItem);

#endif /* WIFI_MESH_QUEUE_H_ */
