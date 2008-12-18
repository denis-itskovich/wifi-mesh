#include "Message.h"
#include "Protocol.h"
#include "Macros.h"

#define CONSTRUCT_MESSAGE(pptr, _msgType, _srcId, _dstId) SAFE_OPERATION \
	( \
		CONSTRUCT(pptr, Message, TRUE); \
		*pptr->type = _msgType; \
		*pptr->originalSrcId = _srcId; \
		*pptr->originalDstId = _dstId \
	)

EStatus MessageNewData(Message** ppThis, StationId srcId, StationId dstId, unsigned long size)
{
	CONSTRUCT_MESSAGE(ppThis, eMSG_TYPE_DATA, srcId, dstId);
	*ppThis->size = size;

	return eSTATUS_COMMON_OK;
}

EStatus MessageNewSearchRequest(Message** ppThis, StationId srcId, StationId lookForId)
{
	CONSTRUCT_MESSAGE(ppThis, eMSG_TYPE_SEARCH_REQUEST, srcId, lookForId);
	return eSTATUS_COMMON_OK;
}

EStatus MessageNewSearchResponse(Message** ppThis, StationId srcId, StationId dstId)
{
	CONSTRUCT_MESSAGE(ppThis, eMSG_TYPE_SEARCH_RESPONSE, srcId, dstId);

	return eSTATUS_COMMON_OK;
}

EStatus MessageNewAck(Message** ppThis, StationId srcId, StationId dstId)
{
	CONSTRUCT_MESSAGE(ppThis, eMSG_TYPE_ACK, srcId, dstId);

	return eSTATUS_COMMON_OK;
}

EStatus MessageDelete(Message** ppThis)
{
	VALIDATE_ARGUMENTS(ppThis && *ppThis);
	DELETE(*ppThis);
}

EStatus MessageClone(Message** ppDst, Message* pSrc)
{
	CONSTRUCT_MESSAGE(ppDst, pSrc->type, pSrc->srcId, pSrc->dstId);
	*ppDst->payload = pSrc->payload;

	return eSTATUS_COMMON_OK;
}
