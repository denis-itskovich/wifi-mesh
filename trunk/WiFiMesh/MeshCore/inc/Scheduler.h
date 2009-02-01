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
#include "Message.h"
#include "TimeLine.h"

typedef struct _Scheduler Scheduler;	///< forward declaration

/** Scheduler adding/removing message handler
 * \param time [in] time, when a message should be issued
 * \param pMessage [in] pointer to message instance
 * \param bAdded [in] if TRUE message is being added, otherwise removed
 * \param pUserArg [in] user defined argument
 */
typedef void (*SchedulerHandler)(double time, const Message* pMessage, Boolean bAdded, void* pUserArg);

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

/** Schedules a message
 * \param pThis [in] pointer to instance
 * \param pMessage [in] pointer to message
 * \param time [in] time, when the message should be issued, in time units
 */
EStatus SchedulerPutMessage(Scheduler* pThis, Message* pMessage, double time);

/** Retrieves available message and removes it from queue
 * \param pThis [in] pointer to instance
 * \param ppMessage [out] pointer to message will be stored at *ppMessage
 * \return eSTATUS_SCHEDULER_NO_MESSAGES_AVAILABLE if there are no ready messages
 * \return eSTATUS_COMMON_OK if a message was retrieved
 */
EStatus SchedulerGetMessage(Scheduler* pThis, Message** ppMessage);

/** Removes all scheduled messages
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
