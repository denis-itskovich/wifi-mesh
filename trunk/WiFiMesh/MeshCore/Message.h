#ifndef _WIFI_MESH_MESSAGE_H
#define _WIFI_MESH_MESSAGE_H

#include "Status.h"

typedef struct _Message Message;

EStatus MessageCreateData(Message** ppThis, StationId srcId, StationId dstId, unsigned long size);
EStatus MessageCreateSearchRequest(Message** ppThis, StationId srcId, StationId dstId, StationId lookForId);
EStatus MessageCreateSearchResponse(Message** ppThis, StationId srcId, StationId dstId);
EStatus MessageCreateAck(Message** ppThis, StationId srcId, StationId dstId);

EStatus MessageDestroy(Message** ppThis);
EStatus MessageClone(Message** ppDst, Message* pSrc);

#endif // _WIFI_MESH_MESSAGE_H
