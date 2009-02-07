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
	Packet*	pReadyPacket;

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

typedef EStatus (*PacketHandler)(Station* pThis, const Packet* pPacket);

void StationHandleRoutingChange(StationId destId,
								StationId transId,
								double expirationTime,
								int length,
								ERouteEntryUpdate updateAction,
								Station* pThis);
void StationHandleScheduleChange(double time, const Packet* pPacket, ESchedulerEvent event, Station* pThis);

EStatus StationSendPacket(Station* pThis, Packet* pPacket);
EStatus StationHandleData(Station* pThis, const Packet* pPacket);
EStatus StationHandleAck(Station* pThis, const Packet* pPacket);
EStatus StationHandleSearchRequest(Station* pThis, const Packet* pPacket);
EStatus StationHandleSearchResponse(Station* pThis, const Packet* pPacket);
EStatus StationHandleTransits(Station* pThis, const Packet* pPacket);
EStatus StationHandleLocals(Station* pThis, const Packet* pPacket);

Boolean StationIsDestination(Station* pThis, const Packet* pPacket);
Boolean StationIsAccepted(Station* pThis, const Packet* pPacket);
Boolean StationIsPacketValid(Station* pThis, const Packet* pPacket);

Boolean StationPacketFilter(Packet* pPacket, Station* pThis);


static PacketHandler s_handlers[eMSG_TYPE_LAST] =
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
	Packet* pPacket;
	VALIDATE_ARGUMENTS(pThis && (timeDelta > 0));

	CHECK(RoutingSynchronize(pThis->pRouting));

	if (pThis->silentTime <= timeDelta) pThis->silentTime = 0;
	else pThis->silentTime -= timeDelta;

	pThis->curLocation.x += pThis->velocity.x * timeDelta;
	pThis->curLocation.y += pThis->velocity.y * timeDelta;

	EStatus ret = SchedulerGetPacket(pThis->pScheduler, &pPacket);
	if (ret == eSTATUS_SCHEDULER_NO_PACKETS) return eSTATUS_COMMON_OK;
	CHECK(ret);

	return StationSendPacket(pThis, pPacket);
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

Boolean StationPacketFilter(Packet* pPacket, Station* pThis)
{
	Packet* pNewMsg = NULL;

	if (!StationIsPacketValid(pThis, pPacket)) return FALSE;
	if (pPacket->transitDstId == BROADCAST_STATION_ID) return TRUE;

	if (eSTATUS_COMMON_OK != RoutingLookFor(pThis->pRouting, pPacket->originalDstId, &pPacket->transitDstId, NULL))
	{
		PacketNewSearchRequest(&pNewMsg, pThis->id, pPacket->originalDstId);
		if (pThis->pReadyPacket)
		{
			CHECK(StationSendPacket(pThis, pNewMsg));
		}
		else pThis->pReadyPacket = pNewMsg;
		RoutingAddPending(pThis->pRouting, pPacket->originalDstId);
		return TRUE;
	}
	if (pPacket->transitDstId == INVALID_STATION_ID) return TRUE;
	if (pThis->pReadyPacket) return TRUE;

	pThis->pReadyPacket = pPacket;
	return FALSE;
}

EStatus StationSendPacket(Station* pThis, Packet* pPacket)
{
	VALIDATE_ARGUMENTS(pThis && pPacket);
	return ListPushBack(pThis->pOutbox, pPacket);
}

EStatus StationGetPacket(Station* pThis, Packet** ppPacket)
{
	VALIDATE_ARGUMENTS(pThis && ppPacket);
	if (pThis->silentTime > 0) return eSTATUS_COMMON_OK;

	pThis->pReadyPacket = NULL;
	CHECK(ListCleanUp(pThis->pOutbox, (ItemFilter)&StationPacketFilter, pThis));
	*ppPacket = pThis->pReadyPacket;

	return eSTATUS_COMMON_OK;
}


EStatus StationPutPacket(Station* pThis, const Packet* pPacket)
{
	VALIDATE_ARGUMENTS(pThis && pPacket && (pPacket->type < eMSG_TYPE_LAST));
	CHECK(SettingsGetTransmitTime(pThis->pSettings, pPacket, &pThis->silentTime));
	CHECK(TimeLineRelativeEvent(pThis->pTimeLine, pThis->silentTime, pPacket));

	if (pPacket->transitSrcId != pThis->id &&
		pPacket->originalSrcId != pThis->id)
	{
		CHECK(RoutingHandlePacket(pThis->pRouting, pPacket));
	}

	if (!StationIsAccepted(pThis, pPacket)) return eSTATUS_STATION_PACKET_NOT_ACCEPTED;
	if (StationHandleLocals(pThis, pPacket) != eSTATUS_COMMON_OK)
	{
		return StationHandleTransits(pThis, pPacket);
	}
	return eSTATUS_COMMON_OK;
}

