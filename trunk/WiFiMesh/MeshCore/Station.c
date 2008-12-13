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

EStatus StationCreate(Station** ppThis, Velocity velocity, Location location)
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

	CHECK_STATUS(QueueInit(&pThis->inbox));
	CHECK_STATUS(QueueInit(&pThis->outbox));

	return eSTATUS_COMMON_OK;
}

EStatus StationDispose(Station** ppThis)
{
	VALIDATE_ARGUMENTS(ppThis && *ppThis);
	StationDestroy(*ppThis);
	DELETE(*ppThis);

	return eSTATUS_COMMON_OK;
}

EStatus StationDestroy(Station* pThis)
{
	VALIDATE_ARGUMENTS(pThis);

	CHECK_STATUS(QueueDestroy(&pThis->inbox));
	CHECK_STATUS(QueueDestroy(&pThis->outbox));

	return eSTATUS_COMMON_OK;
}

EStatus StationMove(Station* pThis, unsigned timeDelta)
{
	VALIDATE_ARGUMENTS(pThis && (timeDelta > 0));
	pThis->location.x += pThis->velocity.x * (double)timeDelta;
	pThis->location.y += pThis->velocity.y * (double)timeDelta;

	return eSTATUS_COMMON_OK;
}

EStatus StationGetId(Station* pThis, StationId* pId)
{
	GET_MEMBER(pId, pThis, id);
}
