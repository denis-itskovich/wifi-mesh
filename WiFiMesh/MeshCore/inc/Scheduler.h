/**
 * \file Scheduler.h
 *
 * Project: MeshCore
 * (C) Copyright 2008 Denis Itskovich
 *
 * \date 22/12/2008
 * \author Denis Itskovich
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "Status.h"
#include "Packet.h"
#include "TimeLine.h"

typedef struct _Scheduler Scheduler;	///< forward declaration

typedef enum _ESchedulerEvent
{
	eSCHEDULE_ADDED,
	eSCHEDULE_REMOVED,
	eSCHEDULE_DELIVERED
} ESchedulerEvent;

/** Scheduler adding/removing packet handler
 * \param time [in] time, when a packet should be issued
 * \param pPacket [in] pointer to packet instance
 * \param bAdded [in] if TRUE packet is being added, otherwise removed
 * \param pUserArg [in] user defined argument
 */
typedef void (*SchedulerHandler)(double time, const Packet* pPacket, ESchedulerEvent bAdded, void* pUserArg);

/** Allocates and initializes new instance
 * \param ppThis [out] pointer to new instance will be stored at *ppThis
 * \param pTimeLine [in] pointer to valid TimeLine instance
 */
EStatus SchedulerNew(Scheduler** ppThis, TimeLine* pTimeLine);

/** Destroys and deallocates an instance
 * \param ppThis [in, out] pointer to valid instance
 */
EStatus SchedulerDelete(Scheduler** ppThis);

/** Initializes an instance
 * \param pThis [in] pointer to instance
 * \param pTimeLine [in] pointer to valid TimeLine instance
 */
EStatus SchedulerInit(Scheduler* pThis, TimeLine* pTimeLine);

/** Destroys an instance
 * \param pThis [in] pointer to valid instance
 */
EStatus SchedulerDestroy(Scheduler* pThis);

/** Handles packet (currently - ACK only)
 * \param pThis [in] pointer to instance
 * \param pPacket [in] pointer to packet to handle
 */
EStatus SchedulerHandlePacket(Scheduler* pThis, const Packet* pPacket);

/** Schedules a packet
 * \param pThis [in] pointer to instance
 * \param pPacket [in] pointer to packet
 * \param time [in] time, when the packet should be issued, in time units
 */
EStatus SchedulerPutPacket(Scheduler* pThis, Packet* pPacket, double time);

/** Retrieves available packet and removes it from queue
 * \param pThis [in] pointer to instance
 * \param ppPacket [out] pointer to packet will be stored at *ppPacket
 * \return eSTATUS_SCHEDULER_NO_PACKETS_AVAILABLE if there are no ready packets
 * \return eSTATUS_COMMON_OK if a packet was retrieved
 */
EStatus SchedulerGetPacket(Scheduler* pThis, Packet** ppPacket);

/** Removes all scheduled packets
 * \param pThis [in] pointer to instance
 */
EStatus SchedulerClear(Scheduler* pThis);

/** Resets a scheduler
 * \param pThis [in]
 */
EStatus SchedulerReset(Scheduler* pThis);

/** Registers scheduler handler
 * \param pThis [in] pointer to instance
 * \param handler [in] scheduler handler
 * \param pUserArg [in] user defined argument
 */
EStatus SchedulerRegisterHandler(Scheduler* pThis, SchedulerHandler handler, void* pUserArg);

#endif /* SCHEDULER_H_ */
