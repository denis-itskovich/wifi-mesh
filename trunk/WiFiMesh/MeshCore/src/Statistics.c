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

#include "../inc/Macros.h"
#include "../inc/Statistics.h"
#include "../inc/Descriptors.h"

int __counter_Statistics = 0;

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
	int scheduledSize = 0;
	int scheduledPackets = 0;

    VALIDATE_ARGUMENTS(pThis);

    scheduledSize = pThis->sizeByTraffic[eTRAFFIC_SCHEDULED];
    scheduledPackets = pThis->packetsByTraffic[eTRAFFIC_SCHEDULED];

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

EStatus StatisticsUpdateValue(double values[eSTAT_LAST], double count, double value)
{
    values[eSTAT_MIN] = (count > 1) ? MIN(value, values[eSTAT_MIN]) : value;
    values[eSTAT_MAX] = (count > 1) ? MAX(value, values[eSTAT_MAX]) : value;
    values[eSTAT_AVG] = (values[eSTAT_AVG] * (count - 1.0) + value) / count;
    return eSTATUS_COMMON_OK;
}

EStatus StatisticsHandleSchedulerEvent(Statistics* pThis, const SchedulerEntry* pEntry)
{
    unsigned long size;
    double delay, count;
    double length;
    const Packet* pPacket;

    VALIDATE_ARGUMENTS(pThis && pEntry);

    pPacket = pEntry->pPacket;
    size = pPacket->payload.size;

    switch (pEntry->state)
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
        count = (double)++pThis->packetsByTraffic[eTRAFFIC_DELIVERED];
        pThis->sizeByTraffic[eTRAFFIC_DELIVERED] += size;
        delay = pEntry->timeStamp[eSCHEDULE_DELIVERED] - pEntry->timeStamp[eSCHEDULE_ISSUED];
        pThis->totalTime += delay;
        length = (double)pPacket->routing.length;
        CHECK(StatisticsUpdateValue(pThis->deliveryDelay, count, delay * 1000));
        CHECK(StatisticsUpdateValue(pThis->routeLength, count, length));
        CHECK(StatisticsUpdateValue(pThis->throughput, count, (double)size * 8.0 / delay));
        pThis->throughput[eSTAT_AVG] = pThis->sizeByTraffic[eTRAFFIC_DELIVERED] * 8 / pThis->totalTime;
        break;
    default:
        break;
    }

    return eSTATUS_COMMON_OK;
}

EStatus StatisticsHandlePacket(Statistics* pThis, const Packet* pPacket, EPacketStatus status)
{
    EPacketType type;
    unsigned size;
//    double totalHopsCount;
//    double totalRouteLength;
    static unsigned long long previousMsgId = (unsigned long long)(-1);
    unsigned long long msgId;

    VALIDATE_ARGUMENTS(pThis && pPacket);
    msgId = (unsigned long long)pPacket->header.transitSrcId | ((unsigned long long)pPacket->header.sequenceNum << 32);

    // if (msgId == previousMsgId) return eSTATUS_COMMON_OK;
    previousMsgId = msgId;

    type = pPacket->header.type;
    ++pThis->packetsByStatus[status];
    if (status != ePKT_STATUS_DELIVERED) return eSTATUS_COMMON_OK;

    CHECK(PacketGetSize(pPacket, &size));
    pThis->sizeByType[type] += size;
    ++pThis->packetsByType[type];

//    totalHopsCount = pThis->avgHopsCount * (double)(pThis->packetsByStatus[status] - 1) + (double)pPacket->header.hopsCount;
//    pThis->avgHopsCount = totalHopsCount / (double)pThis->packetsByStatus[status];
//
//    totalRouteLength = pThis->avgRouteLength * (double)(pThis->packetsByStatus[status] - 1) + (double)pPacket->routing.length;
//    pThis->avgRouteLength = totalRouteLength / (double)pThis->packetsByStatus[status];

//    if (type == ePKT_TYPE_DATA)
//    {
//        if (pPacket->header.transitDstId == pPacket->header.originalDstId)
//            pThis->dataAmounts[eTRAFFIC_DELIVERED] += size;
//    }

    return eSTATUS_COMMON_OK;
}

