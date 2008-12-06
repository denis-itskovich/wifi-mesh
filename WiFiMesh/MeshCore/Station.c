#include "Station.h"
#include "Macros.h"

struct _Station
{
	StationId	id;
	Velocity	velocity;
	Location	location;
};

static unsigned long s_nextId = 0;

EStatus StationCreate(Station** ppThis, Velocity velocity, Location location)
{
	CONSTRUCT(ppThis, Station, TRUE);

	*ppThis->location = location;
	*ppThis->velocity = velocity;
	*ppThis->id = s_nextId++;

	return eSTATUS_COMMON_OK;
}

EStatus StationDestroy(Station** ppThis)
{
	VALIDATE_ARGUMENTS(ppThis && *ppThis);
	DELETE(*ppThis);

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
