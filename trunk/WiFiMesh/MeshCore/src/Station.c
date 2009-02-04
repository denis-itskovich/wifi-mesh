#include "Station.h"
#include "Macros.h"
#include "List.h"
#include "Routing.h"
#include "Scheduler.h"

struct _Station
{
	StationId	id;
	Velocity	velocity;
	Location	curLocation;
	Location	orgLocation;

	Routing*	pRouting;
	List*		pOutbox;
	Scheduler*	pScheduler;
	Settings*	pSettings;
	TimeLine*	pTimeLine;
	double		silentTime;

	struct
	{
		StationSchedulerHandler callback;
		void*					pArg;
	}			schedulerHandler;

	struct
	{
		StationRoutingHandler 	callback;
		void*					pArg;
	}			routingHandler;

};

typedef EStatus (*MessageHandler)(Station* pThis, Message* pMessage);

void StationHandleRoutingChange(StationId destId,
								StationId transId,
								double expirationTime,
								int length,
								ERouteEntryUpdate updateAction,
								Station* pThis);
void StationHandleScheduleChange(double time, const Message* pMessage, Boolean bAdding, Station* pThis);

EStatus StationHandleData(Station* pThis, Message* pMessage);
EStatus StationHandleAck(Station* pThis, Message* pMessage);
EStatus StationHandleSearchRequest(Station* pThis, Message* pMessage);
EStatus StationHandleSearchResponse(Station* pThis, Message* pMessage);
EStatus StationHandleTransits(Station* pThis, Message* pMessage);
EStatus StationHandleLocals(Station* pThis, Message* pMessage);

Boolean StationIsDestination(Station* pThis, Message* pMessage);
Boolean StationIsAccepted(Station* pThis, Message* pMessage);
Boolean StationIsMessageReady(Station* pThis, Message* pMessage);
Boolean StationIsMessageValid(Station* pThis, Message* pMessage);

static MessageHandler s_handlers[eMSG_TYPE_LAST] =
{
		&StationHandleSearchRequest,
		&StationHandleSearchResponse,
		&StationHandleData,
		&StationHandleAck
};

EStatus StationNew(Station** ppThis, Velocity velocity, Location location, TimeLine* pTimeLine, Settings* pSettings)
{
	CONSTRUCT(ppThis, Station, velocity, location, pTimeLine, pSettings);
}

EStatus StationDelete(Station** ppThis)
{
	DESTRUCT(ppThis, Station);
}

EStatus StationInit(Station* pThis, Velocity velocity, Location location, TimeLine* pTimeLine, Settings* pSettings)
{
	VALIDATE_ARGUMENTS(pThis && pTimeLine && pSettings);
	CLEAR(pThis);

	pThis->orgLocation = location;
	pThis->curLocation = location;
	pThis->velocity = velocity;
	pThis->pSettings = pSettings;
	pThis->pTimeLine = pTimeLine;

	CHECK(RoutingNew(&pThis->pRouting, pSettings, pTimeLine));
	CHECK(ListNew(&pThis->pOutbox));
	CHECK(SchedulerNew(&pThis->pScheduler, pTimeLine));

	return eSTATUS_COMMON_OK;
}

EStatus StationDestroy(Station* pThis)
{
	VALIDATE_ARGUMENTS(pThis);

	CHECK(ListDelete(&pThis->pOutbox));
	CHECK(RoutingDelete(&pThis->pRouting));

	return eSTATUS_COMMON_OK;
}

EStatus StationSynchronize(Station* pThis, double timeDelta)
{
	Message* pMessage;
	VALIDATE_ARGUMENTS(pThis && (timeDelta > 0));

	CHECK(RoutingSynchronize(pThis->pRouting));

	if (pThis->silentTime <= timeDelta) pThis->silentTime = 0;
	else pThis->silentTime -= timeDelta;

	pThis->curLocation.x += pThis->velocity.x * timeDelta;
	pThis->curLocation.y += pThis->velocity.y * timeDelta;

	EStatus ret = SchedulerGetMessage(pThis->pScheduler, &pMessage);
	if (ret == eSTATUS_SCHEDULER_NO_MESSAGES) return eSTATUS_COMMON_OK;
	CHECK(ret);

	return StationHandleTransits(pThis, pMessage);
}

EStatus StationSetLocation(Station* pThis, Location newLocation)
{
	VALIDATE_ARGUMENTS(pThis);
	pThis->curLocation = pThis->orgLocation = newLocation;
	return eSTATUS_COMMON_OK;
}

EStatus StationSetVelocity(Station* pThis, Velocity newVelocity)
{
	SET_MEMBER(newVelocity, pThis, velocity);
}

EStatus StationGetId(const Station* pThis, StationId* pId)
{
	GET_MEMBER(pId, pThis, id);
}

EStatus StationSetId(Station* pThis, StationId id)
{
	SET_MEMBER(id, pThis, id);
}

