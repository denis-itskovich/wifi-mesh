#include "Message.h"
#include "Macros.h"

#define CONSTRUCT_MESSAGE(pptr, _msgtype, ...) SAFE_OPERATION \
	( \
		VALIDATE_ARGUMENTS(pptr); \
		*pptr = NEW(Message); \
		VALIDATE(*pptr, eSTATUS_COMMON_NO_MEMORY); \
		return MessageInit ## _msgtype (*pptr, __VA_ARGS__) \
	)

#define INIT_MESSAGE(ptr, _msgtype, _srcId, _dstId) SAFE_OPERATION \
	( \
		VALIDATE_ARGUMENTS(ptr); \
		CLEAR(ptr); \
		ptr->originalSrcId = _srcId; \
		ptr->originalDstId = _dstId; \
		ptr->type = _msgtype; \
	)

EStatus MessageNew(Message** ppThis, EMessageType msgType, StationId srcId, StationId dstId)
{
	CONSTRUCT(ppThis, Message, msgType, srcId, dstId);
}

EStatus MessageNewData(Message** ppThis, StationId srcId, StationId dstId, unsigned long size)
{
	CONSTRUCT_MESSAGE(ppThis, Data, srcId, dstId, size);
}

EStatus MessageNewSearchRequest(Message** ppThis, StationId srcId, StationId lookForId)
{
	CONSTRUCT_MESSAGE(ppThis, SearchRequest, srcId, lookForId);
}

EStatus MessageNewSearchResponse(Message** ppThis, StationId srcId, StationId dstId)
{
	CONSTRUCT_MESSAGE(ppThis, SearchResponse, srcId, dstId);
}

EStatus MessageNewAck(Message** ppThis, StationId srcId, StationId dstId)
{
	CONSTRUCT_MESSAGE(ppThis, SearchRequest, srcId, dstId);
}

EStatus MessageDelete(Message** ppThis)
{
	DESTRUCT(ppThis, Message);
}

EStatus MessageDestroy(Message* pThis)
{
	return eSTATUS_COMMON_OK;
}

EStatus MessageInit(Message* pThis, EMessageType msgType, StationId srcId, StationId dstId)
{
	INIT_MESSAGE(pThis, msgType, srcId, dstId);
	return eSTATUS_COMMON_OK;
}

EStatus MessageInitData(Message* pThis, StationId srcId, StationId dstId, unsigned long size)
{
	INIT_MESSAGE(pThis, eMSG_TYPE_DATA, srcId, dstId);
	pThis->size = size;

	return eSTATUS_COMMON_OK;
}

EStatus MessageInitSearchRequest(Message* pThis, StationId srcId, StationId lookForId)
{
	INIT_MESSAGE(pThis, eMSG_TYPE_SEARCH_REQUEST, srcId, lookForId);
	return eSTATUS_COMMON_OK;
}

EStatus MessageInitSearchResponse(Message* pThis, StationId srcId, StationId dstId)
{
	INIT_MESSAGE(pThis, eMSG_TYPE_SEARCH_RESPONSE, srcId, dstId);
	return eSTATUS_COMMON_OK;
}

EStatus MessageInitAck(Message* pThis, StationId srcId, StationId dstId)
{
	INIT_MESSAGE(pThis, eMSG_TYPE_ACK, srcId, dstId);
	return eSTATUS_COMMON_OK;
}

EStatus MessageClone(Message** ppDst, const Message* pSrc)
{
	VALIDATE_ARGUMENTS(ppDst && pSrc);

	CHECK(MessageNew(ppDst, pSrc->type, pSrc->originalSrcId, pSrc->originalDstId));
	(*ppDst)->size = pSrc->size;
	(*ppDst)->transitSrcId = pSrc->transitSrcId;
	(*ppDst)->transitDstId = pSrc->transitDstId;

	return eSTATUS_COMMON_OK;
}
