#include "Message.h"
#include "Protocol.h"
#include "Macros.h"

#define CONSTRUCT_MESSAGE(pptr, _msgType, _srcId, _dstId) SAFE_OPERATION(CONSTRUCT(pptr, Message, TRUE); *pptr->type = _msgType; *pptr->srcId = _srcId; *pptr->dstId = _dstId)

struct _Message
{
};

EStatus MessageCreateData(Message** ppThis, StationId srcId, StationId dstId, unsigned long size)
{
	CONSTRUCT_MESSAGE(ppThis, eMSG_TYPE_DATA, srcId, dstId);
	*ppThis->payload.data.size = size;

	return eSTATUS_COMMON_OK;
}

EStatus MessageCreateSearchRequest(Message** ppThis, StationId srcId, StationId dstId, StationId lookForId)
{
	CONSTRUCT_MESSAGE(ppThis, eMSG_TYPE_SEARCH_REQUEST, srcId, dstId);
	*ppThis->payload.searchRequest.id = lookForId;

	return eSTATUS_COMMON_OK;
}

EStatus MessageCreateSearchResponse(Message** ppThis, StationId srcId, StationId dstId)
{
	CONSTRUCT_MESSAGE(ppThis, eMSG_TYPE_SEARCH_RESPONSE, srcId, dstId);

	return eSTATUS_COMMON_OK;
}

EStatus MessageCreateAck(Message** ppThis, StationId srcId, StationId dstId)
{
	CONSTRUCT_MESSAGE(ppThis, eMSG_TYPE_ACK, srcId, dstId);
}

EStatus MessageDestroy(Message** ppThis)
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
