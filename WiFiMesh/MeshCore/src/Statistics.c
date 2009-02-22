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
 * TODO Fill file purpose and description
 * @file Statistics.c
 * @date 14/02/2009
 * @author Denis Itskovich
 */

#include "Macros.h"
#include "Statistics.h"

EStatus StatisticsNew(Statistics** ppThis)
{
    CONSTRUCT(ppThis, Statistics);
}

EStatus StatisticsDelete(Statistics** ppThis)
{
    DESTRUCT(ppThis, Statistics);
}

EStatus StatisticsInit(Statistics* pThis)
{
    VALIDATE_ARGUMENTS(pThis);
    return StatisticsClear(pThis);
}

EStatus StatisticsReset(Statistics* pThis)
{
    VALIDATE_ARGUMENTS(pThis);
    int scheduledSize = pThis->sizeByTraffic[eTRAFFIC_SCHEDULED];
    int scheduledPackets = pThis->packetsByTraffic[eTRAFFIC_SCHEDULED];
    CHECK(StatisticsClear(pThis));
    pThis->sizeByTraffic[eTRAFFIC_SCHEDULED] = scheduledSize;
    pThis->packetsByTraffic[eTRAFFIC_SCHEDULED] = scheduledPackets;
    return eSTATUS_COMMON_OK;
}

EStatus StatisticsClear(Statistics* pThis)
{
    VALIDATE_ARGUMENTS(pThis);
    CLEAR(pThis);
    return eSTATUS_COMMON_OK;
}

EStatus StatisticsDestroy(Statistics* pThis)
{
    VALIDATE_ARGUMENTS(pThis);
    return eSTATUS_COMMON_OK;
}

EStatus StatisticsHandleSchedulerEvent(Statistics* pThis, const Packet* pPacket, ESchedulerEvent event)
{
    VALIDATE_ARGUMENTS(pThis && pPacket);

    unsigned long size = pPacket->payload.size;

    switch (event)
    {
    case eSCHEDULE_ADDED:
        ++pThis->packetsByTraffic[eTRAFFIC_SCHEDULED];
        pThis->sizeByTraffic[eTRAFFIC_SCHEDULED] += size;
        break;
    case eSCHEDULE_REMOVED:
        --pThis->packetsByTraffic[eTRAFFIC_SCHEDULED];
        pThis->sizeByTraffic[eTRAFFIC_SCHEDULED] -= size;
        break;
    case eSCHEDULE_RESET:
        pThis->packetsByTraffic[eTRAFFIC_ISSUED] = 0;
        pThis->packetsByTraffic[eTRAFFIC_DELIVERED] = 0;
        pThis->sizeByTraffic[eTRAFFIC_ISSUED] = 0;
        pThis->sizeByTraffic[eTRAFFIC_DELIVERED] = 0;
        break;
    case eSCHEDULE_ISSUED:
        ++pThis->packetsByTraffic[eTRAFFIC_ISSUED];
        pThis->sizeByTraffic[eTRAFFIC_ISSUED] += size;
        break;
    case eSCHEDULE_DELIVERED:
        ++pThis->packetsByTraffic[eTRAFFIC_DELIVERED];
        pThis->sizeByTraffic[eTRAFFIC_DELIVERED] += size;
        break;
    }

    return eSTATUS_COMMON_OK;
}

EStatus StatisticsHandlePacket(Statistics* pThis, const Packet* pPacket, EPacketStatus status)
{
    EPacketType type;
    unsigned size;
    double totalHopsCount;
    double totalRouteLength;
    static const Packet* pLastPacket = NULL;

    VALIDATE_ARGUMENTS(pThis && pPacket);

    if (pPacket == pLastPacket) return eSTATUS_COMMON_OK;
    pLastPacket = pPacket;

    type = pPacket->header.type;
    ++pThis->packetsByStatus[status];
    if (status != ePKT_STATUS_DELIVERED) return eSTATUS_COMMON_OK;

    CHECK(PacketGetSize(pPacket, &size));
    pThis->sizeByType[type] += size;
    ++pThis->packetsByType[type];

    totalHopsCount = pThis->avgHopsCount * (double)(pThis->packetsByStatus[status] - 1) + (double)pPacket->header.hopsCount;
    pThis->avgHopsCount = totalHopsCount / (double)pThis->packetsByStatus[status];

    totalRouteLength = pThis->avgRouteLength * (double)(pThis->packetsByStatus[status] - 1) + (double)pPacket->routing.length;
    pThis->avgRouteLength = totalRouteLength / (double)pThis->packetsByStatus[status];

//    if (type == ePKT_TYPE_DATA)
//    {
//        if (pPacket->header.transitDstId == pPacket->header.originalDstId)
//            pThis->dataAmounts[eTRAFFIC_DELIVERED] += size;
//    }

    return eSTATUS_COMMON_OK;
}

