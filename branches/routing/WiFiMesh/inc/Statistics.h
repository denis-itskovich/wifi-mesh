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
 * @file Statistics.h
 * @date 14/02/2009
 * @author Denis Itskovich
 */

#ifndef STATISTICS_H_
#define STATISTICS_H_

#include "Status.h"
#include "Packet.h"
#include "Scheduler.h"

/** Packet status, handled by sniffer
 * @see PacketSniffer
 */
typedef enum
{
    ePKT_STATUS_DELIVERED,          ///< Transmit was successfully finished
    ePKT_STATUS_COLLISION,          ///< Transmit was aborted due to collision
    ePKT_STATUS_OUT_OF_RANGE,       ///< Transmit was not started because destination was out of range
    ePKT_STATUS_HOPS_LIMIT_REACHED, ///< Transmit was not started because maximal hops count was reached
    ePKT_STATUS_PENDING,            ///< Packet transmitting has been recently started

    ePKT_STATUS_LAST            ///< Terminator
} EPacketStatus;

typedef enum
{
    eTRAFFIC_SCHEDULED,         ///< Scheduled for delivery data - total size of all scheduled packets
    eTRAFFIC_ISSUED,            ///< Issued - delivery was started
    eTRAFFIC_DELIVERED,         /**< Delivered data - total size of all delivered from point to point data packets
                                     (not including intermediators traffic) */

    eTRAFFIC_LAST               ///< Terminator
} ETraffic;

/// Statistics collection
typedef struct _Statistics
{
    int     packetsByStatus[ePKT_STATUS_LAST];      ///< Packet counts by delivery status @see EPacketStatus
    int     packetsByType[ePKT_TYPE_LAST];          ///< Packet counts by packet type @see EPacketType
    int     sizeByType[ePKT_TYPE_LAST];             ///< Data amounts by packet type @see EPacketType
    int     packetsByTraffic[eTRAFFIC_LAST];        ///< Packets count by traffic type @see ETraffic
    int     sizeByTraffic[eTRAFFIC_LAST];           ///< Data amounts by traffic type @see ETraffic

    int     maxHopsCount;                           ///< Maximal delivered hops count
    double  avgHopsCount;                           ///< Average delivered hops count

    int     maxRouteLength;                         ///< Longest delivered route length
    double  avgRouteLength;                         ///< Average delivered route length
} Statistics;

/** Allocates and initializes new instance of statistics
 * @param ppThis [out] pointer to new instance will be stored at *ppThis
 */
EStatus StatisticsNew(Statistics** ppThis);

/** Destroys and deallocates an instance
 * @param ppThis [in, out] *ppThis should point to valid statistics instance
 */
EStatus StatisticsDelete(Statistics** ppThis);

/** Initializes an instance
 * @param pThis [in] pointer to instance
 */
EStatus StatisticsInit(Statistics* pThis);

/** Destroys an instance
 * @param pThis [in] pointer to instance
 */
EStatus StatisticsDestroy(Statistics* pThis);

/** Resets statistics.
 * Statistics should be reset before each run of the simulation
 * @param pThis [in] pointer to instance
 */
EStatus StatisticsReset(Statistics* pThis);

/** Clear statistics.
 * Statistics should be cleared when clearing "the world"
 * @param pThis [in] pointer to instance
 */
EStatus StatisticsClear(Statistics* pThis);

/** Handles packet - used for collecting packet statistics
 * @param pThis [in] pointer to instance
 * @param pPacket [in] pointer to packet
 * @param status [in] packet status
 */
EStatus StatisticsHandlePacket(Statistics* pThis, const Packet* pPacket, EPacketStatus status);

/** Handles scheduler event
 * @param pThis [in] pointer to instance
 * @param pPacket [in] pointer to packet
 * @param event [in] scheduler event
 */
EStatus StatisticsHandleSchedulerEvent(Statistics* pThis, const Packet* pPacket, ESchedulerEvent event);

/** Prints statistics into buffer
 * @param pThis [in] pointer to instance
 * @param buffer [out] character buffer
 * @param len [in] buffer length
 */
EStatus StatisticsPrint(const Statistics* pThis, char* buffer, int len);

#endif /* STATISTICS_H_ */
