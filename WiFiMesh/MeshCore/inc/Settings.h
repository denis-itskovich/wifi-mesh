/**
 * \file Settings.h
 *
 * Project: MeshCore
 * (C) Copyright 2008 Denis Itskovich
 *
 * \date 22/12/2008
 * \author Denis Itskovich
 */


#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "Status.h"
#include "Message.h"

typedef struct _Settings Settings;	///< forward declaration

/** Allocates and initializes new instance
 * \param ppThis [out] new instance will be stored at *ppThis
 */
EStatus SettingsNew(Settings** ppThis);

/** Destroys and deallocates an instance
 * \param ppThis [in, out] *ppThis should point to valid instance
 */
EStatus SettingsDelete(Settings** ppThis);

/** Initializes an instance
 * \param pThis [in] pointer to instance
 */
EStatus SettingsInit(Settings* pThis);

/** Destroys an instance
 * \param pThis [in] pointer to instance
 */
EStatus SettingsDestroy(Settings* pThis);

/** Sets routing table entry time to live
 * \param pThis [in] pointer to instance
 * \param ttl [in] new routing time to live
 */
EStatus SettingsSetRoutingTTL(Settings* pThis, double ttl);

/** Sets coverage
 * \param pThis [in] pointer to instance
 * \param coverage [in] new coverage radius
 */
EStatus SettingsSetCoverage(Settings* pThis, double coverage);

/** Sets data rate
 * \param pThis [in] pointer to instance
 * \param dataRate [in] data rate
 */
EStatus SettingsSetDataRate(Settings* pThis, unsigned long dataRate);

/** Calculates transmit time
 * \param pThis [in] pointer to instance
 * \param pMessage [in] pointer to message
 * \param pTime [out] transmit time will be stored at *pTime
 */
EStatus SettingsGetTransmitTime(const Settings* pThis, const Message* pMessage, double* pTime);

/** Retrieves routing table entry time to live
 * \param pThis [in] pointer to instance
 * \param pTTL [out] time to live will be stored at *pTTL
 */
EStatus SettingsGetRoutingTTL(const Settings* pThis, double* pTTL);

/** Retrieves coverage
 * \param pThis [in] pointer to instance
 * \param pCoverage [out] coverage will be stored at *pCoverage
 */
EStatus SettingsGetCoverage(const Settings* pThis, double* pCoverage);

/** Retrieves data rate
 * \param pThis [in] pointer to instance
 * \param pDataRate [out] data rate will be stored at *pDataRate
 */
EStatus SettingsGetDataRate(const Settings* pThis, unsigned long* pDataRate);

#endif /* SETTINGS_H_ */
