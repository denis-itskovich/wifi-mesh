#ifndef _WIFI_MESH_EVENT_H
#define _WIFI_MESH_EVENT_H

#include "Message.h"
#include "Status.h"

typedef struct _Event Event;

EStatus EventCreate(Event** ppThis, unsigned time, Message* pMessage);
EStatus EventDestroy(Event** ppThis);


#endif // _WIFI_MESH_EVENT_H