void StatisticsPrintArray(char** ppBuff,
                          int* pLen,
                          const char* title,
                          const char * const* desc,
                          const int* pIntArray,
                          const double* pDoubleArray,
                          int count,
                          Boolean isNested,
                          Boolean alignToMax)
{
    double total = 0;
	double percent = 0;
	int i;
    double val;
    char c;
    int lineLen;

    double* pValues = NULL;

    if (pIntArray)
    {
        pValues = NEW_ARRAY(double, count);
        for (i = 0; i < count; ++i) pValues[i] = (double)pIntArray[i];
        pDoubleArray = pValues;
    }

    if (!isNested)
    {
        for (i = 0; i < count; ++i)
        {
            if (!alignToMax) total += pDoubleArray[i];
            else total = MAX(pDoubleArray[i], total);
        }
    }
    else total = pDoubleArray[0];

    lineLen = sprintf(*ppBuff, "%s:\n", title);
    (*ppBuff) += lineLen;
    *pLen -= lineLen;

    for (i = 0; (i < count) && (*pLen > 80); ++i)
    {
        val = pDoubleArray[i];
        c = ' ';
        if (val > 10240) { val /= 1024.0; c = 'K'; }
        if (val > 10240) { val /= 1024.0; c = 'M'; }
        if (val > 10240) { val /= 1024.0; c = 'G'; }

        percent = total > 0 ? pDoubleArray[i] / total * 100 : 0;
        lineLen = sprintf(*ppBuff, "    %-24s%*.0lf%-*c (%.2lf%%)\n", desc[i], c == ' ' ? 9 : 8, val, c == ' ' ? 1 : 2, c, percent);
        (*ppBuff) += lineLen;
        *pLen -= lineLen;

        if (isNested) total = pDoubleArray[i];
    }

    lineLen = sprintf(*ppBuff, "\n");
    (*ppBuff) += lineLen;
    *pLen -= lineLen;

    free(pValues);
}

EStatus StatisticsPrint(const Statistics* pThis, char* buffer, int len)
{
    StatisticsPrintArray(&buffer, &len, "Traffic by packet type [bytes]", DESC_PACKET_TYPE, pThis->sizeByType, NULL, ePKT_TYPE_LAST, FALSE, FALSE);
    StatisticsPrintArray(&buffer, &len, "Packets count by packet type", DESC_PACKET_TYPE, pThis->packetsByType, NULL, ePKT_TYPE_LAST, FALSE, FALSE);
    StatisticsPrintArray(&buffer, &len, "Packets count by delivery status", DESC_PACKET_STATUS, pThis->packetsByStatus, NULL, ePKT_STATUS_PENDING, FALSE, FALSE);
    StatisticsPrintArray(&buffer, &len, "Traffic by scheduling status [bytes]", DESC_TRAFFIC_TYPE, pThis->sizeByTraffic, NULL, eTRAFFIC_LAST, TRUE, FALSE);
    StatisticsPrintArray(&buffer, &len, "Packets count by scheduling status", DESC_TRAFFIC_TYPE, pThis->packetsByTraffic, NULL, eTRAFFIC_LAST, TRUE, FALSE);
    StatisticsPrintArray(&buffer, &len, "Packet delivery delay [msec]", DESC_STAT_TYPE, NULL, pThis->deliveryDelay, eSTAT_LAST, FALSE, TRUE);
    StatisticsPrintArray(&buffer, &len, "Packet delivery path length", DESC_STAT_TYPE, NULL, pThis->routeLength, eSTAT_LAST, FALSE, TRUE);
    StatisticsPrintArray(&buffer, &len, "Upper layer throughput [bit/s]", DESC_STAT_TYPE, NULL, pThis->throughput, eSTAT_LAST, FALSE, TRUE);
    return eSTATUS_COMMON_OK;
}
