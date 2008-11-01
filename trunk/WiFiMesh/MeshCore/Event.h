#ifndef _WIFI_MESH_EVENT_H
#define _WIFI_MESH_EVENT_H

#include "List.h"
#include "Message.h"

typedef struct _Event
{
	ListHeader 		list;
	unsigned long	time;
	Message			message;
} Event;

void CreateEvent()

#endif // _WIFI_MESH_EVENT_H
