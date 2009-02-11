/**
 * \file Packet.h
 *
 * Packet unit
 *
 * Represents a network packet
 *
 * \author Denis Itskovich
 * \date   1/12/2008
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
    struct
    {
        EPacketType    type;                    ///< Packet type
        StationId      originalSrcId;           ///< Original source station id
        StationId      originalDstId;           ///< Original destination station id
        StationId      transitSrcId;            ///< Transit source id
        StationId      transitDstId;            ///< Transit destination id
        unsigned       hopsCount;               ///< Transit nodes counts
        unsigned       sequenceNum;             ///< Packet sequence number
        unsigned       timeToLive;              ///< Maximum hops count to live
    } header;                                   ///< Packet header
    struct
    {
        unsigned long size;                     ///< Data size
    } payload;                                  ///< Data packet payload
    struct
    {
        unsigned   length;
        StationId  path[MAX_PATH_LENGTH];       ///< Path - node list
    } routing;                                  ///< Routing information
} Packet;

/** Allocates new packet
 * \param ppThis [out] pointer to new instance will be stored at *ppThis
 * \param msgType [in] packet type
 * \param srcId [in] source id
 * \param dstId [in] destination id
 */
EStatus PacketNew(Packet** ppThis, EPacketType msgType, StationId srcId, StationId dstId);

/** Allocates data packet
 * \param ppThis [out] pointer to new instance will be stored at *ppThis
 * \param srcId [in] source id
 * \param dstId [in] destination id
 * \param size [in] size
 */
EStatus PacketNewData(Packet** ppThis, StationId srcId, StationId dstId, unsigned long size);

/** Allocates search request packet
 * \param ppThis [out] pointer to new instance will be stored at *ppThis
 * \param srcId [in] source id
 * \param lookForId [in] id of the station to look for
 */
EStatus PacketNewSearchRequest(Packet** ppThis, StationId srcId, StationId lookForId);

/** Allocates search response packet
 * \param ppThis [out] pointer to new instance will be stored at *ppThis
 * \param srcId [in] source id
 * \param dstId [in] destination id
 */
EStatus PacketNewSearchResponse(Packet** ppThis, StationId srcId, StationId dstId);

/** Allocates ack packet
 * \param ppThis [in] pointer to new instance will be stored at *ppThis
 * \param srcId [in] source id
 * \param dstId [in] destination id
 */
EStatus PacketNewAck(Packet** ppThis, StationId srcId, StationId dstId);

/** Initializes packet
 * \param pThis [in] pointer to instance
 * \param msgType [in] packet type
 * \param srcId [in] source id
 * \param dstId [in] destination id
 */
EStatus PacketInit(Packet* pThis, EPacketType msgType, StationId srcId, StationId dstId);

/** Initializes data packet
 * \param pThis [in] pointer to instance
 * \param srcId [in] source id
 * \param dstId [in] destination id
 * \param size [in] size
 */
EStatus PacketInitData(Packet* pThis, StationId srcId, StationId dstId, unsigned long size);

/** Initializes search request packet
 * \param pThis [in] pointer to instance
 * \param srcId [in] source id
 * \param lookForId [in] id of the station to look for
 */
EStatus PacketInitSearchRequest(Packet* pThis, StationId srcId, StationId lookForId);

/** Initializes search response packet
 * \param pThis [in] pointer to instance
 * \param srcId [in] source id
 * \param dstId [in] destination id
 */
EStatus PacketInitSearchResponse(Packet* pThis, StationId srcId, StationId dstId);

/** Initializes ack packet
 * \param pThis [in] pointer to instance
 * \param srcId [in] source id
 * \param dstId [in] destination id
 */
EStatus PacketInitAck(Packet* pThis, StationId srcId, StationId dstId);

/** Destroys and deallocates an instance
 * \param ppThis [in, out] *ppThis must point to valid instance
 */
EStatus PacketDelete(Packet** ppThis);

/** Destroys an instance
 * \param pThis [in] must point to valid instance
 */
EStatus PacketDestroy(Packet* pThis);

/** Copies a packet
 * \param pDst [in] destination instance
 * \param pSrc [in] source instance
 */
EStatus PacketCopy(Packet* pDst, const Packet* pSrc);

/** Clones (allocates and copies) a packet
 * \param ppDst [out] pointer to the cloned packet will be stored at *ppDst
 * \param pSrc [in] source instance
 */
EStatus PacketClone(Packet** ppDst, const Packet* pSrc);

/** Calculates packet size
 * \param pThis [in] packet instance
 * \param pSize [out] size will be stored at *pSize
 */
EStatus PacketGetSize(const Packet* pThis, unsigned* pSize);

#endif // _WIFI_MESH_PACKET_H
