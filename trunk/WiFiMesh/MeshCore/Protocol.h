#ifndef _WIFI_MESH_PROTOCOL_H
#define _WIFI_MESH_PROTOCOL_H

typedef enum
{
	eMSG_TYPE_SEARCH_REQUEST,
	eMSG_TYPE_SEARCH_RESPONSE,
	eMSG_TYPE_DATA,
	eMSG_TYPE_ACK
} EMessageType;

typedef union _Payload
{
	struct
	{
		unsigned long	searchId;
		unsigned long	routeLength;
	} searchRequest;

	struct
	{
		unsigned long	routeLength;
	} searchResponse;

	struct
	{
		unsigned long dataSize;
	} data;

} Payload;

#endif // _WIFI_MESH_PROTOCOL_H
