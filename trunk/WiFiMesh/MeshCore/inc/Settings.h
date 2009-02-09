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
#include "Packet.h"

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

/** Sets world size
 * \param pThis [in] pointer to instance
 * \param size [in] new world size
 */
EStatus SettingsSetWorldSize(Settings* pThis, Size size);

/** Sets search retry timeout
 * \param pThis [in] pointer to instance
 * \param timeout [in] new timeout
 */
EStatus SettingsSetRetryTimeout(Settings* pThis, double timeout);

/** Calculates transmit time for given packet
 * \param pThis [in] pointer to instance
 * \param pPacket [in] pointer to packet
 * \param pTime [out] transmit time will be stored at *pTime
 */
EStatus SettingsGetTransmitTime(const Settings* pThis, const Packet* pPacket, double* pTime);

/** Calculates silence time for given packet
 * \param pThis [in] pointer to instance
 * \param pPacket [in] pointer to packet
 * \param pTime [out] transmit time will be stored at *pTime
 */
EStatus SettingsGetSilenceTime(const Settings* pThis, const Packet* pPacket, double* pTime);

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

/** Retrieves world size
 * \param pThis [in] pointer to instance
 * \param pSize [out] size will be stored at *pSize
 */
EStatus SettingsGetWorldSize(Settings* pThis, Size* pSize);

/** Retrieves search retry timeout
 * \param pThis [in] pointer to instance
 * \param pTimeout [out] timeout will be stored at *pTimeout
 */
EStatus SettingsGetRetryTimeout(Settings* pThis, double* pTimeout);

#endif /* SETTINGS_H_ */
