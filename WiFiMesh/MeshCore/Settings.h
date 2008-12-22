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

typedef struct _Settings Settings;	///< forward declaration

/** Allocates and initializes new instance
 * \param ppThis [out] new instance will be stored at *ppThis
 * \param coverage [in] station coverage range, in length units
 * \param dataRate [in] data rate, in bits per time unit
 * \param routeTTL [in] routing entry time to live
 */
EStatus SettingsNew(Settings** ppThis, double coverage, unsigned dataRate, double routeTTL);

/** Destroys and deallocates an instance
 * \param ppThis [in, out] *ppThis should point to valid instance
 */
EStatus SettingsDelete(Settings** ppThis);

/** Initizes an instance
 * \param pThis [in] pointer to instance
 * \param coverage [in] station coverage range, in length units
 * \param dataRate [in] data rate, in bits per time unit
 * \param routeTTL [in] routing entry time to live
 */
EStatus SettingsInit(Settings* pThis, double coverage, unsigned dataRate, double routeTTL);

/** Destroys an instance
 * \param pThis [in] pointer to instance
 */
EStatus SettingsDestroy(Settings* pThis);

#endif /* SETTINGS_H_ */