EStatus StationGetMessage(Station* pThis, Message** ppMessage)
{
	ListEntry *pEntry, *pNext;
	Message* pMessage;
	*ppMessage = NULL;

	VALIDATE_ARGUMENTS(pThis && ppMessage);
	if (pThis->silentTime > 0) return eSTATUS_COMMON_OK;

	CHECK(ListGetHead(pThis->pOutbox, &pEntry));
	while (pEntry)
	{
		pNext = pEntry;
		CHECK(ListGetNext(&pNext));
		CHECK(ListGetValue(pEntry, &pMessage));

		if (StationIsMessageValid(pThis, pMessage))
		{
			if (StationIsMessageReady(pThis, pMessage))
			{
				CHECK(ListRemove(pThis->pOutbox, pEntry));
				*ppMessage = pMessage;
				break;
			}
		}
		else
		{
			CHECK(ListRemove(pThis->pOutbox, pEntry));
		}
		pEntry = pNext;
	}

	return eSTATUS_COMMON_OK;
}

EStatus StationPutMessage(Station* pThis, Message* pMessage)
{
	VALIDATE_ARGUMENTS(pThis && pMessage && (pMessage->type < eMSG_TYPE_LAST));
	CHECK(SettingsGetTransmitTime(pThis->pSettings, pMessage, &pThis->silentTime));
	CHECK(TimeLineRelativeEvent(pThis->pTimeLine, pThis->silentTime, pMessage));

	if (pMessage->transitSrcId != pThis->id &&
		pMessage->originalSrcId != pThis->id)
	{
		CHECK(RoutingHandleMessage(pThis->pRouting, pMessage));
	}

	if (!StationIsAccepted(pThis, pMessage))
	{
		MessageDelete(&pMessage);
		return eSTATUS_STATION_MESSAGE_NOT_ACCEPTED;
	}

	if (StationHandleLocals(pThis, pMessage) != eSTATUS_COMMON_OK)
	{
		return StationHandleTransits(pThis, pMessage);
	}
	return eSTATUS_COMMON_OK;
}

Boolean StationIsDestination(Station* pThis, Message* pMessage)
{
	return (IS_BROADCAST(pMessage->originalDstId)) || (pMessage->originalDstId == pThis->id) ? TRUE : FALSE;
}

Boolean StationIsAccepted(Station* pThis, Message* pMessage)
{
	if (pMessage->transitSrcId == pThis->id) return FALSE;
	if (pMessage->originalSrcId == pThis->id) return FALSE;
	return (IS_BROADCAST(pMessage->transitDstId)) || (pMessage->transitDstId == pThis->id) ? TRUE : FALSE;
}

EStatus StationHandleTransits(Station* pThis, Message* pMessage)
{
	Message* pNewMsg = NULL;
	unsigned length = 0;

	pMessage->transitSrcId = pThis->id;
	pMessage->transitDstId = INVALID_STATION_ID;

	if (eSTATUS_COMMON_OK == RoutingLookFor(pThis->pRouting, pMessage->originalDstId, &pMessage->transitDstId, &length))
	{
		return ListPushFront(pThis->pOutbox, pMessage);
	}

	CHECK(MessageNewSearchRequest(&pNewMsg, pThis->id, pMessage->originalDstId));
	CHECK(ListPushFront(pThis->pOutbox, pNewMsg));
	CHECK(ListPushBack(pThis->pOutbox, pMessage));

	return eSTATUS_COMMON_OK;
}

EStatus StationHandleLocals(Station* pThis, Message* pMessage)
{
	CHECK(s_handlers[pMessage->type](pThis, pMessage));
	return MessageDelete(&pMessage);
}

EStatus StationHandleData(Station* pThis, Message* pMessage)
{
	VALIDATE_ARGUMENTS(pThis && pMessage);
	return (StationIsDestination(pThis, pMessage)) ? eSTATUS_COMMON_OK : eSTATUS_STATION_MESSAGE_NOT_ACCEPTED;
}

EStatus StationHandleAck(Station* pThis, Message* pMessage)
{
	VALIDATE_ARGUMENTS(pThis && pMessage);
	return (StationIsDestination(pThis, pMessage)) ? eSTATUS_COMMON_OK : eSTATUS_STATION_MESSAGE_NOT_ACCEPTED;
}

EStatus StationHandleSearchRequest(Station* pThis, Message* pMessage)
{
	StationId dstId;
	Message* pResponse;
	unsigned length = 0;
	VALIDATE_ARGUMENTS(pThis && pMessage);

	if (pMessage->originalDstId != pThis->id)
	{
		CHECK(RoutingLookFor(pThis->pRouting, pMessage->originalDstId, &dstId, &length));
	}

	dstId = pMessage->originalSrcId;
	CHECK(MessageNewSearchResponse(&pResponse, pThis->id, dstId));
	pResponse->nodesCount = length;
	pResponse->transitDstId = pMessage->transitSrcId;
	pResponse->originalSrcId = pMessage->originalDstId;
	CHECK(ListPushFront(pThis->pOutbox, pResponse));

	return eSTATUS_COMMON_OK;
}

