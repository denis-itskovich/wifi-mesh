#include "Packet.h"
#include "Macros.h"

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
	(*ppDst)->serviceInfo.retriesCount = 0;

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
