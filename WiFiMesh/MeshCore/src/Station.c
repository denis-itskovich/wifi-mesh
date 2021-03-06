/*********************************************************************************
WiFi Mesh Simulator PRO
Copyright (C) 2009 Denis Itskovich

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*********************************************************************************/

#include "../inc/Station.h"
#include "../inc/Macros.h"
#include "../inc/List.h"
#include "../inc/Routing.h"
#include "../inc/Scheduler.h"
#include "../inc/Log.h"

int __counter_Station = 0;
int __counter_PacketEntry = 0;

#if 0 // __NO_LOG
    static const char* PKT_EVENTS[] =
    {
        "Added",
        "Update",
        "Delivered",
        "Broadcast",
        "Timeout",
        "Outdated",
        "Clean"
    };
#endif // !__NO_LOG

typedef struct _PacketEntry
{
    Packet*     pPacket;
    unsigned    retriesCount;
    double      nextRetryTime;
    Boolean     bClear;
} PacketEntry;

struct _Station
{
    StationId       id;
    Velocity        velocity;
    Location        curLocation;
    Location        orgLocation;

    Routing*        pRouting;
    List*           pOutbox;
    Scheduler*      pScheduler;
    Settings*       pSettings;
    TimeLine*       pTimeLine;
    double          silentTime;
    double          receiveTime;
    PacketEntry*    pOutPacketEntry;
    Packet*         pInPacket;
    Boolean         bBusy;
    Boolean         bTransmitting;
    unsigned        sequenceNum;
    int             freeBufferSize;
    double          lastTime;

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

	struct
	{
	    StationOutboxHandler   callback;
	    void*                  pArg;
	    EOutboxEvent           event;

	}          outboxHandler;
};

typedef EStatus (*PacketHandler)(Station* pThis, const Packet* pPacket);

void StationHandleRoutingChange(StationId destId,
                                StationId transId,
                                double expirationTime,
                                int length,
                                int retriesLeft,
                                ERouteEntryUpdate updateAction,
                                Station* pThis);

void StationHandleScheduleChange(const SchedulerEntry* pEntry,
                                 Station* pThis);

EStatus StationSendAck(Station* pThis, const Packet* pPacket);
EStatus StationSendPacket(Station* pThis, Packet* pPacket);
EStatus StationNewPacketEntry(Station* pThis, Packet* pPacket, PacketEntry** ppEntry);
EStatus StationHandlePacket(Station* pThis, const Packet* pPacket);
EStatus StationHandleData(Station* pThis, const Packet* pPacket);
EStatus StationHandleAck(Station* pThis, const Packet* pPacket);
EStatus StationHandleSearchRequest(Station* pThis, const Packet* pPacket);
EStatus StationHandleSearchResponse(Station* pThis, const Packet* pPacket);
EStatus StationHandleTransits(Station* pThis, const Packet* pPacket);
EStatus StationHandleLocals(Station* pThis, const Packet* pPacket);

EStatus StationRemovePacket(Station* pThis, PacketEntry* pEntry, EOutboxEvent event);

Boolean StationIsDestination(Station* pThis, const Packet* pPacket);
Boolean StationIsAccepted(Station* pThis, const Packet* pPacket);
Boolean StationIsPacketValid(Station* pThis, const Packet* pPacket);
Boolean StationIsPacketPrioritized(Station* pThis, const Packet* pPacket);
Boolean StationIsAckRequired(Station* pThis, const Packet* pPacket);


Boolean StationPacketFilter(PacketEntry* pEntry, Station* pThis);
Boolean StationPacketAcknowledger(PacketEntry* pEntry, Station* pThis);
Boolean StationPacketCleaner(PacketEntry* pEntry, Station* pThis);
Boolean StationDumpPacketEntry(PacketEntry* pEntry, void* pArg);


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
	pThis->id = INVALID_STATION_ID;

	CHECK(RoutingNew(&pThis->pRouting, pSettings, pTimeLine));
	CHECK(ListNew(&pThis->pOutbox));
	CHECK(SchedulerNew(&pThis->pScheduler, pTimeLine));

	return eSTATUS_COMMON_OK;
}

