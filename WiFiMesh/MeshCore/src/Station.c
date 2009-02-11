#include "Station.h"
#include "Macros.h"
#include "List.h"
#include "Routing.h"
#include "Scheduler.h"

#define ASSERT(pred) SAFE_OPERATION(if (!(pred)) asm("int3;"); )

struct _Station
{
	StationId   id;
	Velocity    velocity;
	Location    curLocation;
	Location    orgLocation;

	Routing*    pRouting;
	List*       pOutbox;
	Scheduler*  pScheduler;
	Settings*   pSettings;
	TimeLine*   pTimeLine;
	double      silentTime;
	double      receiveTime;
    Packet*     pOutPacket;
    Packet*     pInPacket;
    Boolean     bBusy;
    Boolean     bTransmitting;

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

void StationHandleScheduleChange(double time,
								 const Packet* pPacket,
								 ESchedulerEvent event,
								 Station* pThis);

EStatus StationSendPacket(Station* pThis, Packet* pPacket);
EStatus StationHandlePacket(Station* pThis, const Packet* pPacket);
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


static PacketHandler s_handlers[ePKT_TYPE_LAST] =
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

EStatus StationSynchronize(Station* pThis, double timeDelta, Packet** ppDeliveredPacket)
{
	Packet* pPacket;
	Boolean isActive;
	VALIDATE_ARGUMENTS(pThis && ppDeliveredPacket);
	*ppDeliveredPacket = NULL;

	CHECK(RoutingSynchronize(pThis->pRouting));

    pThis->curLocation.x += pThis->velocity.x * timeDelta;
    pThis->curLocation.y += pThis->velocity.y * timeDelta;

	if (pThis->receiveTime > 0)
	{
	    CHECK(StationIsActive(pThis, &isActive));
	    if (!isActive || pThis->receiveTime <= timeDelta)
        {
            pThis->receiveTime = 0;
            pThis->bBusy = FALSE;
            pThis->bTransmitting = FALSE;
            if (pThis->pInPacket)
            {
                if (isActive) CHECK(StationHandlePacket(pThis, pThis->pInPacket));
                *ppDeliveredPacket = pThis->pInPacket;
                pThis->pInPacket = NULL;
            }
        }
        else pThis->receiveTime -= timeDelta;
	}

	if (pThis->silentTime <= timeDelta)
	{
		pThis->silentTime = 0;
	}
	else pThis->silentTime -= timeDelta;

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

	if ((pPacket->header.transitDstId != BROADCAST_STATION_ID) &&
	    (eSTATUS_COMMON_OK != RoutingLookFor(pThis->pRouting, pPacket->header.originalDstId, &pPacket->header.transitDstId, NULL)))
	{
		PacketNewSearchRequest(&pNewMsg, pThis->id, pPacket->header.originalDstId);
		if (pThis->pOutPacket)
		{
			CHECK(StationSendPacket(pThis, pNewMsg));
		}
		else pThis->pOutPacket = pNewMsg;
		RoutingAddPending(pThis->pRouting, pPacket->header.originalDstId);
		return TRUE;
	}
	if (pPacket->header.transitDstId == INVALID_STATION_ID) return TRUE;
	if (pThis->pOutPacket) return TRUE;

	pThis->pOutPacket = pPacket;
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

	pThis->pOutPacket = NULL;
	CHECK(ListCleanUp(pThis->pOutbox, (ItemFilter)&StationPacketFilter, pThis));
	*ppPacket = pThis->pOutPacket;

	return eSTATUS_COMMON_OK;
}

EStatus StationUpdateTiming(Station* pThis, const Packet* pPacket)
{
    double receiveTime, silenceTime;
    CHECK(SettingsGetSilenceTime(pThis->pSettings, pPacket, &silenceTime));

    if (pThis->silentTime < silenceTime)
    {
        pThis->silentTime = silenceTime;
        CHECK(TimeLineRelativeEvent(pThis->pTimeLine, pThis->silentTime, pPacket));
    }

    if (pThis->id == pPacket->header.transitSrcId)
    {
        pThis->bTransmitting = TRUE;
        CHECK(SettingsGetTransmitTime(pThis->pSettings, pPacket, &receiveTime));
        CHECK(TimeLineRelativeEvent(pThis->pTimeLine, receiveTime, pPacket));
        pThis->bBusy = TRUE;
        pThis->receiveTime = receiveTime;
        return eSTATUS_COMMON_OK;
    }

    if (!pThis->bBusy)
    {
        CHECK(SettingsGetTransmitTime(pThis->pSettings, pPacket, &receiveTime));
        pThis->receiveTime = receiveTime;
        pThis->bBusy = TRUE;
    }

    return eSTATUS_COMMON_OK;
}

EStatus StationPutPacket(Station* pThis, const Packet* pPacket, Packet** ppAbortedPacket)
{
    VALIDATE_ARGUMENTS(pThis && pPacket && (pPacket->header.type < ePKT_TYPE_LAST));

    EStatus ret = eSTATUS_COMMON_OK;
    *ppAbortedPacket = NULL;

    if (!pThis->bBusy)
    {
        if (StationIsAccepted(pThis, pPacket))
        {
            ASSERT(pThis->pInPacket == NULL);
            CHECK(PacketClone(&pThis->pInPacket, pPacket));
        }
        else
        {
            ret = eSTATUS_STATION_PACKET_NOT_ACCEPTED;
        }
    }
    else
    {
        *ppAbortedPacket = pThis->pInPacket;
        pThis->pInPacket = NULL;
        ret = eSTATUS_STATION_PACKET_COLLISION;
    }

    CHECK(StationUpdateTiming(pThis, pPacket));
    return ret;
}

EStatus StationHandlePacket(Station* pThis, const Packet* pPacket)
{
	VALIDATE_ARGUMENTS(pThis && pPacket && (pPacket->header.type < ePKT_TYPE_LAST));

	if (pPacket->header.transitSrcId != pThis->id &&
		pPacket->header.originalSrcId != pThis->id)
	{
		CHECK(RoutingHandlePacket(pThis->pRouting, pPacket));
	}

	if (!StationIsAccepted(pThis, pPacket)) return eSTATUS_COMMON_OK;

	if (StationHandleLocals(pThis, pPacket) != eSTATUS_COMMON_OK)
	{
		return StationHandleTransits(pThis, pPacket);
	}
	return eSTATUS_COMMON_OK;
}

Boolean StationIsDestination(Station* pThis, const Packet* pPacket)
{
	return (IS_BROADCAST(pPacket->header.originalDstId)) || (pPacket->header.originalDstId == pThis->id) ? TRUE : FALSE;
}

Boolean StationIsAccepted(Station* pThis, const Packet* pPacket)
{
    int i;

    for (i = 0; i < pPacket->routing.length; ++i)
    {
        if (pPacket->routing.path[i] == pThis->id) return FALSE;
    }

    return (IS_BROADCAST(pPacket->header.transitDstId) ||
	       (pPacket->header.transitDstId == pThis->id)) ? TRUE : FALSE;
}

EStatus StationHandleTransits(Station* pThis, const Packet* pPacket)
{
	Packet* pNewMsg = NULL;

	CHECK(PacketClone(&pNewMsg, pPacket));
	pNewMsg->header.transitSrcId = pThis->id;
	//pNewMsg->header.transitDstId = INVALID_STATION_ID;
	CHECK(StationSendPacket(pThis, pNewMsg));
	return eSTATUS_COMMON_OK;
}

EStatus StationHandleLocals(Station* pThis, const Packet* pPacket)
{
	return s_handlers[pPacket->header.type](pThis, pPacket);
}

EStatus StationHandleData(Station* pThis, const Packet* pPacket)
{
	Packet* pResponse;
	VALIDATE_ARGUMENTS(pThis && pPacket);
	if (!StationIsDestination(pThis, pPacket)) return eSTATUS_STATION_PACKET_NOT_ACCEPTED;
	CHECK(PacketNewAck(&pResponse, pThis->id, pPacket->header.originalSrcId));
	pResponse->header.sequenceNum = pPacket->header.sequenceNum;
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

	if (pPacket->header.originalDstId != pThis->id)
	{
		CHECK(RoutingLookFor(pThis->pRouting, pPacket->header.originalDstId, &dstId, &length));
		if (dstId == INVALID_STATION_ID) return eSTATUS_LIST_NOT_FOUND;
	}

	dstId = pPacket->header.originalSrcId;
	CHECK(PacketNewSearchResponse(&pResponse, pThis->id, dstId));
	pResponse->header.hopsCount = length;
	pResponse->header.transitDstId = pPacket->header.transitSrcId;
	pResponse->header.originalSrcId = pPacket->header.originalDstId;
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

EStatus StationIsTransmitting(const Station* pThis, Boolean* pIsTxing)
{
    VALIDATE_ARGUMENTS(pThis && pIsTxing);
    *pIsTxing = pThis->bTransmitting;
    return eSTATUS_COMMON_OK;
}

EStatus StationReset(Station* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	pThis->curLocation = pThis->orgLocation;

	if (pThis->pInPacket) CHECK(PacketDelete(&pThis->pInPacket));
	if (pThis->pOutPacket) CHECK(PacketDelete(&pThis->pOutPacket));
	pThis->bBusy = FALSE;
	pThis->bTransmitting = FALSE;
    pThis->silentTime = 0;
    pThis->receiveTime = 0;
	CHECK(ListCleanUp(pThis->pOutbox, NULL, NULL));
	CHECK(RoutingClear(pThis->pRouting));
	CHECK(SchedulerReset(pThis->pScheduler));

	return eSTATUS_COMMON_OK;
}

Boolean StationIsPacketValid(Station* pThis, const Packet* pPacket)
{
	if (pPacket->header.type != ePKT_TYPE_SEARCH_REQUEST) return TRUE;
	return RoutingLookFor(pThis->pRouting, pPacket->header.originalDstId, NULL, NULL) != eSTATUS_COMMON_OK ? TRUE : FALSE;
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
