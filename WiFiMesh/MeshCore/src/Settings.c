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
 * @file Settings.c
 *
 * Project: MeshCore
 * (C) Copyright 2009 Denis Itskovich
 *
 * @date 02/01/2009
 * @author Denis Itskovich
 */

#include "../inc/Settings.h"
#include "../inc/Macros.h"
#include <stdlib.h>
#include <math.h>

int __counter_Settings = 0;

#define SETTINGS_PROPERTY(name, type, initval)  IMPLEMENT_PROPERTY(Settings, name, type, initval)
#define SETTINGS_INIT(name)						INIT_PROPERTY(Settings, name, pThis)

struct _Settings
{
	double     propMaxAttenuation;
	double     propAttenuationCoefficient;
	unsigned   propDataRate;
	double     propRouteExpirationTimeout;
	double     propRouteRetryTimeout;
	int        propRouteRetryThreshold;
	int        propRoutingTableSize;
	double     propPacketRetryTimeout;
	int        propPacketRetryThreshold;
	int        propPacketHopsThreshold;
    int        propRelayBufferSize;
	Size       propWorldSize;
	double     propMaxDuration;
};

static const Size DEFAULT_WORLD_SIZE = {600, 400};

SETTINGS_PROPERTY(WorldSize, Size, DEFAULT_WORLD_SIZE);
SETTINGS_PROPERTY(DataRate, unsigned long, 65536);
SETTINGS_PROPERTY(RouteExpirationTimeout, double, 20.0);
SETTINGS_PROPERTY(RouteRetryTimeout, double, 2);
SETTINGS_PROPERTY(RouteRetryThreshold, int, 10);
SETTINGS_PROPERTY(RoutingTableSize, int, 64);
SETTINGS_PROPERTY(PacketRetryTimeout, double, 0.05);
SETTINGS_PROPERTY(PacketRetryThreshold, int, 5);
SETTINGS_PROPERTY(PacketHopsThreshold, int, 20);
SETTINGS_PROPERTY(RelayBufferSize, int, 65536);
SETTINGS_PROPERTY(MaxDuration, double, 60.0);
// SETTINGS_PROPERTY(Coverage, double, 100);
SETTINGS_PROPERTY(AttenuationCoefficient, double, 0.2);
SETTINGS_PROPERTY(MaxAttenuation, double, 2000);

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

	SETTINGS_INIT(DataRate);
	SETTINGS_INIT(WorldSize);
	SETTINGS_INIT(RouteExpirationTimeout);
	SETTINGS_INIT(RouteRetryTimeout);
    SETTINGS_INIT(RouteRetryThreshold);
    SETTINGS_INIT(RoutingTableSize);
    SETTINGS_INIT(PacketRetryTimeout);
	SETTINGS_INIT(PacketRetryThreshold);
	SETTINGS_INIT(PacketHopsThreshold);
	SETTINGS_INIT(RelayBufferSize);
    SETTINGS_INIT(MaxDuration);
    // SETTINGS_INIT(Coverage);
    SETTINGS_INIT(AttenuationCoefficient);
    SETTINGS_INIT(MaxAttenuation);

	return eSTATUS_COMMON_OK;
}

EStatus SettingsDestroy(Settings* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	return eSTATUS_COMMON_OK;
}

EStatus SettingsGetCoverage(const Settings* pThis, double* pCoverage)
{
    VALIDATE_ARGUMENTS(pThis && pCoverage);
    *pCoverage = sqrt(pThis->propMaxAttenuation / pThis->propAttenuationCoefficient);
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
