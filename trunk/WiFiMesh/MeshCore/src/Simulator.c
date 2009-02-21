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

#include "Simulator.h"
#include "Station.h"
#include "TimeLine.h"
#include "Macros.h"
#include "List.h"
#include <stdio.h>
#include <math.h>

static const double PI = 3.1415269;

struct _Simulator
{
	List*          pStations;
	TimeLine*      pTimeLine;
	Settings*      pSettings;
    Statistics*    pStatistics;
	StationId      minId;
	StationId      maxId;
	Boolean        bDupBroadcast;
	Boolean        bUpdateRequired;
	double         lastUpdateTime;
	double         timeDelta;
	int            activeStations;

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

/** Frees station id
 * @param pThis [in] pointer to instance
 * @param id [in] id to free
 */
void SimulatorFreeId(Simulator* pThis, StationId id);

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
 * @param pThis [in] pointer to instance
 */
void SimulatorRoutingHandler(const Station* pStation,
                             StationId destId,
                             StationId transId,
                             double expirationTime,
                             int length,
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
	SimulatorFreeId(pThis, id);

	return FALSE;
}

Boolean SimulatorResetter(Station* pStation, Simulator* pThis)
{
	StationReset(pStation);
	SimulatorInvokeTracker(pThis, pStation, eSTATION_UPDATED);
	return TRUE;
}

EStatus SimulatorNew(Simulator** ppThis, Settings* pSettings, TimeLine* pTimeLine)
{
	CONSTRUCT(ppThis, Simulator, pSettings, pTimeLine);
}

EStatus SimulatorInit(Simulator* pThis, Settings* pSettings, TimeLine* pTimeLine)
{
	VALIDATE_ARGUMENTS(pThis && pSettings);
	CLEAR(pThis);

	CHECK(ListNew(&pThis->pStations));
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

    CHECK(StatisticsDelete(&pThis->pStatistics));
	CHECK(ListDelete(&pThis->pStations));

	return eSTATUS_COMMON_OK;
}

EStatus SimulatorAddStation(Simulator* pThis, Station* pStation)
{
	VALIDATE_ARGUMENTS(pThis && pStation);
	CHECK(StationSetId(pStation, SimulatorAssignId(pThis)));
	SimulatorInvokeTracker(pThis, pStation, eSTATION_ADDED);
	return ListPushBack(pThis->pStations, pStation);
}

EStatus SimulatorRemoveStation(Simulator* pThis, Station* pStation)
{
	StationId id;
	ListEntry* pEntry;

	VALIDATE_ARGUMENTS(pThis && pStation);

	CHECK(StationGetId(pStation, &id));
	CHECK(SimulatorGetStationEntry(pThis, id, &pEntry));
	SimulatorCleaner(pStation, pThis);

	return ListRemove(pThis->pStations, pEntry);
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
	CHECK(ListFind(pThis->pStations, ppEntry, (ItemComparator)&SimulatorStationFinder, &id, NULL));
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorGetStation(Simulator* pThis, StationId id, Station** ppStation)
{
	ListEntry* pEntry;
	CHECK(SimulatorGetStationEntry(pThis, id, &pEntry));
	CHECK(ListGetValue(pEntry, ppStation));
	return eSTATUS_COMMON_OK;
}

Boolean SimulatorSynchronizer(Station* pStation, Simulator* pThis)
{
    Packet* pDeliveredPacket;
	Boolean isActive;
	StationIsActive(pStation, &isActive);
	if (isActive)
	{
		StationSynchronize(pStation, pThis->timeDelta, &pDeliveredPacket);
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
	double oldTime;
	double curTime;

	BEGIN_FUNCTION;

	VALIDATE_ARGUMENTS(pThis);

	if (!pThis->activeStations) return eSTATUS_TIME_LINE_FINISHED;
	pThis->activeStations = 0;

    CHECK(TimeLineGetTime(pThis->pTimeLine, &oldTime));
    CHECK(TimeLineNext(pThis->pTimeLine));
    CHECK(TimeLineGetTime(pThis->pTimeLine, &curTime));
    pThis->bUpdateRequired = (curTime - pThis->lastUpdateTime > 0.01) ? TRUE : FALSE;
    if (pThis->bUpdateRequired) pThis->lastUpdateTime = curTime;

    pThis->timeDelta = curTime - oldTime;

    INFO_PRINT("Performing simulation step: [time delta: %.2f]", pThis->timeDelta);

	CHECK(ListEnumerate(pThis->pStations, (ItemEnumerator)&SimulatorSynchronizer, pThis));
    CHECK(ListEnumerate(pThis->pStations, (ItemEnumerator)&SimulatorDispatcher, pThis));

    DUMP_INSTANCE(Simulator, pThis);

	END_FUNCTION;
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorInvokeSniffer(Simulator* pThis, const Packet* pPacket, const Station* pSrc, const Station* pDst, EPacketStatus status)
{
	if (pThis->sniffer.callback)
	{
		if ((pThis->bDupBroadcast && pDst) ||
			(!pThis->bDupBroadcast && !pDst))
		{
			pThis->sniffer.callback(pPacket, pSrc, pDst, status, pThis->sniffer.pArg);
		}
	}
	return StatisticsHandlePacket(pThis->pStatistics, pPacket, status);
}

EStatus SimulatorInvokeTracker(Simulator* pThis, Station* pStation, EStationEvent event)
{
    if (pThis->tracker.callback)
    {
        pThis->tracker.callback(pStation, event, pThis->tracker.pArg);
    }

    switch (event)
    {
    case eSTATION_ADDED:
        CHECK(StationRegisterRoutingHandler(pStation, (StationRoutingHandler)&SimulatorRoutingHandler, pThis));
        CHECK(StationRegisterSchedulerHandler(pStation, (StationSchedulerHandler)&SimulatorSchedulerHandler, pThis));
        CHECK(StationRegisterOutboxHandler(pStation, (StationOutboxHandler)&SimulatorOutboxHandler, pThis));
        break;
    case eSTATION_REMOVED:
        CHECK(StationRegisterRoutingHandler(pStation, NULL, NULL));
        CHECK(StationRegisterSchedulerHandler(pStation, NULL, NULL));
        CHECK(StationRegisterOutboxHandler(pStation, NULL, NULL));
        break;
    default: break;
    }

    return eSTATUS_COMMON_OK;
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

	VALIDATE_ARGUMENTS(pThis && pStation);
	CHECK(StationGetId(pStation, &id));
	CHECK(StationGetPacket(pStation, &pPacket));

	if (!pPacket) return eSTATUS_COMMON_OK;
	pPacket->routing.path[pPacket->routing.length++] = id;
    pPacket->header.hopsCount++;

	CHECK(ListGetHead(pThis->pStations, &pEntry));
	while (pEntry)
	{
        CHECK(ListGetValue(pEntry, &pCurrent));
        CHECK(StationGetId(pCurrent, &id));
	    CHECK(StationIsActive(pCurrent, &isActive));
	    if (isActive)
	    {
	        CHECK(StationIsAdjacent(pStation, pCurrent, &isAdjacent));
	        if (isAdjacent)
	        {
	            ret = StationPutPacket(pCurrent, pPacket, &pAbortedPacket);
	            switch (ret)
	            {
	            case eSTATUS_COMMON_OK:
	                CHECK(SimulatorInvokeSniffer(pThis, pPacket, pStation, pCurrent, ePKT_STATUS_PENDING));
	                break;
                case eSTATUS_STATION_PACKET_COLLISION:
                    if (pAbortedPacket)
                    {
                        CHECK(SimulatorInvokeSniffer(pThis, pAbortedPacket, pStation, pCurrent, ePKT_STATUS_COLLISION));
                        PacketDelete(&pAbortedPacket);
                    }
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
	            if (pPacket->header.transitDstId == id) SimulatorInvokeSniffer(pThis, pPacket, pStation, pCurrent, ePKT_STATUS_OUT_OF_RANGE);
	        }
	    }

		CHECK(ListGetNext(&pEntry));
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
    CHECK(ListEnumerate(pThis->pStations, (ItemEnumerator)&SimulatorResetter, pThis));
    CHECK(StatisticsReset(pThis->pStatistics));
	pThis->lastUpdateTime = 0;
	pThis->activeStations = -1;
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorClear(Simulator* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	CHECK(ListCleanUp(pThis->pStations, (ItemFilter)&SimulatorCleaner, pThis));
	CHECK(TimeLineClear(pThis->pTimeLine));
    CHECK(StatisticsClear(pThis->pStatistics));
    pThis->lastUpdateTime = 0;
    pThis->activeStations = -1;
	return eSTATUS_COMMON_OK;
}

EStatus SimulatorEnumerateStations(Simulator* pThis, StationsEnumerator enumerator, void* pUserArg)
{
	VALIDATE_ARGUMENTS(pThis && enumerator);
	return ListEnumerate(pThis->pStations, (ItemEnumerator)enumerator, pUserArg);
}

StationId SimulatorAssignId(Simulator* pThis)
{
	if (pThis->minId > 0) return --pThis->minId;
	return ++pThis->maxId;
}

void SimulatorFreeId(Simulator* pThis, StationId id)
{
	int count = 0;
	if (pThis->minId == id) ++pThis->minId;
	if (pThis->maxId == id) --pThis->maxId;
	ListGetCount(pThis->pStations, &count);
	if (count == 1) pThis->minId = pThis->maxId = 0;
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
                             ERouteEntryUpdate updateAction,
                             Simulator* pThis)
{
    if (pThis->routingHandler.callback)
    {
        pThis->routingHandler.callback(pStation, destId, transId, expirationTime, length, updateAction, pThis->routingHandler.pArg);
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
    CHECK(ListEnumerate(pThis->pStations, (ItemEnumerator)&SimulatorDumpStation, NULL));
    return eSTATUS_COMMON_OK;
}

EStatus SimulatorImport(Simulator* pThis, const char* filename)
{
    FILE* file;

    int routingProtocol;
    int dataLinkProtocol;
    int bufferRouting;
    double attenuationConst;
    double maxAttenuation;
    Size worldSize;
    int stationsCount;
    int dataRate;
    int bufferSize;
    int ctrlMessagesPriority;
    double crcProbability;
    double timeUntilReply;
    int routingTableSize;
    double routeEntryTimeout;
    int searchRetries;
    int routingTablePolicy;
    int maxEntriesToSameHost;
    double beaconSendInterval;
    double neighborsListSendTime;
    double tdmaSlotSize;
    int tdmaSlotsNumber;
    int connectionAttempts;
    double simulationSamplingRate;
    double simulationTimeout;

    int i;
    double angle, vel;
    Location l;
    Velocity v;
    Station* pStation;

    int src, dst;
    int pktId;
    double time;
    int size;
    int hopsCount;
    Packet* pPacket;

    VALIDATE_ARGUMENTS(pThis && filename);

    file = fopen(filename, "r");
    VALIDATE(fscanf(file, "%d %d %d", &routingProtocol, &dataLinkProtocol, &bufferRouting) == 3, eSTATUS_SIMULATOR_FILE_CORRUPTED);
    VALIDATE(fscanf(file, "%lf %lf %d %lf %lf", &worldSize.x, &worldSize.y, &stationsCount, &maxAttenuation, &attenuationConst) == 5, eSTATUS_SIMULATOR_FILE_CORRUPTED);
    VALIDATE(fscanf(file, "%d %d %d %lf", &dataRate, &bufferSize, &ctrlMessagesPriority, &crcProbability) == 4, eSTATUS_SIMULATOR_FILE_CORRUPTED);
    VALIDATE(fscanf(file, "%lf %d %lf", &timeUntilReply, &routingTableSize, &routeEntryTimeout) == 3, eSTATUS_SIMULATOR_FILE_CORRUPTED);
    VALIDATE(fscanf(file, "%d %d %d", &searchRetries, &routingTablePolicy, &maxEntriesToSameHost) == 3, eSTATUS_SIMULATOR_FILE_CORRUPTED);
    VALIDATE(fscanf(file, "%lf %lf %lf %d", &beaconSendInterval, &neighborsListSendTime, &tdmaSlotSize, &tdmaSlotsNumber) == 4, eSTATUS_SIMULATOR_FILE_CORRUPTED);
    VALIDATE(fscanf(file, "%d %lf %lf", &connectionAttempts, &simulationSamplingRate, &simulationTimeout) == 3, eSTATUS_SIMULATOR_FILE_CORRUPTED);

    CHECK(SettingsSetDataRate(pThis->pSettings, dataRate * 1024));
    CHECK(SettingsSetWorldSize(pThis->pSettings, worldSize));
    CHECK(SettingsSetCoverage(pThis->pSettings, sqrt(maxAttenuation/attenuationConst)));
    CHECK(SettingsSetPacketRetryTimeout(pThis->pSettings, timeUntilReply));
    CHECK(SettingsSetRouteExpirationTimeout(pThis->pSettings, routeEntryTimeout));

    for (i = 0; i < stationsCount; ++i)
    {
        VALIDATE(fscanf(file, "%lf %lf %lf %lf", &l.x, &l.y, &angle, &vel) == 4, eSTATUS_SIMULATOR_FILE_CORRUPTED);
        angle = angle / 180 * PI;
        l.x -= worldSize.x / 2.0;
        l.y -= worldSize.y / 2.0;
        v.x = vel * cos(angle);
        v.y = -vel * sin(angle);
        CHECK(StationNew(&pStation, v, l, pThis->pTimeLine, pThis->pSettings));
        CHECK(SimulatorAddStation(pThis, pStation));
    }

    while (!feof(file))
    {
        if (fscanf(file, "%d %d %d %lf %d %d", &pktId, &src, &dst, &time, &size, &hopsCount) < 6) break;
        ++src, ++dst;

        CHECK(PacketNewData(&pPacket, (StationId)src, (StationId)dst, (unsigned long)size, pktId));
        pPacket->header.timeToLive = hopsCount;

        CHECK(SimulatorGetStation(pThis, src, &pStation));
        CHECK(StationSchedulePacket(pStation, pPacket, time));
    }

    fclose(file);

    return eSTATUS_COMMON_OK;
}

EStatus SimulatorExport(Simulator* pThis, const char* filename)
{
    FILE* file;

    int routingProtocol = 0;
    int dataLinkProtocol = 0;
    int bufferRouting = 0;
    double attenuationConst = 0;
    double maxAttenuation = 0;
    Size worldSize = {0};
    int stationsCount = 0;
    unsigned long dataRate = 0;
    int bufferSize = 0;
    int ctrlMessagesPriority = 0;
    double crcProbability = 0;
    double timeUntilReply = 0;
    int routingTableSize = 0;
    double routeEntryTimeout = 0;
    int searchRetries = 0;
    int routingTablePolicy = 0;
    int maxEntriesToSameHost = 0;
    double beaconSendInterval = 0;
    double neighborsListSendTime = 0;
    double tdmaSlotSize = 0;
    int tdmaSlotsNumber = 0;
    int connectionAttempts = 0;
    double simulationSamplingRate = 0;
    double simulationTimeout = 0;
    double coverage = 0;

//    int i = 0;
    double angle = 0, vel = 0;
    Location l = {0};
    Velocity v = {0};
    Station* pStation = NULL;

//    StationId src = 0, dst = 0;
//    int pktId = 0;
//    double time = 0;
//    int size = 0;
//    int hopsCount = 0;
//    Packet* pPacket = NULL;
    ListEntry* pEntry;

    VALIDATE_ARGUMENTS(pThis && filename);

    file = fopen(filename, "w");

    CHECK(SettingsGetDataRate(pThis->pSettings, &dataRate));
    CHECK(SettingsGetWorldSize(pThis->pSettings, &worldSize));
    CHECK(SettingsGetCoverage(pThis->pSettings, &coverage));
    CHECK(SettingsGetPacketRetryTimeout(pThis->pSettings, &timeUntilReply));
    CHECK(SettingsGetRouteExpirationTimeout(pThis->pSettings, &routeEntryTimeout));
    CHECK(ListGetCount(pThis->pStations, &stationsCount));

    attenuationConst = 1;
    maxAttenuation = pow(coverage, 2);

    fprintf(file, "%d %d %d\n", routingProtocol, dataLinkProtocol, bufferRouting);
    fprintf(file, "%d %d %d %lf %lf\n", (int)worldSize.x, (int)worldSize.y, stationsCount, maxAttenuation, attenuationConst);
    fprintf(file, "%d %d %d %lf\n", (int)dataRate / 1024, bufferSize, ctrlMessagesPriority, crcProbability);
    fprintf(file, "%lf %d %lf ", timeUntilReply, routingTableSize, routeEntryTimeout);
    fprintf(file, "%d %d %d\n", searchRetries, routingTablePolicy, maxEntriesToSameHost);
    fprintf(file, "%lf %lf %lf %d\n", beaconSendInterval, neighborsListSendTime, tdmaSlotSize, tdmaSlotsNumber);
    fprintf(file, "%d %lf %lf\n", connectionAttempts, simulationSamplingRate, simulationTimeout);

    fprintf(file, "\n\n");

    CHECK(ListGetHead(pThis->pStations, &pEntry));
    while (pEntry)
    {
        CHECK(ListGetValue(pEntry, &pStation));
        CHECK(StationGetLocation(pStation, &l));
        CHECK(StationGetVelocity(pStation, &v));

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
        fprintf(file, "%d %d %lf %lf\n", (int)l.x, (int)l.y, angle, vel);
        CHECK(ListGetNext(&pEntry));
    }

    fprintf(file, "\n\n");

    CHECK(ListGetHead(pThis->pStations, &pEntry));
    while (pEntry)
    {
        CHECK(ListGetValue(pEntry, &pStation));
        CHECK(StationExport(pStation, file));
        CHECK(ListGetNext(&pEntry));
    }

    fclose(file);

    return eSTATUS_COMMON_OK;
}