EStatus StationDestroy(Station* pThis)
{
    BEGIN_FUNCTION;
	VALIDATE_ARGUMENTS(pThis);

	CHECK(StationReset(pThis));
    CHECK(SchedulerDelete(&pThis->pScheduler));
	CHECK(ListDelete(&pThis->pOutbox));
	CHECK(RoutingDelete(&pThis->pRouting));

	END_FUNCTION;
	return eSTATUS_COMMON_OK;
}

EStatus StationSynchronize(Station* pThis, Packet** ppDeliveredPacket)
{
    Packet* pPacket;
    Boolean isActive;
    double time = 0;
	EStatus ret = eSTATUS_COMMON_OK;

    VALIDATE_ARGUMENTS(pThis && ppDeliveredPacket);
    *ppDeliveredPacket = NULL;

    CHECK(RoutingSynchronize(pThis->pRouting));
    CHECK(TimeLineGetTime(pThis->pTimeLine, &time));

    if (pThis->receiveTime >= time)
    {
        CHECK(StationIsActive(pThis, &isActive));
        if (!isActive || pThis->receiveTime == time)
        {
            pThis->bBusy = FALSE;
            pThis->bTransmitting = FALSE;
            if (pThis->pInPacket)
            {
                if (StationHandlePacket(pThis, pThis->pInPacket) == eSTATUS_COMMON_OK)
                {
                    *ppDeliveredPacket = pThis->pInPacket;
                    pThis->pInPacket = NULL;
                }

                if (pThis->pInPacket)
                {
                    PacketDelete(&pThis->pInPacket);
                }
            }
        }
	}

    ret = SchedulerGetPacket(pThis->pScheduler, &pPacket);
    if (ret == eSTATUS_SCHEDULER_NO_PACKETS) return eSTATUS_COMMON_OK;
    CHECK(ret);

    return StationSendPacket(pThis, pPacket);
}