Boolean StationIsDestination(Station* pThis, const Packet* pPacket)
{
	return (IS_BROADCAST(pPacket->originalDstId)) || (pPacket->originalDstId == pThis->id) ? TRUE : FALSE;
}

Boolean StationIsAccepted(Station* pThis, const Packet* pPacket)
{
	if (pPacket->transitSrcId == pThis->id) return FALSE;
	if (pPacket->originalSrcId == pThis->id) return FALSE;
	return (IS_BROADCAST(pPacket->transitDstId)) || (pPacket->transitDstId == pThis->id) ? TRUE : FALSE;
}

EStatus StationHandleTransits(Station* pThis, const Packet* pPacket)
{
	Packet* pNewMsg = NULL;

	CHECK(PacketClone(&pNewMsg, pPacket));
	pNewMsg->transitSrcId = pThis->id;
	pNewMsg->transitDstId = INVALID_STATION_ID;
	CHECK(StationSendPacket(pThis, pNewMsg));
	return eSTATUS_COMMON_OK;
}

EStatus StationHandleLocals(Station* pThis, const Packet* pPacket)
{
	return s_handlers[pPacket->type](pThis, pPacket);
}

EStatus StationHandleData(Station* pThis, const Packet* pPacket)
{
	Packet* pResponse;
	VALIDATE_ARGUMENTS(pThis && pPacket);
	if (!StationIsDestination(pThis, pPacket)) return eSTATUS_STATION_PACKET_NOT_ACCEPTED;
	CHECK(PacketNewAck(&pResponse, pThis->id, pPacket->originalSrcId));
	pResponse->sequenceNum = pPacket->sequenceNum;
	return StationSendPacket(pThis, pResponse);
}

EStatus StationHandleAck(Station* pThis, const Packet* pPacket)
{
	VALIDATE_ARGUMENTS(pThis && pPacket);
	if (!StationIsDestination(pThis, pPacket)) return eSTATUS_STATION_PACKET_NOT_ACCEPTED;
	return SchedulerHandlePacket(pThis->pScheduler, pPacket);
}

EStatus StationHandleSearchRequest(Station* pThis, const Packet* pPacket)
{
	StationId dstId;
	Packet* pResponse;
	unsigned length = 0;
	VALIDATE_ARGUMENTS(pThis && pPacket);

	if (pPacket->originalDstId != pThis->id)
	{
		CHECK(RoutingLookFor(pThis->pRouting, pPacket->originalDstId, &dstId, &length));
		if (dstId == INVALID_STATION_ID) return eSTATUS_LIST_NOT_FOUND;
	}

	dstId = pPacket->originalSrcId;
	CHECK(PacketNewSearchResponse(&pResponse, pThis->id, dstId));
	pResponse->nodesCount = length;
	pResponse->transitDstId = pPacket->transitSrcId;
	pResponse->originalSrcId = pPacket->originalDstId;
	CHECK(StationSendPacket(pThis, pResponse));

	return eSTATUS_COMMON_OK;
}

EStatus StationHandleSearchResponse(Station* pThis, const Packet* pPacket)
{
	VALIDATE_ARGUMENTS(pThis && pPacket);
	return (StationIsDestination(pThis, pPacket)) ? eSTATUS_COMMON_OK : eSTATUS_STATION_PACKET_NOT_ACCEPTED;
}

EStatus StationSchedulePacket(Station* pThis, Packet* pPacket, double time)
{
	VALIDATE_ARGUMENTS(pThis && pPacket);
	return SchedulerPutPacket(pThis->pScheduler, pPacket, time);
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

Boolean StationIsPacketValid(Station* pThis, const Packet* pPacket)
{
	if (pPacket->type != eMSG_TYPE_SEARCH_REQUEST) return TRUE;
	return RoutingLookFor(pThis->pRouting, pPacket->originalDstId, NULL, NULL) != eSTATUS_COMMON_OK ? TRUE : FALSE;
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

void StationHandleScheduleChange(double time, const Packet* pPacket, ESchedulerEvent event, Station* pThis)
{
	if (pThis->schedulerHandler.callback)
	{
		pThis->schedulerHandler.callback(pThis, time, pPacket, event, pThis->schedulerHandler.pArg);
	}
}
