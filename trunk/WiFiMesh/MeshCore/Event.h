/**
 * \file Event.h
 *
 * Event module
 *
 * Represents a single Event
 *
 * \author Denis Itskovich
 * \date   1/12/2008
 */

#ifndef _WIFI_MESH_EVENT_H
#define _WIFI_MESH_EVENT_H

#include "Message.h"
#include "Status.h"

typedef struct _Event Event;

/** Allocates and initializes an event
 * \param ppThis [out] pointer to new instance will be stored at *ppThis
 * \param time [in] event time
 * \param pMessage [in] pointer to a message associated with an event
 * \sa Message
 */
EStatus EventNew(Event** ppThis, unsigned time, Message* pMessage);

/** Destroys and deallocates and instance of Event
 * \param ppThis [in, out] *ppThis must point to valid instance
 */
EStatus EventDelete(Event** ppThis);

/** Initializes an event
 * \param pThis [in] pointer to valid instance
 * \param time [in] event time
 * \param pMessage [in] pointer to a message associated with an event
 * \sa Message
 */
EStatus EventInit(Event* pThis, double time, Message* pMessage);

/** Destroys an event
 * \param pThis [in] pointer to valid instance
 */
EStatus EventDestroy(Event* pThis);

#endif // _WIFI_MESH_EVENT_H
