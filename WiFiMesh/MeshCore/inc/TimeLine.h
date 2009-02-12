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

#include "Status.h"
#include "Packet.h"

typedef struct _TimeLine TimeLine; ///< TimeLine forward declaration

/** Time line callback
 * Callback is called each time an event is added to or removed from the time line
 * \param time [in] event time
 * \param pPacket [in] associated packet
 * \param isAdded [in] determines whether an event is being added or removed
 * \param pUserArg [in] user defined argument
 */
typedef void (*EventTracker)(double time, const Packet* pPacket, Boolean isAdded, void* pUserArg);

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
 * \param pPacket [in] packet, associated with an event
 */
EStatus TimeLineEvent(TimeLine* pThis, double time, const Packet* pPacket);

/** Add a new relative milestone to time line
 * \param pThis [in] pointer to instance
 * \param timeDelta [in] delta time
 * \param pPacket [in] packet, associated with an event
 */
EStatus TimeLineRelativeEvent(TimeLine* pThis, double timeDelta, const Packet* pPacket);

/** Switches to next milestone
 * \param pThis [in] pointer to instance
 */
EStatus TimeLineNext(TimeLine* pThis);

/** Retrieves current time
 * \param pThis [in] pointer to instance
 * \param pTime [out] current time will be stored at *pTime
 */
EStatus TimeLineGetTime(const TimeLine* pThis, double* pTime);

/** Clear time line
 * \param pThis [in] pointer to instance
 */
EStatus TimeLineClear(TimeLine* pThis);

/** Retrieves time line length
 * \param pThis [in] pointer to instance
 * \param pLength [out] length will be stored at *pLength
 */
EStatus TimeLineGetLength(TimeLine* pThis, double* pLength);

/** Sets time line event tracker
 * \param pThis [in] pointer to instance
 * \param tracker [in] event tracker
 * \param pUserArg [in] user defined argument
 * \see EventTracker
 */
EStatus TimeLineSetEventTracker(TimeLine* pThis, EventTracker tracker, void* pUserArg);

#endif // _WIFI_MESH_TIMELINE_H
