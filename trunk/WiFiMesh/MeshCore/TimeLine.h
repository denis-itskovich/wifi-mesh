/**
 * \file TimeLine.h
 *
 * TimeLine module
 *
 * Represents a time line - events collection
 *
 * \author Denis Itskovich
 * \date 12/12/2008
 */
#ifndef _WIFI_MESH_TIMELINE_H
#define _WIFI_MESH_TIMELINE_H

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

/** Add a new milestone to time line
 * \param pThis [in] pointer to instance
 * \param time [in] time of event, in time units
 */
EStatus TimeLineMilestone(TimeLine* pThis, double time);

/** Switches to next milestone
 * \param pThis [in] pointer to instance
 */
EStatus TimeLineNext(TimeLine* pThis);

/** Retrieves current time
 * \param pThis [in] pointer to instance
 * \param pTime [out] current time will be stored at *pTime
 */
EStatus TimeLineGetTime(TimeLine* pThis, double* pTime);

#endif // _WIFI_MESH_TIMELINE_H
