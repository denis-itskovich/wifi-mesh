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

/**
 * @file Simulator.c
 *
 * @date 06/12/2008
 * @author Denis Itskovich
 */

#include "../inc/Simulator.h"
#include "../inc/Station.h"
#include "../inc/TimeLine.h"
#include "../inc/Macros.h"
#include "../inc/SortedList.h"
#include "../inc/PathLoss.h"
#include <stdio.h>
#include <math.h>

int __counter_Simulator = 0;

static const double PI = 3.1415269;

struct _Simulator
{
    SortedList*     pStations;
    TimeLine*       pTimeLine;
    Settings*       pSettings;
    Statistics*     pStatistics;
    PathLoss*       pPathLoss;
    Boolean         bDupBroadcast;
    Boolean         bUpdateRequired;
    double          lastUpdateTime;
    int             activeStations;
    char*           pathLossFile;

	struct
	{
        PacketSniffer           callback;
        void*                   pArg;
	} sniffer;

    struct
    {
        StationTracker          callback;
        void*                   pArg;
	} tracker;

    struct
    {
        StationRoutingHandler   callback;
        void*                   pArg;
    } routingHandler;

    struct
    {
        StationSchedulerHandler callback;
        void*                   pArg;
    } schedulerHandler;

    struct
    {
        StationOutboxHandler    callback;
        void*                   pArg;
    } outboxHandler;
};

EStatus SimulatorAddId(Simulator* pThis, StationId id);
EStatus SimulatorInitPathLoss(Simulator* pThis);

/** Looks for a station with specified id
 * @param pThis [in] pointer to instance
 * @param id [in] station id to look for
 * @param ppStation [out] pointer to station will be stored at *ppStation
 */
EStatus SimulatorGetStation(Simulator* pThis, StationId id, Station** ppStation);

/** Dispatches outbound packets for specified station
 * @param pThis [in] pointer to instance
 * @param pStation [in] pointer to station
 */
EStatus SimulatorDispatchPacket(Simulator* pThis, Station* pStation);

/** Retrieves station list entry by id
 * @param pThis [in] pointer to instance
 * @param id [in] station id
 * @param ppEntry [out] pointer to entry will be stored at *ppEntry
 */
EStatus SimulatorGetStationEntry(Simulator* pThis, StationId id, ListEntry** ppEntry);

/** Assigns unique station id
 * @param pThis [in] pointer to instance
 */
StationId SimulatorAssignId(Simulator* pThis);

/** Invokes sniffer callback (if exists)
 * @param pThis [in] pointer to instance
 * @param pPacket [in] pointer to packet
 * @param pSrc [in] pointer to source station
 * @param pDst [in] pointer to destination station
 */
EStatus SimulatorInvokeSniffer(Simulator* pThis, const Packet* pPacket, const Station* pSrc, const Station* pDst, EPacketStatus status);

/** Invokes tracker callback (if exists)
 * @param pThis [in] pointer to instance
 * @param pStation [in] pointer to station being updated
 * @param event [in] event type /see EStationEvent
 */
EStatus SimulatorInvokeTracker(Simulator* pThis, Station* pStation, EStationEvent event);

/** Handles routing events
 * @param pStation [in] pointer to station
 * @param destId [in] destination station id
 * @param transId [in] transit station id
 * @param expirationTime [in] entry expiration time
 * @param updateAction [in] routing event
 * @param length [in] route length
 * @param retriesLeft [in] retries left for pending entry
 * @param pThis [in] pointer to instance
 */
void SimulatorRoutingHandler(const Station* pStation,
                             StationId destId,
                             StationId transId,
                             double expirationTime,
                             int length,
                             int retriesLeft,
                             ERouteEntryUpdate updateAction,
                             Simulator* pThis);

/** Handles station scheduler events
 * Is called each time a packet is scheduled/removed
 * @param pStation [in] pointer to station
 * @param time [in] time, when a packet should be issued
 * @param pPacket [in] pointer to packet instance
 * @param bAdded [in] if TRUE packet is being added, otherwise removed
 * @param pThis [in] pointer to instance
 */
