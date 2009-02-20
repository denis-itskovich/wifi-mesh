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

#include "Station.h"
#include "Macros.h"
#include "List.h"
#include "Routing.h"
#include "Scheduler.h"
#include "Log.h"

#define ASSERT(pred) SAFE_OPERATION(if (!(pred)) asm("int3;"); )

#ifndef __NO_LOG
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
	StationId      id;
	Velocity       velocity;
	Location       curLocation;
	Location       orgLocation;

	Routing*       pRouting;
	List*          pOutbox;
	Scheduler*     pScheduler;
	Settings*      pSettings;
	TimeLine*      pTimeLine;
	double         silentTime;
	double         receiveTime;
    PacketEntry*   pOutPacketEntry;
    Packet*        pInPacket;
    Boolean        bBusy;
    Boolean        bTransmitting;
    unsigned       sequenceNum;

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
								ERouteEntryUpdate updateAction,
								Station* pThis);

void StationHandleScheduleChange(double time,
								 const Packet* pPacket,
								 ESchedulerEvent event,
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

	CHECK(RoutingNew(&pThis->pRouting, pSettings, pTimeLine));
	CHECK(ListNew(&pThis->pOutbox));
	CHECK(SchedulerNew(&pThis->pScheduler, pTimeLine));

	return eSTATUS_COMMON_OK;
}

EStatus StationDestroy(Station* pThis)
{
	VALIDATE_ARGUMENTS(pThis);

    CHECK(SchedulerDelete(&pThis->pScheduler));
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

Boolean StationPacketFilter(PacketEntry* pEntry, Station* pThis)
{
    Packet* pPacket = pEntry->pPacket;
	Packet* pNewMsg = NULL;
	double time;

	if (!StationIsPacketValid(pThis, pPacket))
	{
	    StationRemovePacket(pThis, pEntry, ePKT_EVENT_REMOVED_OUTDATED);
	    return FALSE;
	}

	// Checking route table whether there is a path for destination
	if ((pPacket->header.transitDstId != BROADCAST_STATION_ID) &&
	    (eSTATUS_COMMON_OK != RoutingLookFor(pThis->pRouting, pPacket->header.originalDstId, &pPacket->header.transitDstId, NULL)))
	{
	    // Path does not exist, not in active and not in pending destinations
	    // so we are sending another one path query
		PacketNewSearchRequest(&pNewMsg, pThis->id, pPacket->header.originalDstId);

		// if there is ready packet we will schedule it for later transmitting
		if (pThis->pOutPacketEntry)
		{
			CHECK(StationSendPacket(pThis, pNewMsg));
		}
		else
        {
		    CHECK(StationNewPacketEntry(pThis, pNewMsg, &pThis->pOutPacketEntry));
        }

		// Adding pending destination
		RoutingAddPending(pThis->pRouting, pPacket->header.originalDstId);
		return TRUE;
	}

    // Packet already exists
    if (pThis->pOutPacketEntry) return TRUE;

	// Destination is pending for a path query response
    if (pPacket->header.transitDstId == INVALID_STATION_ID) return TRUE;

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
    SettingsGetPacketRetryTimeout(pThis->pSettings, &pEntry->nextRetryTime);
    pEntry->nextRetryTime += time;
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
    INFO_PRINT("Removing outbox entry: %s", PKT_EVENTS[pThis->outboxHandler.event]);
    StationDumpPacketEntry(pEntry, NULL);

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
    DELETE(pEntry);

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
    VALIDATE_ARGUMENTS(pThis && pPacket && ppEntry);

    PacketEntry* pEntry = NEW(PacketEntry);
    CLEAR(pEntry);
    pEntry->pPacket = pPacket;
    pEntry->retriesCount = 1;

    if (StationIsAckRequired(pThis, pPacket))
        CHECK(SettingsGetPacketRetryThreshold(pThis->pSettings, &pEntry->retriesCount));

    *ppEntry = pEntry;

    return eSTATUS_COMMON_OK;
}

EStatus StationSendAck(Station* pThis, const Packet* pPacket)
{
    Packet* pAck;
    if (!StationIsAckRequired(pThis, pPacket)) return eSTATUS_COMMON_OK;
    CHECK(PacketNewAck(&pAck, pThis->id, pPacket->header.transitSrcId));
    pAck->header.sequenceNum = pPacket->header.sequenceNum;
    return StationSendPacket(pThis, pAck);
}

EStatus StationSendPacket(Station* pThis, Packet* pPacket)
{
    PacketEntry* pEntry;
	VALIDATE_ARGUMENTS(pThis && pPacket);
	if (pPacket->header.type != ePKT_TYPE_ACK) pPacket->header.sequenceNum = pThis->sequenceNum++;
	CHECK(StationNewPacketEntry(pThis, pPacket, &pEntry));

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
	INFO_PRINT("Adding outbox entry: ");
	StationDumpPacketEntry(pEntry, NULL);
	return eSTATUS_COMMON_OK;
}

EStatus StationGetPacket(Station* pThis, Packet** ppPacket)
{
	VALIDATE_ARGUMENTS(pThis && ppPacket);
	if (pThis->silentTime > 0) return eSTATUS_COMMON_OK;

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
	    DELETE(pThis->pOutPacketEntry);
    }

	pThis->pOutPacketEntry = NULL;

	return eSTATUS_COMMON_OK;
}

EStatus StationUpdateTiming(Station* pThis, const Packet* pPacket)
{
    double receiveTime, silenceTime;
    Boolean isEmpty;

    CHECK(SettingsGetTransmitTime(pThis->pSettings, pPacket, &receiveTime));
    silenceTime = receiveTime;

    if (pThis->silentTime < silenceTime)
    {
        CHECK(ListIsEmpty(pThis->pOutbox, &isEmpty));

        if (!isEmpty)
            CHECK(SettingsGetSilenceTime(pThis->pSettings, pPacket, &silenceTime));

        pThis->silentTime = silenceTime;
        CHECK(TimeLineRelativeEvent(pThis->pTimeLine, pThis->silentTime, pPacket));
    }

    if (pThis->id == pPacket->header.transitSrcId)
    {
        pThis->bTransmitting = TRUE;
        CHECK(TimeLineRelativeEvent(pThis->pTimeLine, receiveTime, pPacket));
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
    pThis->silentTime = 0;
    pThis->receiveTime = 0;
    pThis->sequenceNum = 0;
    pThis->outboxHandler.event = ePKT_EVENT_REMOVED_CLEAN;

    CHECK(RoutingClear(pThis->pRouting));
    CHECK(SchedulerReset(pThis->pScheduler));

    DUMP_INSTANCE(Station, pThis);

    CHECK(ListCleanUp(pThis->pOutbox, (ItemFilter)&StationPacketCleaner, pThis));

	return eSTATUS_COMMON_OK;
}

Boolean StationIsPacketPrioritized(Station* pThis, const Packet* pPacket)
{
    return (pPacket->header.type != ePKT_TYPE_DATA) ? TRUE : FALSE;
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

EStatus StationExport(const Station* pThis, FILE* file)
{
    return SchedulerExport(pThis->pScheduler, file);
}

EStatus StationDump(const Station* pThis)
{
    DUMP_PRINT("Station: [id=%d]", pThis->id);
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
