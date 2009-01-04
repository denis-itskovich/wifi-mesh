/**
 * \file Event.c
 *
 * \date 17/12/2008
 * \author Denis Itskovich
 */

#include "Event.h"
#include "List.h"
#include "Macros.h"

struct _Event
{
	double		time;		///< time of event
	Message*	pMessage;	///< a Message associated with event
};

EStatus EventNew(Event** ppThis, double time, Message* pMessage)
{
	CONSTRUCT(ppThis, Event, time, pMessage);
}

EStatus EventDelete(Event** ppThis)
{
	DESTRUCT(ppThis, Event);
}

EStatus EventInit(Event* pThis, double time, Message* pMessage)
{
	VALIDATE_ARGUMENTS(pThis && pMessage);
	CLEAR(pThis);

	pThis->time = time;

	return MessageClone(pThis->pMessage, pMessage);
}

EStatus EventDestroy(Event* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	MessageDestroy(pThis->pMessage);

	return eSTATUS_COMMON_OK;
}
