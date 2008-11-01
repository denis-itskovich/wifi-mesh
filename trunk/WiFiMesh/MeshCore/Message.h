#ifndef _WIFI_MESH_MESSAGE_H
#define _WIFI_MESH_MESSAGE_H

#include "Protocol.h"

typedef struct _Message
{
	EMessageType	type;
	unsigned long	srcId;
	unsigned long	dstId;
	unsigned long	size;
	Payload			payload;
} Message;

#endif // _WIFI_MESH_MESSAGE_H