EStatus StationAdvance(Station* pThis)
{
    double time = 0;
    double timeDelta = 0;
    VALIDATE_ARGUMENTS(pThis);

    CHECK(TimeLineGetTime(pThis->pTimeLine, &time));

    timeDelta = time - pThis->lastTime;
    pThis->lastTime = time;

    pThis->curLocation.x += pThis->velocity.x * timeDelta;
    pThis->curLocation.y += pThis->velocity.y * timeDelta;

    return eSTATUS_COMMON_OK;
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

EStatus StationGetRetryTime(Station* pThis, PacketEntry* pEntry)
{
    double timeout;
    double time;
    double transmitTime = 0.0;
    VALIDATE_ARGUMENTS(pThis && pEntry);
    // CHECK(SettingsGetTransmitTime(pThis->pSettings, pEntry->pPacket, &transmitTime));
    CHECK(SettingsGetPacketRetryTimeout(pThis->pSettings, &timeout));
    CHECK(TimeLineGetTime(pThis->pTimeLine, &time));

    timeout += transmitTime;
    // VARIANCE(timeout, 0.1);

    pEntry->nextRetryTime = time + timeout;
    return eSTATUS_COMMON_OK;
}

Boolean StationPacketFilter(PacketEntry* pEntry, Station* pThis)
{
    Packet* pPacket = pEntry->pPacket;
	Packet* pNewMsg = NULL;
	double time;
	unsigned length = 0;

	if (!StationIsPacketValid(pThis, pPacket))
	{
	    StationRemovePacket(pThis, pEntry, ePKT_EVENT_REMOVED_OUTDATED);
	    return FALSE;
	}

	// Checking route table whether there is a path for destination
	if ((pPacket->header.transitDstId != BROADCAST_STATION_ID) &&
	    (eSTATUS_COMMON_OK != RoutingLookFor(pThis->pRouting, pPacket->header.originalDstId, &pPacket->header.transitDstId, &length)))
	{
	    // Path does not exist, not in active and not in pending destinations
	    // so we are sending another one path query
		PacketNewSearchRequest(&pNewMsg, pThis->id, pPacket->header.originalDstId);

		// if there is ready packet we will schedule it for later transmitting
		if (pThis->pOutPacketEntry)
		{
			StationSendPacket(pThis, pNewMsg);
		}
		else
        {
		    StationNewPacketEntry(pThis, pNewMsg, &pThis->pOutPacketEntry);
		    if (pThis->pOutPacketEntry) pThis->pOutPacketEntry->bClear = TRUE;
		    else PacketDelete(&pNewMsg);
        }

		// Adding pending destination
		RoutingAddPending(pThis->pRouting, pPacket->header.originalDstId);
		return TRUE;
	}

    // Packet already exists
    if (pThis->pOutPacketEntry) return TRUE;

	// Destination is pending for a path query response
    if (pPacket->header.transitDstId == INVALID_STATION_ID) return TRUE;

    // Check TTL/length matching
    if (length > pPacket->header.ttl) return TRUE;

    // Checking whether we should retry a packet now
    TimeLineGetTime(pThis->pTimeLine, &time);
    if (time < pEntry->nextRetryTime) return TRUE;

    // Last retry attempt, so no need to schedule next retry
    if (pEntry->retriesCount-- == 0)
    {
        RoutingRemoveTransit(pThis->pRouting, pPacket->header.transitDstId);
        StationRemovePacket(pThis, pEntry, ePKT_EVENT_REMOVED_TIMEOUT);
        return FALSE;
    }

    // Assign this packet for transmitting
    pThis->pOutPacketEntry = pEntry;

    if (!StationIsAckRequired(pThis, pPacket))
    {
        pEntry->bClear = TRUE;
        return FALSE;
    }

    // Scheduling retry transmit for this packet
    StationGetRetryTime(pThis, pEntry);
    TimeLineEvent(pThis->pTimeLine, pEntry->nextRetryTime, pPacket);

    return TRUE;
}

EStatus StationRemovePacket(Station* pThis, PacketEntry* pEntry, EOutboxEvent event)
{
    pThis->outboxHandler.event = event;
    StationPacketCleaner(pEntry, pThis);
    return eSTATUS_COMMON_OK;
}

Boolean StationPacketCleaner(PacketEntry* pEntry, Station* pThis)
{
    unsigned size;
    PacketGetSize(pEntry->pPacket, &size);
    pThis->freeBufferSize += size;

    if (pThis->outboxHandler.callback)
    {
        pThis->outboxHandler.callback(pThis,
                                      pEntry->pPacket,
                                      pThis->outboxHandler.event,
                                      pEntry->retriesCount,
                                      pEntry->nextRetryTime,
                                      pThis->outboxHandler.pArg);
    }
    PacketDelete(&pEntry->pPacket);
    DELETE(PacketEntry, pEntry);

    return FALSE;
}

Boolean StationPacketAcknowledger(PacketEntry* pEntry, Station* pThis)
{
    Packet* pAckPacket = pThis->pInPacket;
    if (pEntry->pPacket->header.sequenceNum != pAckPacket->header.sequenceNum) return TRUE;
    StationRemovePacket(pThis, pEntry, ePKT_EVENT_REMOVED_DELIVERED);
    return FALSE;
}

EStatus StationNewPacketEntry(Station* pThis, Packet* pPacket, PacketEntry** ppEntry)
{
    PacketEntry* pEntry;
    int size;
    VALIDATE_ARGUMENTS(pThis && pPacket && ppEntry);

    *ppEntry = NULL;
    CHECK(PacketGetSize(pPacket, &size));
    if (pThis->freeBufferSize < size)
	{
        return eSTATUS_COMMON_OK;
	}

    pThis->freeBufferSize -= size;

    pEntry = NEW(PacketEntry);
    CLEAR(pEntry);
    pEntry->pPacket = pPacket;
    pEntry->retriesCount = 1;

    if (StationIsAckRequired(pThis, pPacket))
        CHECK(SettingsGetPacketRetryThreshold(pThis->pSettings, &pEntry->retriesCount));

    *ppEntry = pEntry;

    return eSTATUS_COMMON_OK;
}

Comparison StationAckFinder(PacketEntry* pEntry, const Packet* pPacket, Station* pThis)
{
    return (pEntry->pPacket->header.type == ePKT_TYPE_ACK) &&
           (pEntry->pPacket->header.transitDstId == pPacket->header.transitSrcId) &&
           (pEntry->pPacket->header.sequenceNum == pPacket->header.sequenceNum) ? EQUAL : LESS;
}

EStatus StationSendAck(Station* pThis, const Packet* pPacket)
{
    Packet* pAck;
    ListEntry* pEntry;
    if (!StationIsAckRequired(pThis, pPacket)) return eSTATUS_COMMON_OK;
    NCHECK(ListFind(pThis->pOutbox, &pEntry, (ItemComparator)&StationAckFinder, (void*)pPacket, pThis));
    CHECK(PacketNewAck(&pAck, pThis->id, pPacket->header.transitSrcId));
    pAck->header.sequenceNum = pPacket->header.sequenceNum;
    return StationSendPacket(pThis, pAck);
}

EStatus StationSendPacket(Station* pThis, Packet* pPacket)
{
    PacketEntry* pEntry;

	VALIDATE_ARGUMENTS(pThis && pPacket);
	if (pPacket->header.type != ePKT_TYPE_ACK) pPacket->header.sequenceNum = pThis->sequenceNum++;
	StationNewPacketEntry(pThis, pPacket, &pEntry);
	if (!pEntry)
	{
	    PacketDelete(&pPacket);
	    return eSTATUS_COMMON_OK;
	}

    if (pThis->outboxHandler.callback)
    {
        pThis->outboxHandler.callback(pThis,
                                      pEntry->pPacket,
                                      ePKT_EVENT_ADDED,
                                      pEntry->retriesCount,
                                      pEntry->nextRetryTime,
                                      pThis->outboxHandler.pArg);
    }

	if (StationIsPacketPrioritized(pThis, pPacket)) CHECK(ListPushFront(pThis->pOutbox, pEntry));
	else CHECK(ListPushBack(pThis->pOutbox, pEntry));

	// Don't check the result: it can fail if silentTime is in the past - just ignore this case
	// TimeLineEvent(pThis->pTimeLine, pThis->silentTime, pPacket);

	return eSTATUS_COMMON_OK;
}

EStatus StationTransmitPacket(Station* pThis, Packet** ppPacket)
{
    int size;
    double time;
	VALIDATE_ARGUMENTS(pThis && ppPacket);

	CHECK(TimeLineGetTime(pThis->pTimeLine, &time));
	if (pThis->silentTime > time) return eSTATUS_COMMON_OK;

	pThis->pOutPacketEntry = NULL;
	CHECK(ListCleanUp(pThis->pOutbox, (ItemFilter)&StationPacketFilter, pThis));
	if (!pThis->pOutPacketEntry) return eSTATUS_COMMON_OK;

	if (!pThis->pOutPacketEntry->bClear)
	{
	    CHECK(PacketClone(ppPacket, pThis->pOutPacketEntry->pPacket));
	}
	else
    {
	    *ppPacket = pThis->pOutPacketEntry->pPacket;
        CHECK(PacketGetSize(*ppPacket, &size));
        pThis->freeBufferSize += size;
	    DELETE(PacketEntry, pThis->pOutPacketEntry);
    }

	pThis->pOutPacketEntry = NULL;

	return eSTATUS_COMMON_OK;
}

EStatus StationUpdateTiming(Station* pThis, const Packet* pPacket)
{
    double time, receiveTime, silenceTime;

    CHECK(TimeLineGetTime(pThis->pTimeLine, &time));
    CHECK(SettingsGetTransmitTime(pThis->pSettings, pPacket, &receiveTime));
    CHECK(SettingsGetSilenceTime(pThis->pSettings, pPacket, &silenceTime));

    receiveTime += time;
    silenceTime += time;

    if (pThis->silentTime < silenceTime)
    {
        CHECK(TimeLineEvent(pThis->pTimeLine, silenceTime, pPacket));
        pThis->silentTime = silenceTime;
    }

    if (pThis->id == pPacket->header.transitSrcId)
    {
        pThis->bTransmitting = TRUE;
        CHECK(TimeLineEvent(pThis->pTimeLine, receiveTime, pPacket));
        pThis->bBusy = TRUE;
        pThis->receiveTime = receiveTime;
        return eSTATUS_COMMON_OK;
    }

    if (!pThis->bBusy)
    {
        pThis->receiveTime = receiveTime;
        pThis->bBusy = TRUE;
    }

    return eSTATUS_COMMON_OK;
}

EStatus StationReceivePacket(Station* pThis, const Packet* pPacket, Packet** ppAbortedPacket)
{
    EStatus ret = eSTATUS_COMMON_OK;

	VALIDATE_ARGUMENTS(pThis && pPacket && (pPacket->header.type < ePKT_TYPE_LAST));

    *ppAbortedPacket = NULL;

    if (!pThis->bBusy)
    {
        CHECK(PacketClone(&pThis->pInPacket, pPacket));
    }
    else
    {
        ret = eSTATUS_STATION_PACKET_COLLISION;

        if (pThis->pInPacket && StationIsAccepted(pThis, pThis->pInPacket))
        {
            *ppAbortedPacket = pThis->pInPacket;
            pThis->pInPacket = NULL;
        }

        if (pThis->pInPacket)
        {
            PacketDelete(&pThis->pInPacket);
        }
    }

    if (!StationIsAccepted(pThis, pPacket))
    {
        ret = eSTATUS_STATION_PACKET_NOT_ACCEPTED;
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

	if (!StationIsAccepted(pThis, pPacket)) return eSTATUS_STATION_PACKET_NOT_ACCEPTED;

	if (StationHandleLocals(pThis, pPacket) != eSTATUS_COMMON_OK)
	{
		CHECK(StationHandleTransits(pThis, pPacket));
	}
    CHECK(StationSendAck(pThis, pPacket));

	return eSTATUS_COMMON_OK;
}

Boolean StationIsAckRequired(Station* pThis, const Packet* pPacket)
{
    // No ACKs required for broadcast and ACK packets
    return (pPacket->header.type == ePKT_TYPE_ACK || pPacket->header.transitDstId == BROADCAST_STATION_ID) ? FALSE : TRUE;
}

Boolean StationIsDestination(Station* pThis, const Packet* pPacket)
{
	return (IS_BROADCAST(pPacket->header.originalDstId)) || (pPacket->header.originalDstId == pThis->id) ? TRUE : FALSE;
}

Boolean StationIsAccepted(Station* pThis, const Packet* pPacket)
{
    unsigned i;

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

	CHECK(StationSendPacket(pThis, pNewMsg));
	return eSTATUS_COMMON_OK;
}

EStatus StationHandleLocals(Station* pThis, const Packet* pPacket)
{
	return s_handlers[pPacket->header.type](pThis, pPacket);
}

EStatus StationHandleData(Station* pThis, const Packet* pPacket)
{
	VALIDATE_ARGUMENTS(pThis && pPacket);
	if (!StationIsDestination(pThis, pPacket)) return eSTATUS_STATION_PACKET_NOT_ACCEPTED;
	return eSTATUS_COMMON_OK;
}

EStatus StationHandleAck(Station* pThis, const Packet* pPacket)
{
	VALIDATE_ARGUMENTS(pThis && pPacket);
	if (!StationIsDestination(pThis, pPacket)) return eSTATUS_STATION_PACKET_NOT_ACCEPTED;
	CHECK(ListCleanUp(pThis->pOutbox, (ItemFilter)&StationPacketAcknowledger, pThis));
	return eSTATUS_COMMON_OK;
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
	CHECK(PacketNewSearchResponse(&pResponse, pThis->id, dstId, pPacket->header.hopsCount));
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

EStatus StationGetFreeBuffer(const Station* pThis, int* pSize)
{
    GET_MEMBER(pSize, pThis, freeBufferSize);
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

	INFO_PRINT("Reseting station");

	pThis->curLocation = pThis->orgLocation;

	if (pThis->pInPacket) CHECK(PacketDelete(&pThis->pInPacket));

	if (pThis->pOutPacketEntry)
    {
	    CHECK(StationRemovePacket(pThis, pThis->pOutPacketEntry, ePKT_EVENT_REMOVED_CLEAN));
	    pThis->pOutPacketEntry = NULL;
    }

	pThis->bBusy = FALSE;
	pThis->bTransmitting = FALSE;
    pThis->silentTime = -1;
    pThis->receiveTime = -1;
    pThis->sequenceNum = 0;
    pThis->outboxHandler.event = ePKT_EVENT_REMOVED_CLEAN;
    pThis->lastTime = 0;

    CHECK(RoutingClear(pThis->pRouting));
    CHECK(SchedulerReset(pThis->pScheduler));

    DUMP_INSTANCE(Station, pThis);

    CHECK(ListCleanUp(pThis->pOutbox, (ItemFilter)&StationPacketCleaner, pThis));
    CHECK(SettingsGetRelayBufferSize(pThis->pSettings, &pThis->freeBufferSize));

	return eSTATUS_COMMON_OK;
}

Boolean StationIsPacketPrioritized(Station* pThis, const Packet* pPacket)
{
    return (pPacket->header.type == ePKT_TYPE_ACK) ? TRUE : FALSE;
}

Boolean StationIsPacketValid(Station* pThis, const Packet* pPacket)
{
	if (pPacket->header.type != ePKT_TYPE_SEARCH_REQUEST) return TRUE;
	if (RoutingLookFor(pThis->pRouting, pPacket->header.originalDstId, NULL, NULL) == eSTATUS_COMMON_OK) return FALSE;
	RoutingAddPending(pThis->pRouting, pPacket->header.originalDstId);
	return TRUE;
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

EStatus StationRegisterOutboxHandler(Station* pThis, StationOutboxHandler handler, void* pUserArg)
{
    VALIDATE_ARGUMENTS(pThis);
    pThis->outboxHandler.callback = handler;
    pThis->outboxHandler.pArg = pUserArg;
    return eSTATUS_COMMON_OK;
}

void StationHandleRoutingChange(StationId destId,
								StationId transId,
								double expirationTime,
								int length,
								int retriesLeft,
								ERouteEntryUpdate updateAction,
								Station* pThis)
{
	if (pThis->routingHandler.callback)
	{
		pThis->routingHandler.callback(pThis,
		                               destId,
		                               transId,
		                               expirationTime,
		                               length,
		                               retriesLeft,
		                               updateAction,
		                               pThis->routingHandler.pArg);
	}
}

void StationHandleScheduleChange(const SchedulerEntry* pEntry, Station* pThis)
{
	if (pThis->schedulerHandler.callback)
	{
		pThis->schedulerHandler.callback(pThis, pEntry, pThis->schedulerHandler.pArg);
	}
}

EStatus StationPacketDelivered(Station* pThis, const Packet* pPacket)
{
    VALIDATE_ARGUMENTS(pThis && pPacket);
    return SchedulerPacketDelivered(pThis->pScheduler, pPacket);
}

EStatus StationExport(const Station* pThis, FILE* file)
{
    return SchedulerExport(pThis->pScheduler, file);
}

EStatus StationDump(const Station* pThis)
{
    DUMP_PRINT("Station: [id=%d]", (int)pThis->id);
    DUMP_PRINT("Outbox:");
    CHECK(ListEnumerate(pThis->pOutbox, (ItemEnumerator)&StationDumpPacketEntry, NULL));
    DUMP_INSTANCE(Routing, pThis->pRouting);
    DUMP_INSTANCE(Scheduler, pThis->pScheduler);
    return eSTATUS_COMMON_OK;
}

Boolean StationDumpPacketEntry(PacketEntry* pEntry, void* pArg)
{
    DUMP_PRINT("PacketEntry: [retriesCount=%d, nextRetryTime=%f]", pEntry->retriesCount, pEntry->nextRetryTime);
    DUMP_INSTANCE(Packet, pEntry->pPacket);
    return TRUE;
}
