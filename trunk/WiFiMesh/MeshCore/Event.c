#include "Event.h"
#include "List.h"
#include "Macros.h"

struct _Event
{
	ListHeader 		list;
	unsigned long	time;
	Message*		pMessage;
};

EStatus EventCreate(Event** ppThis, unsigned time, Message* pMessage)
{
	CONSTRUCT(ppThis, Event, pMessage);

	*ppThis->time = time;
	return MessageClone(*ppThis->pMessage, pMessage);
}

EStatus EventDestroy(Event** ppThis)
{
	VALIDATE_ARGUMENTS(ppThis && *ppThis);

	MessageDestroy(*ppThis->pMessage);
	DELETE(*ppThis);

	return eSTATUS_COMMON_OK;
}
