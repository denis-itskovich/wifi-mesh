/**
 * \file Protocol.h
 *
 * Protocol manager
 *
 * \author Denis Itskovich
 * \date 12/12/2008
 */
#ifndef _WIFI_MESH_PROTOCOL_H
#define _WIFI_MESH_PROTOCOL_H

#include "Status.h"
#include "TimeLine.h"

typedef struct _Protocol Protocol; ///< forward declaration of protocol manager

/** Allocates and initializes new instance
 * \param ppThis [out] pointer to new instance will be stored at *ppThis
 * \param pTimeLine [in] pointer to time line
 */
EStatus ProtocolNew(Protocol** ppThis, TimeLine* pTimeLine);

/** Destroys and deallocates an instance
 * \param ppThis [in, out] pointer to valid instance
 */
EStatus ProtocolDelete(Protocol** ppThis);

/** Initializes an instance
 * \param pThis [in] pointer to instance
 * \param pTimeLine [in] pointer to time line
 */
EStatus ProtocolInit(Protocol* pThis, TimeLine* pTimeLine);

/** Destroys an instance
 * \param pThis [in] pointer to valid instance
 */
EStatus ProtocolDestroy(Protocol* pThis);

/** Dispatches a message
 * \param pThis [in] pointer to instance
 * \param pMessage [in] pointer to message
 */
EStatus ProtocolDispatchMessage(Protocol* pThis, Message* pMessage);

#endif // _WIFI_MESH_PROTOCOL_H