void SimulatorSchedulerHandler(const Station* pStation,
                               double time,
                               const Packet* pPacket,
                               ESchedulerEvent event,
                               Simulator* pThis);

/** Handles station output box events
 * @param pStation [in] pointer to station
 * @param pPacket [in] packet being added/removed
 * @param event [in] outbox event
 * @param retriesCount [in] updated retries count
 * @param nextRetryTime [in] next retry time
 * @param pThis [in] pointer to instance
 */
void SimulatorOutboxHandler(const Station* pStation,
                            const Packet* pPacket,
                            EOutboxEvent event,
                            int retriesCount,
                            double nextRetryTime,
                            Simulator* pThis);

Boolean SimulatorCleaner(Station* pStation, Simulator* pThis)
{
	StationId id;

    SimulatorInvokeTracker(pThis, pStation, eSTATION_REMOVED);
	StationGetId(pStation, &id);
	StationDelete(&pStation);

	return FALSE;
}

Boolean SimulatorResetter(Station* pStation, Simulator* pThis)
{
	StationReset(pStation);
	SimulatorInvokeTracker(pThis, pStation, eSTATION_UPDATED);
	return TRUE;
}

Comparison SimulatorStationComparator(Station* pLeft, Station* pRight, Simulator* pThis)
{
    StationId left, right;
    StationGetId(pLeft, &left);
    StationGetId(pRight, &right);
    return (left < right) ? LESS : (left > right) ? GREAT : EQUAL;
}

EStatus SimulatorNew(Simulator** ppThis, Settings* pSettings, TimeLine* pTimeLine)
{
	CONSTRUCT(ppThis, Simulator, pSettings, pTimeLine);
}

EStatus SimulatorInit(Simulator* pThis, Settings* pSettings, TimeLine* pTimeLine)
{
	VALIDATE_ARGUMENTS(pThis && pSettings);
	CLEAR(pThis);

	CHECK(SortedListNew(&pThis->pStations, (ItemComparator)&SimulatorStationComparator, pThis));
	CHECK(StatisticsNew(&pThis->pStatistics));
	pThis->pTimeLine = pTimeLine;
	pThis->pSettings = pSettings;
	pThis->bDupBroadcast = TRUE;

	return eSTATUS_COMMON_OK;
}

EStatus SimulatorDelete(Simulator** ppThis)
{
	DESTRUCT(ppThis, Simulator);
}

EStatus SimulatorDestroy(Simulator* pThis)
{
	VALIDATE_ARGUMENTS(pThis);

	CHECK(SimulatorClear(pThis));
    CHECK(StatisticsDelete(&pThis->pStatistics));
	CHECK(SortedListDelete(&pThis->pStations));
	if (pThis->pPathLoss) CHECK(PathLossDelete(&pThis->pPathLoss));
	if (pThis->pathLossFile) free(pThis->pathLossFile);

	return eSTATUS_COMMON_OK;
}

EStatus SimulatorAddStation(Simulator* pThis, Station* pStation)
{
    StationId id;
	VALIDATE_ARGUMENTS(pThis && pStation);
	CHECK(StationGetId(pStation, &id));

	if (id == INVALID_STATION_ID) id = SimulatorAssignId(pThis);

	CHECK(StationSetId(pStation, id));
    CHECK(SortedListAdd(pThis->pStations, pStation, TRUE));
	return SimulatorInvokeTracker(pThis, pStation, eSTATION_ADDED);
}

EStatus SimulatorRemoveStation(Simulator* pThis, Station* pStation)
{
	StationId id;
	ListEntry* pEntry;

	VALIDATE_ARGUMENTS(pThis && pStation);

	CHECK(StationGetId(pStation, &id));
	CHECK(SimulatorGetStationEntry(pThis, id, &pEntry));
	SimulatorCleaner(pStation, pThis);

	return SortedListRemove(pThis->pStations, pEntry);
}

Comparison SimulatorStationFinder(const Station* pStation, const StationId* pId, void* pUserArg)
{
	StationId id;
	StationGetId(pStation, &id);

	return (id == *pId) ? EQUAL :
		   (id < *pId) ? LESS : GREAT;
}

