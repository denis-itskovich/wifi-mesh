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
 * @file Packet.h
 *
 * Packet unit
 *
 * Represents a network packet
 *
 * @author Denis Itskovich
 * @date   1/12/2008
 */

#ifndef _WIFI_MESH_PACKET_H
#define _WIFI_MESH_PACKET_H

#include "Status.h"
#include "CommonTypes.h"

#define MAX_PATH_LENGTH     64

/// Packet type
typedef enum
{
	ePKT_TYPE_SEARCH_REQUEST,		///< Search request - sent via broadcast in order to locate a route
	ePKT_TYPE_SEARCH_RESPONSE,		///< Search response - response to search request
	ePKT_TYPE_DATA,					///< Data packet
	ePKT_TYPE_ACK,					///< Acknowledgment packet - response to data packet

	ePKT_TYPE_LAST					///< Terminator item
} EPacketType;

/// Packet declaration
typedef struct _Packet
{
    //---------------------------------------------------------------------------
    struct _PacketHeader
    {
        EPacketType     type;                       ///< Packet type
        StationId       originalSrcId;              ///< Original source station id
        StationId       originalDstId;              ///< Original destination station id
        StationId       transitSrcId;               ///< Transit source id
        StationId       transitDstId;               ///< Transit destination id
        unsigned        hopsCount;                  ///< Transit nodes counts
        unsigned        sequenceNum;                ///< Packet sequence number
    }                   header;                     ///< Packet header
    //---------------------------------------------------------------------------
    struct _PacketPayload
    {
        unsigned long   size;                       ///< Data size
        int             id;                         ///< Data packet id
    }                   payload;                    ///< Data packet payload
    //---------------------------------------------------------------------------
    struct _PacketRouting
    {
        unsigned        length;                     ///< Path length
        StationId       path[MAX_PATH_LENGTH];      ///< Path - node list
    }                   routing;                    ///< Routing information
    //---------------------------------------------------------------------------
} Packet;

/** Allocates new packet
 * @param ppThis [out] pointer to new instance will be stored at *ppThis
 * @param msgType [in] packet type
 * @param srcId [in] source id
 * @param dstId [in] destination id
 */
EStatus PacketNew(Packet** ppThis, EPacketType msgType, StationId srcId, StationId dstId);

/** Allocates data packet
 * @param ppThis [out] pointer to new instance will be stored at *ppThis
 * @param srcId [in] source id
 * @param dstId [in] destination id
 * @param size [in] size
 * @param id [in] data packet id
 */
EStatus PacketNewData(Packet** ppThis, StationId srcId, StationId dstId, unsigned long size, int id);

/** Allocates search request packet
 * @param ppThis [out] pointer to new instance will be stored at *ppThis
 * @param srcId [in] source id
 * @param lookForId [in] id of the station to look for
 */
EStatus PacketNewSearchRequest(Packet** ppThis, StationId srcId, StationId lookForId);

/** Allocates search response packet
 * @param ppThis [out] pointer to new instance will be stored at *ppThis
 * @param srcId [in] source id
 * @param dstId [in] destination id
 */
EStatus PacketNewSearchResponse(Packet** ppThis, StationId srcId, StationId dstId);

/** Allocates ack packet
 * @param ppThis [in] pointer to new instance will be stored at *ppThis
 * @param srcId [in] source id
 * @param dstId [in] destination id
 */
EStatus PacketNewAck(Packet** ppThis, StationId srcId, StationId dstId);

/** Initializes packet
 * @param pThis [in] pointer to instance
 * @param msgType [in] packet type
 * @param srcId [in] source id
 * @param dstId [in] destination id
 */
EStatus PacketInit(Packet* pThis, EPacketType msgType, StationId srcId, StationId dstId);

/** Initializes data packet
 * @param pThis [in] pointer to instance
 * @param srcId [in] source id
 * @param dstId [in] destination id
 * @param size [in] size
 * @param id [in] data packet id
 */
EStatus PacketInitData(Packet* pThis, StationId srcId, StationId dstId, unsigned long size, int id);

/** Initializes search request packet
 * @param pThis [in] pointer to instance
 * @param srcId [in] source id
 * @param lookForId [in] id of the station to look for
 */
EStatus PacketInitSearchRequest(Packet* pThis, StationId srcId, StationId lookForId);

/** Initializes search response packet
 * @param pThis [in] pointer to instance
 * @param srcId [in] source id
 * @param dstId [in] destination id
 */
EStatus PacketInitSearchResponse(Packet* pThis, StationId srcId, StationId dstId);

/** Initializes ack packet
 * @param pThis [in] pointer to instance
 * @param srcId [in] source id
 * @param dstId [in] destination id
 */
EStatus PacketInitAck(Packet* pThis, StationId srcId, StationId dstId);

/** Destroys and deallocates an instance
 * @param ppThis [in, out] *ppThis must point to valid instance
 */
EStatus PacketDelete(Packet** ppThis);

/** Destroys an instance
 * @param pThis [in] must point to valid instance
 */
EStatus PacketDestroy(Packet* pThis);

/** Copies a packet
 * @param pDst [in] destination instance
 * @param pSrc [in] source instance
 */
EStatus PacketCopy(Packet* pDst, const Packet* pSrc);

/** Clones (allocates and copies) a packet
 * @param ppDst [out] pointer to the cloned packet will be stored at *ppDst
 * @param pSrc [in] source instance
 */
EStatus PacketClone(Packet** ppDst, const Packet* pSrc);

/** Calculates packet size
 * @param pThis [in] packet instance
 * @param pSize [out] size will be stored at *pSize
 */
EStatus PacketGetSize(const Packet* pThis, unsigned* pSize);

/** Dumps a packet
 * @param pThis [in] pointer to packet
 */
EStatus PacketDump(const Packet* pThis);

#endif // _WIFI_MESH_PACKET_H
