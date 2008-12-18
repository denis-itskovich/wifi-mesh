#include "Station.h"
#include "Macros.h"
#include "Queue.h"

struct _Station
{
	StationId	id;
	Velocity	velocity;
	Location	location;

	Queue		inbox;
	Queue		outbox;
};

static unsigned long s_nextId = 0;

EStatus StationNew(Station** ppThis, Velocity velocity, Location location)
{
	CONSTRUCT(ppThis, Station, TRUE);
	return StationInit(*ppThis, velocity, location);
}

EStatus StationInit(Station* pThis, Velocity velocity, Location location)
{
	CLEAR(pThis);

	pThis->location = location;
	pThis->velocity = velocity;
	pThis->id = s_nextId++;

	CHECK(QueueInit(&pThis->inbox));
	CHECK(QueueInit(&pThis->outbox));

	return eSTATUS_COMMON_OK;
}

EStatus StationDelete(Station** ppThis)
{
	VALIDATE_ARGUMENTS(ppThis && *ppThis);
	StationDestroy(*ppThis);
	DELETE(*ppThis);

	return eSTATUS_COMMON_OK;
}

EStatus StationDestroy(Station* pThis)
{
	VALIDATE_ARGUMENTS(pThis);

	CHECK(QueueDestroy(&pThis->inbox));
	CHECK(QueueDestroy(&pThis->outbox));

	return eSTATUS_COMMON_OK;
}

EStatus StationSynchronize(Station* pThis, double time)
{
	VALIDATE_ARGUMENTS(pThis && (timeDelta > 0));
	pThis->location.x += pThis->velocity.x * timeDelta;
	pThis->location.y += pThis->velocity.y * timeDelta;

	return eSTATUS_COMMON_OK;
}

EStatus StationMoveTo(Station* pThis, Location newLocation)
{
	SET_MEMBER(newLocation, pThis, location);
}

EStatus StationGetId(Station* pThis, StationId* pId)
{
	GET_MEMBER(pId, pThis, id);
}

EStatus StationGetMessage(Station* pThis, Message** ppMessage)
{
	VALIDATE_ARGUMENTS(pThis && ppMessage);
	return QueuePop(&pThis->outbox, (void**)ppMessage);
}

EStatus StationSendMessage(Station* pThis, Message* pMessage)
{

}

EStatus StationPutMessage(Station* pThis, Message* pMessage)
{
	pMessage->
}
