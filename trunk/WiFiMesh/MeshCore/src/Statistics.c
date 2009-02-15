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
    int scheduledDataAmount = pThis->scheduledDataAmount;
    CHECK(StatisticsClear(pThis));
    pThis->scheduledDataAmount = scheduledDataAmount;
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

EStatus StatisticsHandlePacket(Statistics* pThis, const Packet* pPacket, EPacketStatus status)
{
    EPacketType type;
    unsigned size;
    double totalHopsCount;
    double totalRouteLength;

    VALIDATE_ARGUMENTS(pThis);

    type = pPacket->header.type;
    ++pThis->packetsByStatus[status];
    if (status != ePKT_STATUS_DELIVERED) return eSTATUS_COMMON_OK;

    ++pThis->packetsByType[type];

    CHECK(PacketGetSize(pPacket, &size));
    type = pPacket->header.type;

    totalHopsCount = pThis->avgHopsCount * (double)(pThis->packetsByStatus[status] - 1) + (double)pPacket->header.hopsCount;
    pThis->avgHopsCount = totalHopsCount / (double)pThis->packetsByStatus[status];

    totalRouteLength = pThis->avgRouteLength * (double)(pThis->packetsByStatus[status] - 1) + (double)pPacket->routing.length;
    pThis->avgRouteLength = totalRouteLength / (double)pThis->packetsByStatus[status];

    if (type == ePKT_TYPE_DATA)
    {
        pThis->usefulDataAmount += size;
        if (pPacket->header.transitDstId == pPacket->header.originalDstId)
            pThis->deliveredDataAmount += size;
    }
    else
    {
        pThis->controlDataAmount += size;
    }

    return eSTATUS_COMMON_OK;
}
