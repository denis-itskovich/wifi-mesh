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

#include "Packet.h"
#include "Macros.h"

#ifndef __NO_LOG
    static const char* PKT_TYPES[ePKT_TYPE_LAST] =
    {
         "Search Request",
         "Search Response",
         "Data",
         "Ack"
    };
#endif // !__NO_LOG

#define CONSTRUCT_PACKET(pptr, _msgtype, ...) SAFE_OPERATION \
	( \
		VALIDATE_ARGUMENTS(pptr); \
		*pptr = NEW(Packet); \
		VALIDATE(*pptr, eSTATUS_COMMON_NO_MEMORY); \
		return PacketInit ## _msgtype (*pptr, __VA_ARGS__) \
	)

#define INIT_PACKET(ptr, _msgtype, _srcId, _dstId) SAFE_OPERATION \
	( \
		VALIDATE_ARGUMENTS(ptr); \
		CLEAR(ptr); \
		ptr->header.originalSrcId = _srcId; \
		ptr->header.originalDstId = _dstId; \
		ptr->header.transitSrcId = _srcId; \
		ptr->header.transitDstId = INVALID_STATION_ID; \
		ptr->header.type = _msgtype; \
	)

EStatus PacketNew(Packet** ppThis, EPacketType msgType, StationId srcId, StationId dstId)
{
	CONSTRUCT(ppThis, Packet, msgType, srcId, dstId);
}

EStatus PacketNewData(Packet** ppThis, StationId srcId, StationId dstId, unsigned long size)
{
	CONSTRUCT_PACKET(ppThis, Data, srcId, dstId, size);
}

EStatus PacketNewSearchRequest(Packet** ppThis, StationId srcId, StationId lookForId)
{
	CONSTRUCT_PACKET(ppThis, SearchRequest, srcId, lookForId);
}

EStatus PacketNewSearchResponse(Packet** ppThis, StationId srcId, StationId dstId)
{
	CONSTRUCT_PACKET(ppThis, SearchResponse, srcId, dstId);
}

EStatus PacketNewAck(Packet** ppThis, StationId srcId, StationId dstId)
{
	CONSTRUCT_PACKET(ppThis, Ack, srcId, dstId);
}

EStatus PacketDelete(Packet** ppThis)
{
	DESTRUCT(ppThis, Packet);
}

EStatus PacketDestroy(Packet* pThis)
{
	return eSTATUS_COMMON_OK;
}

EStatus PacketInit(Packet* pThis, EPacketType msgType, StationId srcId, StationId dstId)
{
	INIT_PACKET(pThis, msgType, srcId, dstId);
	return eSTATUS_COMMON_OK;
}

EStatus PacketInitData(Packet* pThis, StationId srcId, StationId dstId, unsigned long size)
{
	INIT_PACKET(pThis, ePKT_TYPE_DATA, srcId, dstId);
	pThis->payload.size = size;

	return eSTATUS_COMMON_OK;
}

EStatus PacketInitSearchRequest(Packet* pThis, StationId srcId, StationId lookForId)
{
	INIT_PACKET(pThis, ePKT_TYPE_SEARCH_REQUEST, srcId, lookForId);
	pThis->header.transitDstId = BROADCAST_STATION_ID;
	return eSTATUS_COMMON_OK;
}

EStatus PacketInitSearchResponse(Packet* pThis, StationId srcId, StationId dstId)
{
	INIT_PACKET(pThis, ePKT_TYPE_SEARCH_RESPONSE, srcId, dstId);
	return eSTATUS_COMMON_OK;
}

EStatus PacketInitAck(Packet* pThis, StationId srcId, StationId dstId)
{
	INIT_PACKET(pThis, ePKT_TYPE_ACK, srcId, dstId);
	return eSTATUS_COMMON_OK;
}

EStatus PacketClone(Packet** ppDst, const Packet* pSrc)
{
	VALIDATE_ARGUMENTS(ppDst && pSrc);

	CHECK(PacketNew(ppDst, pSrc->header.type, pSrc->header.originalSrcId, pSrc->header.originalDstId));
	memcpy(*ppDst, pSrc, sizeof(*pSrc));

	return eSTATUS_COMMON_OK;
}

EStatus PacketGetSize(const Packet* pThis, unsigned* pSize)
{
    unsigned size = sizeof(pThis->header);
    VALIDATE_ARGUMENTS(pThis && pSize);
    size += pThis->payload.size;
    size += pThis->routing.length * pThis->routing.path[0];
    *pSize = size;
    return eSTATUS_COMMON_OK;
}

EStatus PacketDump(const Packet* pThis)
{
    DUMP_PRINT("Packet: [type: %s, from: %d, to: %d, src: %d, dst: %d]",
               pThis->header.type < ePKT_TYPE_LAST ? PKT_TYPES[pThis->header.type] : "Invalid",
               pThis->header.originalSrcId,
               pThis->header.originalDstId,
               pThis->header.transitSrcId,
               pThis->header.transitDstId);
    return eSTATUS_COMMON_OK;
}
