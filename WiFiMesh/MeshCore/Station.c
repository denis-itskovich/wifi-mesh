#include "Station.h"
#include "Macros.h"
#include "Queue.h"
#include "Routing.h"

struct _Station
{
	StationId	id;
	Velocity	velocity;
	Location	location;

	Routing*	pRouting;
	Queue*		pInbox;
	Queue*		pOutbox;
};

static unsigned long s_nextId = 0;

typedef EStatus (*MessageHandler)(Station* pThis, Message* pMessage);

EStatus StationHandleData(Station* pThis, Message* pMessage);
EStatus StationHandleAck(Station* pThis, Message* pMessage);
EStatus StationHandleSearchRequest(Station* pThis, Message* pMessage);
EStatus StationHandleSearchResponse(Station* pThis, Message* pMessage);
EStatus StationHandleTransits(Station* pThis, Message* pMessage);
EStatus StationHandleLocals(Station* pThis, Message* pMessage);

Boolean StationIsDestination(Station* pThis, Message* pMessage);

static MessageHandler s_handlers[eMSG_TYPE_LAST] =
{
		&StationHandleSearchRequest,
		&StationHandleSearchResponse,
		&StationHandleData,
		&StationHandleAck
};

EStatus StationNew(Station** ppThis, Velocity velocity, Location location)
{
	CONSTRUCT(ppThis, Station, velocity, location);
}

EStatus StationDelete(Station** ppThis)
{
	DESTRUCT(ppThis, Station);
}

EStatus StationInit(Station* pThis, Velocity velocity, Location location)
{
	CLEAR(pThis);

	pThis->location = location;
	pThis->velocity = velocity;
	pThis->id = s_nextId++;

	CHECK(RoutingNew(&pThis->pRouting));
	CHECK(QueueNew(&pThis->pInbox));
	CHECK(QueueNew(&pThis->pOutbox));

	return eSTATUS_COMMON_OK;
}

EStatus StationDestroy(Station* pThis)
{
	VALIDATE_ARGUMENTS(pThis);

	CHECK(QueueDelete(&pThis->pInbox));
	CHECK(QueueDelete(&pThis->pOutbox));
	CHECK(RoutingDelete(&pThis->pRouting));

	return eSTATUS_COMMON_OK;
}

EStatus StationSynchronize(Station* pThis, double timeDelta)
{
	VALIDATE_ARGUMENTS(pThis && (timeDelta > 0));

	pThis->location.x += pThis->velocity.x * timeDelta;
	pThis->location.y += pThis->velocity.y * timeDelta;

	return RoutingSynchronize(pThis->pRouting);
}

EStatus StationMoveTo(Station* pThis, Location newLocation)
{
	SET_MEMBER(newLocation, pThis, location);
}

EStatus StationGetId(const Station* pThis, StationId* pId)
{
	GET_MEMBER(pId, pThis, id);
}

EStatus StationGetMessage(Station* pThis, Message** ppMessage)
{
	VALIDATE_ARGUMENTS(pThis && ppMessage);
	return QueuePop(pThis->pOutbox, (void**)ppMessage);
}

EStatus StationPutMessage(Station* pThis, Message* pMessage)
{
	VALIDATE_ARGUMENTS(pThis && pMessage && (pMessage->type < eMSG_TYPE_LAST));
	CHECK(RoutingHandleMessage(pThis->pRouting, pMessage));
	if (!StationIsDestination(pThis, pMessage)) return StationHandleTransits(pThis, pMessage);
	return StationHandleLocals(pThis, pMessage);
}

Boolean StationIsDestination(Station* pThis, Message* pMessage)
{
	return (IS_BROADCAST(pMessage->originalDstId)) || (pMessage->originalDstId == pThis->id) ? TRUE : FALSE;
}

EStatus StationHandleTransits(Station* pThis, Message* pMessage)
{
	Message* pNewMsg = NULL;

	++pMessage->nodesCount;
	pMessage->transitSrcId = pThis->id;

	if (eSTATUS_COMMON_OK == RoutingLookFor(pThis->pRouting, pMessage->originalDstId, &pMessage->transitDstId))
	{
		return QueuePush(pThis->pOutbox, pMessage);
	}

	pMessage->transitDstId = INVALID_STATION_ID;
	CHECK(MessageNewSearchRequest(&pNewMsg, pThis->id, pMessage->originalDstId));
	CHECK(QueuePush(pThis->pOutbox, pNewMsg));
	CHECK(QueuePush(pThis->pOutbox, pMessage));

	return eSTATUS_COMMON_OK;
}

EStatus StationHandleLocals(Station* pThis, Message* pMessage)
{
	CHECK(s_handlers[pMessage->type](pThis, pMessage));
	return MessageDelete(&pMessage);
}

EStatus StationHandleSearchRequest(Station* pThis, Message* pMessage)
{
	StationId dst = pMessage->originalSrcId;
}
