#ifndef _WIFI_MESH_PROTOCOL_H
#define _WIFI_MESH_PROTOCOL_H

typedef enum
{
	eMSG_TYPE_SEARCH_REQUEST,
	eMSG_TYPE_SEARCH_RESPONSE,
	eMSG_TYPE_DATA,
	eMSG_TYPE_ACK
} EMessageType;

typedef unsigned long StationId;

typedef union _Payload
{
	struct
	{
		StationId id;
	} searchRequest;

	struct
	{
	} searchResponse;

	struct
	{
		unsigned long size;
	} data;

} Payload;

#endif // _WIFI_MESH_PROTOCOL_H
