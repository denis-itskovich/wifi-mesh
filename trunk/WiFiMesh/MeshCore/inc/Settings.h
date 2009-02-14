/*********************************************************************************
WiFi Mesh Simulator PRO
Copyright (C) 2009 Denis Itskovich

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*********************************************************************************/

/**
 * @file Settings.h
 *
 * Project: MeshCore
 * (C) Copyright 2008 Denis Itskovich
 *
 * @date 22/12/2008
 * @author Denis Itskovich
 */


#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "Status.h"
#include "Packet.h"

typedef struct _Settings Settings;	///< forward declaration

/** Allocates and initializes new instance
 * @param ppThis [out] new instance will be stored at *ppThis
 */
EStatus SettingsNew(Settings** ppThis);

/** Destroys and deallocates an instance
 * @param ppThis [in, out] *ppThis should point to valid instance
 */
EStatus SettingsDelete(Settings** ppThis);

/** Initializes an instance
 * @param pThis [in] pointer to instance
 */
EStatus SettingsInit(Settings* pThis);

/** Destroys an instance
 * @param pThis [in] pointer to instance
 */
EStatus SettingsDestroy(Settings* pThis);

/** Sets coverage
 * @param pThis [in] pointer to instance
 * @param coverage [in] new coverage radius
 */
EStatus SettingsSetCoverage(Settings* pThis, double coverage);

/** Retrieves coverage
 * @param pThis [in] pointer to instance
 * @param pCoverage [out] coverage will be stored at *pCoverage
 */
EStatus SettingsGetCoverage(const Settings* pThis, double* pCoverage);

/** Sets data rate
 * @param pThis [in] pointer to instance
 * @param dataRate [in] data rate
 */
EStatus SettingsSetDataRate(Settings* pThis, unsigned long dataRate);

/** Retrieves data rate
 * @param pThis [in] pointer to instance
 * @param pDataRate [out] data rate will be stored at *pDataRate
 */
EStatus SettingsGetDataRate(const Settings* pThis, unsigned long* pDataRate);

/** Sets world size
 * @param pThis [in] pointer to instance
 * @param size [in] new world size
 */
EStatus SettingsSetWorldSize(Settings* pThis, Size size);

/** Retrieves world size
 * @param pThis [in] pointer to instance
 * @param pSize [out] size will be stored at *pSize
 */
EStatus SettingsGetWorldSize(const Settings* pThis, Size* pSize);

/** Sets route table entry expiration timeout
 * @param pThis [in] pointer to instance
 * @param timeout [in] new expiration timeout
 */
EStatus SettingsSetRouteExpirationTimeout(Settings* pThis, double timeout);

/** Retrieves route table entry expiration timeout
 * @param pThis [in] pointer to instance
 * @param pTimeout [out] time to live will be stored at *pTimeout
 */
EStatus SettingsGetRouteExpirationTimeout(const Settings* pThis, double* pTimeout);

/** Sets search retry timeout
 * @param pThis [in] pointer to instance
 * @param timeout [in] new route search retry timeout
 */
EStatus SettingsSetRouteRetryTimeout(Settings* pThis, double timeout);

/** Retrieves search retry timeout
 * @param pThis [in] pointer to instance
 * @param pTimeout [out] timeout will be stored at *pTimeout
 */
EStatus SettingsGetRouteRetryTimeout(const Settings* pThis, double* pTimeout);

/** Sets Tx retry timeout
 * @param pThis [in] pointer to instance
 * @param timeout [in] new packet retry timeout
 */
EStatus SettingsSetPacketRetryTimeout(Settings* pThis, double timeout);

/** Retrieves Tx retry timeout
 * @param pThis [in] pointer to instance
 * @param pTimeout [out] timeout will be stored at *pTimeout
 */
EStatus SettingsGetPacketRetryTimeout(const Settings* pThis, double* pTimeout);

/** Sets maximum retries count
 * @param pThis [in] pointer to instance
 * @param retries [in] new retries threshold
 */
EStatus SettingsSetPacketRetryThreshold(Settings* pThis, int retries);

/** Retrieves maximum retries count
 * @param pThis [in] pointer to settings instance
 * @param pRetries [out] retries threshold will be stored at *pRetries
 */
EStatus SettingsGetPacketRetryThreshold(const Settings* pThis, int* pRetries);

/** Sets maximum packet hops count
 * @param pThis [in] pointer to settings instance
 * @param hops [in] new hops count threshold
 */
EStatus SettingsSetPacketHopsThreshold(Settings* pThis, int hops);

/** Retrieves maximum packet hops count
 * @param pThis [in] pointer to settings instance
 * @param pHops [out] hops count threshold will be stored at *pHops
 */
EStatus SettingsGetPacketHopsThreshold(const Settings* pThis, int* pHops);

/** Calculates transmit time for given packet
 * @param pThis [in] pointer to instance
 * @param pPacket [in] pointer to packet
 * @param pTime [out] transmit time will be stored at *pTime
 */
EStatus SettingsGetTransmitTime(const Settings* pThis, const Packet* pPacket, double* pTime);

/** Calculates silence time for given packet
 * @param pThis [in] pointer to instance
 * @param pPacket [in] pointer to packet
 * @param pTime [out] transmit time will be stored at *pTime
 */
EStatus SettingsGetSilenceTime(const Settings* pThis, const Packet* pPacket, double* pTime);

#endif /* SETTINGS_H_ */