EStatus SimulatorGetStationEntry(Simulator* pThis, StationId id, ListEntry** ppEntry)
{
	VALIDATE_ARGUMENTS(pThis && ppEntry);
	CHECK(SortedListFind(pThis->pStations, ppEntry, (ItemComparator)&SimulatorStationFinder, &id, NULL));
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorGetStation(Simulator* pThis, StationId id, Station** ppStation)
{
	ListEntry* pEntry;
	CHECK(SimulatorGetStationEntry(pThis, id, &pEntry));
	CHECK(SortedListGetValue(pEntry, ppStation));
	return eSTATUS_COMMON_OK;
}

Boolean SimulatorSynchronizer(Station* pStation, Simulator* pThis)
{
    Packet* pDeliveredPacket;
	Boolean isActive;
	StationIsActive(pStation, &isActive);
	if (isActive)
	{
		StationSynchronize(pStation, &pDeliveredPacket);
		if (!pThis->pPathLoss) StationAdvance(pStation);
		StationIsActive(pStation, &isActive);
		if (pDeliveredPacket)
        {
		    SimulatorInvokeSniffer(pThis, pDeliveredPacket, NULL, pStation, ePKT_STATUS_DELIVERED);
		    PacketDelete(&pDeliveredPacket);
        }
        if (!isActive) SimulatorInvokeTracker(pThis, pStation, eSTATION_UPDATED);
        else ++pThis->activeStations;
	}
	return TRUE;
}

Boolean SimulatorDispatcher(Station* pStation, Simulator* pThis)
{
	Boolean isActive;
	StationIsActive(pStation, &isActive);
	if (isActive)
	{
		SimulatorDispatchPacket(pThis, pStation);
	    if (pThis->bUpdateRequired) SimulatorInvokeTracker(pThis, pStation, eSTATION_UPDATED);
	}
	return TRUE;
}

EStatus SimulatorProcess(Simulator* pThis)
{
	double curTime;

	BEGIN_FUNCTION;

	VALIDATE_ARGUMENTS(pThis);

	if (pThis->activeStations < 2) return eSTATUS_TIME_LINE_FINISHED;
	pThis->activeStations = 0;

    CHECK(TimeLineGetTime(pThis->pTimeLine, &curTime));
    pThis->bUpdateRequired = (curTime - pThis->lastUpdateTime > 0.01) ? TRUE : FALSE;
    if (pThis->bUpdateRequired) pThis->lastUpdateTime = curTime;

    if (pThis->pPathLoss) CHECK(PathLossSynchronize(pThis->pPathLoss, curTime));
	CHECK(SortedListEnumerate(pThis->pStations, (ItemEnumerator)&SimulatorSynchronizer, pThis));
    CHECK(SortedListEnumerate(pThis->pStations, (ItemEnumerator)&SimulatorDispatcher, pThis));

    CHECK(TimeLineNext(pThis->pTimeLine));

    DUMP_INSTANCE(Simulator, pThis);

	END_FUNCTION;
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorInvokeSniffer(Simulator* pThis, const Packet* pPacket, const Station* pSrc, const Station* pDst, EPacketStatus status)
{
    Station* pOrigSrc;

	if (pThis->sniffer.callback)
	{
		if ((pThis->bDupBroadcast && pDst) ||
			(!pThis->bDupBroadcast && !pDst))
		{
			pThis->sniffer.callback(pPacket, pSrc, pDst, status, pThis->sniffer.pArg);
		}
	}
	if (pPacket->header.type == ePKT_TYPE_DATA &&
	    status == ePKT_STATUS_DELIVERED &&
	    pPacket->header.originalDstId == pPacket->header.transitDstId)
	{
	    CHECK(SimulatorGetStation(pThis, pPacket->header.originalSrcId, &pOrigSrc));
	    CHECK(StationPacketDelivered(pOrigSrc, pPacket));
	}
	if (status != ePKT_STATUS_PENDING) CHECK(StatisticsHandlePacket(pThis->pStatistics, pPacket, status));
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorInvokeTracker(Simulator* pThis, Station* pStation, EStationEvent event)
{
    if (pThis->tracker.callback)
    {
        pThis->tracker.callback(pStation, event, pThis->tracker.pArg);
    }

    if (event == eSTATION_ADDED)
    {
        CHECK(StationRegisterRoutingHandler(pStation, (StationRoutingHandler)&SimulatorRoutingHandler, pThis));
        CHECK(StationRegisterSchedulerHandler(pStation, (StationSchedulerHandler)&SimulatorSchedulerHandler, pThis));
        CHECK(StationRegisterOutboxHandler(pStation, (StationOutboxHandler)&SimulatorOutboxHandler, pThis));
    }

    return eSTATUS_COMMON_OK;
}

EStatus SimulatorAreAdjacent(Simulator* pThis, Station* pSrc, Station* pDst, Boolean* pAreAdjacent)
{
    StationId srcId, dstId;
    VALIDATE_ARGUMENTS(pThis && pSrc && pDst && pAreAdjacent);
    if (pThis->pPathLoss)
    {
        CHECK(StationGetId(pSrc, &srcId));
        CHECK(StationGetId(pDst, &dstId));
        return PathLossIsAdjacent(pThis->pPathLoss, srcId, dstId, pAreAdjacent);
    }
    return StationIsAdjacent(pSrc, pDst, pAreAdjacent);
}

EStatus SimulatorDispatchPacket(Simulator* pThis, Station* pStation)
{
	Packet* pPacket = NULL;
	Packet* pAbortedPacket;
	ListEntry* pEntry = NULL;
	Boolean isAdjacent = FALSE;
	Boolean isActive = FALSE;
	Station* pCurrent = NULL;
	EStatus ret;
	StationId id;
	int maxLength;

	VALIDATE_ARGUMENTS(pThis && pStation);
	CHECK(StationGetId(pStation, &id));
	CHECK(StationTransmitPacket(pStation, &pPacket));

	if (!pPacket) return eSTATUS_COMMON_OK;

    pPacket->routing.path[pPacket->routing.length++] = id;
    pPacket->header.hopsCount++;

    CHECK(SettingsGetPacketHopsThreshold(pThis->pSettings, &maxLength));
    if (pPacket->routing.length <= maxLength)
    {
        CHECK(SortedListGetHead(pThis->pStations, &pEntry));
        while (pEntry)
        {
            CHECK(SortedListGetValue(pEntry, &pCurrent));
            CHECK(StationGetId(pCurrent, &id));
            CHECK(StationIsActive(pCurrent, &isActive));
            if (isActive)
            {
                CHECK(SimulatorAreAdjacent(pThis, pStation, pCurrent, &isAdjacent));
                if (isAdjacent)
                {
                    ret = StationReceivePacket(pCurrent, pPacket, &pAbortedPacket);

                    if (pAbortedPacket)
                    {
                        CHECK(SimulatorInvokeSniffer(pThis, pAbortedPacket, pStation, pCurrent, ePKT_STATUS_COLLISION));
                        PacketDelete(&pAbortedPacket);
                    }

                    switch (ret)
                    {
                    case eSTATUS_COMMON_OK:
                        CHECK(SimulatorInvokeSniffer(pThis, pPacket, pStation, pCurrent, ePKT_STATUS_PENDING));
                        break;
                    case eSTATUS_STATION_PACKET_COLLISION:
                        CHECK(SimulatorInvokeSniffer(pThis, pPacket, pStation, pCurrent, ePKT_STATUS_PENDING));
                        CHECK(SimulatorInvokeSniffer(pThis, pPacket, pStation, pCurrent, ePKT_STATUS_COLLISION));
                        break;
                    case eSTATUS_STATION_PACKET_NOT_ACCEPTED:
                        break;
                    default:
                        return ret;
                    }

                }
                else
                {
                    if (pPacket->header.transitDstId == id)
                        CHECK(SimulatorInvokeSniffer(pThis, pPacket, pStation, pCurrent, ePKT_STATUS_OUT_OF_RANGE));
                }
            }

            CHECK(SortedListGetNext(&pEntry));
        }
    }
    else
    {
        CHECK(SimulatorInvokeSniffer(pThis, pPacket, pStation, NULL, ePKT_STATUS_HOPS_LIMIT_REACHED));
    }

	CHECK(PacketDelete(&pPacket));
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorSetPacketSniffer(Simulator* pThis, PacketSniffer sniffer, void* pUserArg)
{
	VALIDATE_ARGUMENTS(pThis);
	pThis->sniffer.callback = sniffer;
	pThis->sniffer.pArg = pUserArg;
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorSetStationTracker(Simulator* pThis, StationTracker tracker, void* pUserArg)
{
	VALIDATE_ARGUMENTS(pThis);
	pThis->tracker.callback = tracker;
	pThis->tracker.pArg = pUserArg;
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorSetRoutingHandler(Simulator* pThis, StationRoutingHandler handler, void* pUserArg)
{
    VALIDATE_ARGUMENTS(pThis);
    pThis->routingHandler.callback = handler;
    pThis->routingHandler.pArg = pUserArg;
    return eSTATUS_COMMON_OK;
}

EStatus SimulatorSetSchedulerHandler(Simulator* pThis, StationSchedulerHandler handler, void* pUserArg)
{
    VALIDATE_ARGUMENTS(pThis);
    pThis->schedulerHandler.callback = handler;
    pThis->schedulerHandler.pArg = pUserArg;
    return eSTATUS_COMMON_OK;
}

EStatus SimulatorSetOutboxHandler(Simulator* pThis, StationOutboxHandler handler, void* pUserArg)
{
    VALIDATE_ARGUMENTS(pThis);
    pThis->outboxHandler.callback = handler;
    pThis->outboxHandler.pArg = pUserArg;
    return eSTATUS_COMMON_OK;
}

EStatus SimulatorReset(Simulator* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
    CHECK(TimeLineClear(pThis->pTimeLine));
    CHECK(SortedListEnumerate(pThis->pStations, (ItemEnumerator)&SimulatorResetter, pThis));
    CHECK(StatisticsReset(pThis->pStatistics));
    CHECK(SimulatorInitPathLoss(pThis));
	pThis->lastUpdateTime = 0;
	pThis->activeStations = 2;
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorClear(Simulator* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	CHECK(SortedListCleanUp(pThis->pStations, (ItemFilter)&SimulatorCleaner, pThis));
	CHECK(TimeLineClear(pThis->pTimeLine));
    CHECK(StatisticsClear(pThis->pStatistics));
    pThis->lastUpdateTime = 0;
    pThis->activeStations = -1;
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorEnumerateStations(Simulator* pThis, StationsEnumerator enumerator, void* pUserArg)
{
	VALIDATE_ARGUMENTS(pThis && enumerator);
	return SortedListEnumerate(pThis->pStations, (ItemEnumerator)enumerator, pUserArg);
}

EStatus SimulatorAddId(Simulator* pThis, StationId id)
{
    return eSTATUS_COMMON_OK;
}

Boolean SimulatorIdFinder(Station* pStation, StationId* pId)
{
    StationId id;
    StationGetId(pStation, &id);
    if (id != *pId) return FALSE;
    ++*pId;
    return TRUE;
}

StationId SimulatorAssignId(Simulator* pThis)
{
    StationId id = 0;
    SortedListEnumerate(pThis->pStations, (ItemEnumerator)&SimulatorIdFinder, &id);
    return id;
}

EStatus SimulatorSetSniffingMode(Simulator* pThis, Boolean bSingle)
{
	SET_MEMBER(bSingle, pThis, bDupBroadcast);
}

EStatus SimulatorGetSniffingMode(Simulator* pThis, Boolean* pSingle)
{
	GET_MEMBER(pSingle, pThis, bDupBroadcast);
}

EStatus SimulatorGetStatistics(const Simulator* pThis, const Statistics** ppStatistics)
{
    VALIDATE_ARGUMENTS(pThis && ppStatistics);
    *ppStatistics = (const Statistics*)pThis->pStatistics;
    return eSTATUS_COMMON_OK;
}

void SimulatorRoutingHandler(const Station* pStation,
                             StationId destId,
                             StationId transId,
                             double expirationTime,
                             int length,
                             int retriesLeft,
                             ERouteEntryUpdate updateAction,
                             Simulator* pThis)
{
    if (pThis->routingHandler.callback)
    {
        pThis->routingHandler.callback(pStation,
                                       destId,
                                       transId,
                                       expirationTime,
                                       length,
                                       retriesLeft,
                                       updateAction,
                                       pThis->routingHandler.pArg);
    }
}

void SimulatorSchedulerHandler(const Station* pStation,
                               double time,
                               const Packet* pPacket,
                               ESchedulerEvent event,
                               Simulator* pThis)
{
    if (pThis->schedulerHandler.callback)
    {
        pThis->schedulerHandler.callback(pStation, time, pPacket, event, pThis->schedulerHandler.pArg);
    }
    StatisticsHandleSchedulerEvent(pThis->pStatistics, pPacket, event);
}

void SimulatorOutboxHandler(const Station* pStation,
                            const Packet* pPacket,
                            EOutboxEvent event,
                            int retriesCount,
                            double nextRetryTime,
                            Simulator* pThis)
{
    if (pThis->outboxHandler.callback)
    {
        pThis->outboxHandler.callback(pStation, pPacket, event, retriesCount, nextRetryTime, pThis->outboxHandler.pArg);
    }
}

Boolean SimulatorDumpStation(const Station* pStation, void* pArg)
{
    DUMP_INSTANCE(Station, pStation);
    return TRUE;
}

EStatus SimulatorDump(const Simulator* pThis)
{
    DUMP_PRINT("Simulator:");
    CHECK(SortedListEnumerate(pThis->pStations, (ItemEnumerator)&SimulatorDumpStation, NULL));
    return eSTATUS_COMMON_OK;
}

EStatus SimulatorImport(Simulator* pThis, const char* filename)
{
    FILE* file;

    double  attenuationConst;
    double  maxAttenuation;
    Size    worldSize;
    int     stationsCount;
    int     dataRate;
    int     bufferSize;
    int     routingTableSize;
    double  routeExpirationTimeout;
    int     routeRetryThreshold;
    double  routeRetryTimeout;
    int     packetHopsThreshold;
    int     packetRetryThreshold;
    double  packetRetryTimeout;
    double  simulationTimeout;

    int i;
    double angle, vel;
    Location l;
    Velocity v;
    Station* pStation;

    int id, src, dst;
    int pktId;
    double time;
    int size;
    Packet* pPacket;

    VALIDATE_ARGUMENTS(pThis && filename);

    file = fopen(filename, "r");
    VALIDATE(file, eSTATUS_SIMULATOR_FILE_OPEN_FAILURE);
    VALIDATE(fscanf(file, "%lf %lf %d", &worldSize.x, &worldSize.y, &stationsCount) == 3, eSTATUS_SIMULATOR_FILE_CORRUPTED);
    VALIDATE(fscanf(file, "%lf %lf %d %d", &maxAttenuation, &attenuationConst, &dataRate, &bufferSize) == 4, eSTATUS_SIMULATOR_FILE_CORRUPTED);
    VALIDATE(fscanf(file, "%d %lf %d %lf", &routingTableSize, &routeExpirationTimeout, &routeRetryThreshold, &routeRetryTimeout) == 4, eSTATUS_SIMULATOR_FILE_CORRUPTED);
    VALIDATE(fscanf(file, "%d %d %lf %lf", &packetHopsThreshold, &packetRetryThreshold, &packetRetryTimeout, &simulationTimeout) == 4, eSTATUS_SIMULATOR_FILE_CORRUPTED);

    CHECK(SettingsSetMaxAttenuation(pThis->pSettings, maxAttenuation));
    CHECK(SettingsSetAttenuationCoefficient(pThis->pSettings, attenuationConst));
    CHECK(SettingsSetDataRate(pThis->pSettings, dataRate * 1024));
    CHECK(SettingsSetWorldSize(pThis->pSettings, worldSize));
    CHECK(SettingsSetMaxDuration(pThis->pSettings, simulationTimeout));
    CHECK(SettingsSetRelayBufferSize(pThis->pSettings, bufferSize));
    CHECK(SettingsSetRoutingTableSize(pThis->pSettings, routingTableSize));
    CHECK(SettingsSetRouteExpirationTimeout(pThis->pSettings, routeExpirationTimeout));
    CHECK(SettingsSetRouteRetryThreshold(pThis->pSettings, routeRetryThreshold));
    CHECK(SettingsSetRouteRetryTimeout(pThis->pSettings, routeRetryTimeout));
    CHECK(SettingsSetPacketHopsThreshold(pThis->pSettings, packetHopsThreshold));
    CHECK(SettingsSetPacketRetryThreshold(pThis->pSettings, packetRetryThreshold));
    CHECK(SettingsSetPacketRetryTimeout(pThis->pSettings, packetRetryTimeout));

    for (i = 0; i < stationsCount; ++i)
    {
        VALIDATE(fscanf(file, "%d %lf %lf %lf %lf", &id, &l.x, &l.y, &angle, &vel) == 5, eSTATUS_SIMULATOR_FILE_CORRUPTED);
        angle = angle / 180 * PI;
        l.x -= worldSize.x / 2.0;
        l.y -= worldSize.y / 2.0;
        v.x = vel * cos(angle);
        v.y = -vel * sin(angle);
        CHECK(StationNew(&pStation, v, l, pThis->pTimeLine, pThis->pSettings));
        CHECK(StationSetId(pStation, id));
        CHECK(SimulatorAddStation(pThis, pStation));
    }

    while (!feof(file))
    {
        if (fscanf(file, "%d %d %d %lf %d", &pktId, &src, &dst, &time, &size) < 5) break;

        CHECK(PacketNewData(&pPacket, (StationId)src, (StationId)dst, (unsigned long)size, pktId));
        CHECK(SimulatorGetStation(pThis, src, &pStation));
        CHECK(StationSchedulePacket(pStation, pPacket, time));
    }

    fclose(file);

    return eSTATUS_COMMON_OK;
}

EStatus SimulatorExport(Simulator* pThis, const char* filename)
{
    FILE* file;

    double          attenuationConst = 0;
    double          maxAttenuation = 0;
    Size            worldSize = {0};
    int             stationsCount = 0;
    unsigned long   dataRate = 0;
    int             bufferSize = 0;
    int             routingTableSize = 0;
    double          routeExpirationTimeout = 0;
    int             routeRetryThreshold = 0;
    double          routeRetryTimeout = 0;
    int             packetHopsThreshold = 0;
    int             packetRetryThreshold = 0;
    double          packetRetryTimeout = 0;
    double          simulationTimeout = 0;

    double angle = 0, vel = 0;
    Location l = {0};
    Velocity v = {0};
    Station* pStation = NULL;
    StationId id;

    ListEntry* pEntry;

    VALIDATE_ARGUMENTS(pThis && filename);

    file = fopen(filename, "w");
    VALIDATE(file, eSTATUS_SIMULATOR_FILE_OPEN_FAILURE);

    CHECK(SettingsGetMaxAttenuation(pThis->pSettings, &maxAttenuation));
    CHECK(SettingsGetAttenuationCoefficient(pThis->pSettings, &attenuationConst));
    CHECK(SettingsGetDataRate(pThis->pSettings, &dataRate));
    CHECK(SettingsGetWorldSize(pThis->pSettings, &worldSize));
    CHECK(SettingsGetMaxDuration(pThis->pSettings, &simulationTimeout));
    CHECK(SettingsGetRelayBufferSize(pThis->pSettings, &bufferSize));
    CHECK(SettingsGetRoutingTableSize(pThis->pSettings, &routingTableSize));
    CHECK(SettingsGetRouteExpirationTimeout(pThis->pSettings, &routeExpirationTimeout));
    CHECK(SettingsGetRouteRetryThreshold(pThis->pSettings, &routeRetryThreshold));
    CHECK(SettingsGetRouteRetryTimeout(pThis->pSettings, &routeRetryTimeout));
    CHECK(SettingsGetPacketHopsThreshold(pThis->pSettings, &packetHopsThreshold));
    CHECK(SettingsGetPacketRetryThreshold(pThis->pSettings, &packetRetryThreshold));
    CHECK(SettingsGetPacketRetryTimeout(pThis->pSettings, &packetRetryTimeout));

    CHECK(SortedListGetCount(pThis->pStations, &stationsCount));

    fprintf(file, "%lf %lf %d\n", worldSize.x, worldSize.y, stationsCount);
    fprintf(file, "%lf %lf %d %d\n", maxAttenuation, attenuationConst, (int)dataRate / 1024, bufferSize);
    fprintf(file, "%d %lf %d %lf", routingTableSize, routeExpirationTimeout, routeRetryThreshold, routeRetryTimeout);
    fprintf(file, "%d %d %lf %lf", packetHopsThreshold, packetRetryThreshold, packetRetryTimeout, simulationTimeout);

    fprintf(file, "\n\n");

    CHECK(SortedListGetHead(pThis->pStations, &pEntry));
    while (pEntry)
    {
        CHECK(SortedListGetValue(pEntry, &pStation));
        CHECK(StationGetLocation(pStation, &l));
        CHECK(StationGetVelocity(pStation, &v));
        CHECK(StationGetId(pStation, &id));

        vel = sqrt(pow(v.x, 2) + pow(v.y, 2));

        if (vel != 0)
        {
            angle = acos(v.x/vel);
            angle = angle / PI * 180;
            if (v.y > 0) angle = 360-angle;
        }
        else angle = 0;

        l.x += worldSize.x / 2;
        l.y += worldSize.y / 2;
        fprintf(file, "%d %d %d %lf %lf\n", (int)id, (int)l.x, (int)l.y, angle, vel);
        CHECK(SortedListGetNext(&pEntry));
    }

    fprintf(file, "\n\n");

    CHECK(SortedListGetHead(pThis->pStations, &pEntry));
    while (pEntry)
    {
        CHECK(SortedListGetValue(pEntry, &pStation));
        CHECK(StationExport(pStation, file));
        CHECK(SortedListGetNext(&pEntry));
    }

    fclose(file);

    return eSTATUS_COMMON_OK;
}

StationId SimulatorGetMaxId(Simulator* pThis)
{
    ListEntry* pEntry;
    Station* pStation;
    StationId id;

    SortedListGetTail(pThis->pStations, &pEntry);
    if (!pEntry) return 0;

    SortedListGetValue(pEntry, &pStation);
    StationGetId(pStation, &id);

    return id + 1;
}

EStatus SimulatorInitPathLoss(Simulator* pThis)
{
    double maxAttenuation;
    StationId maxId;

    VALIDATE_ARGUMENTS(pThis);

    if (pThis->pPathLoss) CHECK(PathLossDelete(&pThis->pPathLoss));
    if (!pThis->pathLossFile) return eSTATUS_COMMON_OK;

    maxId = SimulatorGetMaxId(pThis);
    CHECK(SettingsGetMaxAttenuation(pThis->pSettings, &maxAttenuation));
    EStatus ret = PathLossNew(&pThis->pPathLoss, maxId, maxAttenuation, pThis->pathLossFile);

    if (ret != eSTATUS_COMMON_OK)
    {
        CHECK(PathLossDelete(&pThis->pPathLoss));
        return ret;
    }

    return PathLossReset(pThis->pPathLoss);
}

EStatus SimulatorSetPathLoss(Simulator* pThis, const char* filename)
{
    VALIDATE_ARGUMENTS(pThis);

    if (pThis->pathLossFile) free(pThis->pathLossFile);
    pThis->pathLossFile = (filename) ? strdup(filename) : NULL;

    return eSTATUS_COMMON_OK;
}
