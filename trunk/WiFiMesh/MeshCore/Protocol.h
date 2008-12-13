#ifndef _WIFI_MESH_PROTOCOL_H
#define _WIFI_MESH_PROTOCOL_H

#include "Status.h"

typedef struct _Protocol Protocol;

EStatus ProtocolCreate(Protocol** pThis, TimeLine* pTimeLine);
EStatus ProtocolDispose(Protocol** ppThis);
EStatus ProtocolInit(Protocol* pThis, TimeLine* pTimeLine);
EStatus ProtocolDestroy(Protocol* pThis);

EStatus ProtocolDispatchMessage(Protocol* pThis, Message* pMessage);

#endif // _WIFI_MESH_PROTOCOL_H