EStatus StationHandleSearchResponse(Station* pThis, Message* pMessage)
{
	VALIDATE_ARGUMENTS(pThis && pMessage);
	return (StationIsDestination(pThis, pMessage)) ? eSTATUS_COMMON_OK : eSTATUS_STATION_MESSAGE_NOT_ACCEPTED;
}

EStatus StationScheduleMessage(Station* pThis, Message* pMessage, double time)
{
	VALIDATE_ARGUMENTS(pThis && pMessage);
	return SchedulerPutMessage(pThis->pScheduler, pMessage, time);
}

EStatus StationGetLocation(const Station* pThis, Location* pLocation)
{
	GET_MEMBER(pLocation, pThis, curLocation);
}

EStatus StationGetVelocity(const Station* pThis, Velocity* pVelocity)
{
	GET_MEMBER(pVelocity, pThis, velocity);
}

EStatus StationIsAdjacent(const Station* pThis, const Station* pStation, Boolean* pIsAdjacent)
{
	double coverage;
	double dx, dy;
	VALIDATE_ARGUMENTS(pThis && pStation && pIsAdjacent);
	CHECK(SettingsGetCoverage(pThis->pSettings, &coverage));
	dx = pThis->curLocation.x - pStation->curLocation.x;
	dy = pThis->curLocation.y - pStation->curLocation.y;
	*pIsAdjacent = (coverage*coverage >= dx*dx + dy*dy) ? TRUE : FALSE;

	return eSTATUS_COMMON_OK;
}

EStatus StationIsActive(const Station* pThis, Boolean* pIsActive)
{
	Size size;
	Location loc;
	VALIDATE_ARGUMENTS(pThis);
	CHECK(SettingsGetWorldSize(pThis->pSettings, &size));
	loc = pThis->curLocation;
	*pIsActive = (loc.x >= -size.x/2.0 && loc.x <= size.x/2.0) &&
				 (loc.y >= -size.y/2.0 && loc.y <= size.y/2.0) ? TRUE : FALSE;
	return eSTATUS_COMMON_OK;
}

EStatus StationReset(Station* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	pThis->curLocation = pThis->orgLocation;

	CHECK(ListCleanUp(pThis->pOutbox, NULL, NULL));
	CHECK(RoutingClear(pThis->pRouting));
	CHECK(SchedulerReset(pThis->pScheduler));

	return eSTATUS_COMMON_OK;
}

Boolean StationIsMessageReady(Station* pThis, Message* pMessage)
{
	unsigned length = 0;
	if (pMessage->transitDstId != INVALID_STATION_ID) return TRUE;
	return eSTATUS_COMMON_OK == RoutingLookFor(pThis->pRouting, pMessage->originalDstId, &pMessage->transitDstId, &length);
}

Boolean StationIsMessageValid(Station* pThis, Message* pMessage)
{
	unsigned length = 0;
	if (pMessage->type != eMSG_TYPE_SEARCH_REQUEST) return TRUE;
	if (pMessage->originalSrcId != pThis->id) return TRUE;
	return RoutingLookFor(pThis->pRouting, pMessage->originalDstId, &pMessage->transitDstId, &length) != eSTATUS_COMMON_OK ? TRUE : FALSE;
}

EStatus StationRegisterSchedulerHandler(Station* pThis, StationSchedulerHandler handler, void* pUserArg)
{
	VALIDATE_ARGUMENTS(pThis);
	pThis->schedulerHandler.callback = handler;
	pThis->schedulerHandler.pArg = pUserArg;
	if (handler) CHECK(SchedulerRegisterHandler(pThis->pScheduler, (SchedulerHandler)&StationHandleScheduleChange, pThis));
	else CHECK(SchedulerRegisterHandler(pThis->pScheduler, NULL, NULL));
	return eSTATUS_COMMON_OK;
}

EStatus StationRegisterRoutingHandler(Station* pThis, StationRoutingHandler handler, void* pUserArg)
{
	VALIDATE_ARGUMENTS(pThis);
	pThis->routingHandler.callback = handler;
	pThis->routingHandler.pArg = pUserArg;
	if (handler) CHECK(RoutingRegisterHandler(pThis->pRouting, (RoutingHandler)&StationHandleRoutingChange, pThis));
	else CHECK(RoutingRegisterHandler(pThis->pRouting, NULL, NULL));
	return eSTATUS_COMMON_OK;
}

void StationHandleRoutingChange(StationId destId,
								StationId transId,
								double expirationTime,
								int length,
								ERouteEntryUpdate updateAction,
								Station* pThis)
{
	if (pThis->routingHandler.callback)
	{
		pThis->routingHandler.callback(pThis, destId, transId, expirationTime, length, updateAction, pThis->routingHandler.pArg);
	}
}

void StationHandleScheduleChange(double time, const Message* pMessage, Boolean bAdding, Station* pThis)
{
	if (pThis->schedulerHandler.callback)
	{
		pThis->schedulerHandler.callback(pThis, time, pMessage, bAdding, pThis->schedulerHandler.pArg);
	}
}
