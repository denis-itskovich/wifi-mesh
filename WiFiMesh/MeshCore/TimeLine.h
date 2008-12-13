#ifndef _WIFI_MESH_TIMELINE_H
#define _WIFI_MESH_TIMELINE_H

#include "Event.h"

typedef struct _TimeLine TimeLine;

EStatus TimeLineCreate(TimeLine** ppThis);
EStatus TimeLineDispose(TimeLine** ppThis);
EStatus TimeLineInit(TimeLine* pThis);
EStatus TimeLineDestroy(TimeLine* pThis);

EStatus TimeLineAddEvent(TimeLine* pThis, Event* pEvent);
EStatus TimeLineGetNextEvent(TimeLine* pThis, Event** pEvent);

#endif // _WIFI_MESH_TIMELINE_H
