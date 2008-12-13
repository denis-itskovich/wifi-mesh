/**
 * \file TimeLine.h
 *
 * TimeLine module
 *
 * Represents a time line - events collection
 * \see Event.h
 *
 * \author Denis Itskovich
 * \date 12/12/2008
 */
#ifndef _WIFI_MESH_TIMELINE_H
#define _WIFI_MESH_TIMELINE_H

#include "Event.h"

typedef struct _TimeLine TimeLine; ///< TimeLine forward declaration

/** Allocates and initializes a new instance of TimeLine
 * \param ppThis [out] pointer to new instance will be stored at *ppThis
 */
EStatus TimeLineNew(TimeLine** ppThis);

/** Destroys and deallocates an instance of TimeLine
 * \param ppThis [in, out] *ppThis must point to valid instance
 */
EStatus TimeLineDelete(TimeLine** ppThis);

/** Initializes an instance of TimeLine
 * \param pThis [in] pointer to instance
 */
EStatus TimeLineInit(TimeLine* pThis);

/** Destroys an instance of TimeLine
 * \param pThis [in] pointer to instance
 */
EStatus TimeLineDestroy(TimeLine* pThis);

/** Puts an event into the time line
 * \param pThis [in] pointer to instance
 * \param pEvent [in] pointer to event
 */
EStatus TimeLinePutEvent(TimeLine* pThis, Event* pEvent);

/** Retrieves next event from the time line
 * \param pThis [in] pointer to instance
 * \param ppEvent [out] a pointer to event will be stored at *ppEvent
 */
EStatus TimeLineGetEvent(TimeLine* pThis, Event** ppEvent);

#endif // _WIFI_MESH_TIMELINE_H
