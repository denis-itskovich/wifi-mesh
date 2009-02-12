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
 * \file Settings.c
 *
 * Project: MeshCore
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 02/01/2009
 * \author Denis Itskovich
 */

#include "Settings.h"
#include "Macros.h"
#include <stdlib.h>

#define SETTINGS_PROPERTY(...)  IMPLEMENT_PROPERTY(Settings, ## __VA_ARGS__)
#define SETTINGS_INIT(name)     INIT_PROPERTY(Settings, name, pThis)

struct _Settings
{
	double     propCoverage;
	unsigned   propDataRate;
	double     propRouteExpirationTimeout;
	double     propRouteRetryTimeout;
	double     propPacketRetryTimeout;
	int        propPacketRetryThreshold;
	int        propPacketHopsThreshold;
	Size       propWorldSize;
};

SETTINGS_PROPERTY(Coverage, double, 75);
SETTINGS_PROPERTY(DataRate, unsigned long, 1048576);

static const Size DEFAULT_WorldSize_VALUE = {600, 400};
EStatus SettingsGetWorldSize(const Settings* pThis, Size* pValue) { GET_MEMBER(pValue, pThis, propWorldSize); }
EStatus SettingsSetWorldSize(Settings* pThis, Size value) { SET_MEMBER(value, pThis, propWorldSize); }
EStatus SettingsInitWorldSize(Settings* pThis) { return SettingsSetWorldSize(pThis, DEFAULT_WorldSize_VALUE); }


//SETTINGS_PROPERTY(WorldSize, Size, {600, 400});
SETTINGS_PROPERTY(RouteExpirationTimeout, double, 20.0);
SETTINGS_PROPERTY(RouteRetryTimeout, double, 0.5);
SETTINGS_PROPERTY(PacketRetryTimeout, double, 0.01);
SETTINGS_PROPERTY(PacketRetryThreshold, int, 5);
SETTINGS_PROPERTY(PacketHopsThreshold, int, 20);

EStatus SettingsNew(Settings** ppThis)
{
	CONSTRUCT(ppThis, Settings);
}

EStatus SettingsDelete(Settings** ppThis)
{
	DESTRUCT(ppThis, Settings);
}

EStatus SettingsInit(Settings* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	CLEAR(pThis);

	SETTINGS_INIT(Coverage);
	SETTINGS_INIT(DataRate);
	SETTINGS_INIT(WorldSize);
	SETTINGS_INIT(RouteExpirationTimeout);
	SETTINGS_INIT(RouteRetryTimeout);
	SETTINGS_INIT(PacketRetryTimeout);
	SETTINGS_INIT(PacketRetryThreshold);
	SETTINGS_INIT(PacketHopsThreshold);

	return eSTATUS_COMMON_OK;
}

EStatus SettingsDestroy(Settings* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	return eSTATUS_COMMON_OK;
}

EStatus SettingsGetTransmitTime(const Settings* pThis, const Packet* pPacket, double* pTime)
{
    unsigned size;
    CHECK(PacketGetSize(pPacket, &size));
    *pTime = ((double)size) / pThis->propDataRate;
    return eSTATUS_COMMON_OK;
}

EStatus SettingsGetSilenceTime(const Settings* pThis, const Packet* pPacket, double* pTime)
{
    unsigned size;
    double variance;
    double resolution;
    VALIDATE_ARGUMENTS(pThis && pPacket && pTime);

    CHECK(PacketGetSize(pPacket, &size));
    CHECK(SettingsGetTransmitTime(pThis, pPacket, pTime));
    resolution = RAND_MAX;
    variance = (((double)(rand() % sizeof(pPacket->header)) * resolution) + 1) / resolution / 2.0;
    *pTime += variance / pThis->propDataRate;
    return eSTATUS_COMMON_OK;
}
