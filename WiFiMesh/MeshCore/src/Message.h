/**
 * \file Message.h
 *
 * Message unit
 *
 * Represents a network message
 *
 * \author Denis Itskovich
 * \date   1/12/2008
 */

#ifndef _WIFI_MESH_MESSAGE_H
#define _WIFI_MESH_MESSAGE_H

#include "Status.h"
#include "CommonTypes.h"

/// Message type
typedef enum
{
	eMSG_TYPE_SEARCH_REQUEST,		///< Search request - sent via broadcast in order to locate a route
	eMSG_TYPE_SEARCH_RESPONSE,		///< Search response - response to search request
	eMSG_TYPE_DATA,					///< Data message
	eMSG_TYPE_ACK,					///< Acknowledgment message - response to data message

	eMSG_TYPE_LAST					///< Terminator item
} EMessageType;

/// Message declaration
typedef struct _Message
{
	EMessageType	type;					///< Message type
	StationId		originalSrcId;			///< Original source station id
	StationId		originalDstId;			///< Original destination station id
	StationId		transitSrcId;			///< Transit source id
	StationId		transitDstId;			///< Transit destination id
	unsigned		nodesCount;				///< Transit nodes counts

	unsigned long 	size;					///< Data size <i>(for data packets only)</i>
} Message;


/** Allocates new message
 * \param ppThis [out] pointer to new instance will be stored at *ppThis
 * \param msgType [in] message type
 * \param srcId [in] source id
 * \param dstId [in] destination id
 */
EStatus MessageNew(Message** ppThis, EMessageType msgType, StationId srcId, StationId dstId);

/** Allocates data message
 * \param ppThis [out] pointer to new instance will be stored at *ppThis
 * \param srcId [in] source id
 * \param dstId [in] destination id
 * \param size [in] size
 */
EStatus MessageNewData(Message** ppThis, StationId srcId, StationId dstId, unsigned long size);

/** Allocates search request message
 * \param ppThis [out] pointer to new instance will be stored at *ppThis
 * \param srcId [in] source id
 * \param lookForId [in] id of the station to look for
 */
EStatus MessageNewSearchRequest(Message** ppThis, StationId srcId, StationId lookForId);

/** Allocates search response message
 * \param ppThis [out] pointer to new instance will be stored at *ppThis
 * \param srcId [in] source id
 * \param dstId [in] destination id
 */
EStatus MessageNewSearchResponse(Message** ppThis, StationId srcId, StationId dstId);

/** Allocates ack message
 * \param ppThis [in] pointer to new instance will be stored at *ppThis
 * \param srcId [in] source id
 * \param dstId [in] destination id
 */
EStatus MessageNewAck(Message** ppThis, StationId srcId, StationId dstId);

/** Initializes message
 * \param pThis [in] pointer to instance
 * \param msgType [in] message type
 * \param srcId [in] source id
 * \param dstId [in] destination id
 */
EStatus MessageInit(Message* pThis, EMessageType msgType, StationId srcId, StationId dstId);

/** Initializes data message
 * \param pThis [in] pointer to instance
 * \param srcId [in] source id
 * \param dstId [in] destination id
 * \param size [in] size
 */
EStatus MessageInitData(Message* pThis, StationId srcId, StationId dstId, unsigned long size);

/** Initializes search request message
 * \param pThis [in] pointer to instance
 * \param srcId [in] source id
 * \param lookForId [in] id of the station to look for
 */
EStatus MessageInitSearchRequest(Message* pThis, StationId srcId, StationId lookForId);

/** Initializes search response message
 * \param pThis [in] pointer to instance
 * \param srcId [in] source id
 * \param dstId [in] destination id
 */
EStatus MessageInitSearchResponse(Message* pThis, StationId srcId, StationId dstId);

/** Initializes ack message
 * \param pThis [in] pointer to instance
 * \param srcId [in] source id
 * \param dstId [in] destination id
 */
EStatus MessageInitAck(Message* pThis, StationId srcId, StationId dstId);

/** Destroys and deallocates an instance
 * \param ppThis [in, out] *ppThis must point to valid instance
 */
EStatus MessageDelete(Message** ppThis);

/** Destroys an instance
 * \param pThis [in] must point to valid instance
 */
EStatus MessageDestroy(Message* pThis);

/** Copies a message
 * \param pDst [in] destination instance
 * \param pSrc [in] source instance
 */
EStatus MessageCopy(Message* pDst, const Message* pSrc);

/** Clones (allocates and copies) a message
 * \param ppDst [out] pointer to the cloned message will be stored at *ppDst
 * \param pSrc [in] source instance
 */
EStatus MessageClone(Message** ppDst, const Message* pSrc);

#endif // _WIFI_MESH_MESSAGE_H
