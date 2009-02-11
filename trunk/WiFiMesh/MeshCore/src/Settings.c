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

#define DEFAULT_COVERAGE		50.0
#define	DEFAULT_DATA_RATE		1048576
#define DEFAULT_ROUTE_TTL		20.0
#define DEFAULT_RETRY_TIMEOUT	0.5

struct _Settings
{
	double		coverage;
	unsigned	dataRate;
	double		routeTTL;
	double		retryTimeout;
	Size		size;
};

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
	pThis->coverage = DEFAULT_COVERAGE;
	pThis->dataRate = DEFAULT_DATA_RATE;
	pThis->routeTTL = DEFAULT_ROUTE_TTL;
	pThis->retryTimeout = DEFAULT_RETRY_TIMEOUT;
	pThis->size.x = 400.0;
	pThis->size.y = 400.0;
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
    *pTime = ((double)size) / pThis->dataRate;
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
    *pTime += variance / pThis->dataRate;
    return eSTATUS_COMMON_OK;
}

EStatus SettingsGetRoutingTTL(const Settings* pThis, double* pTTL)
{
	GET_MEMBER(pTTL, pThis, routeTTL);
}

EStatus SettingsGetCoverage(const Settings* pThis, double* pCoverage)
{
	GET_MEMBER(pCoverage, pThis, coverage);
}

EStatus SettingsGetDataRate(const Settings* pThis, unsigned long* pDataRate)
{
	GET_MEMBER(pDataRate, pThis, dataRate);
}

EStatus SettingsGetWorldSize(Settings* pThis, Size* pSize)
{
	GET_MEMBER(pSize, pThis, size);
}

EStatus SettingsGetRetryTimeout(Settings* pThis, double* pTimeout)
{
	GET_MEMBER(pTimeout, pThis, retryTimeout);
}

EStatus SettingsSetRoutingTTL(Settings* pThis, double ttl)
{
	SET_MEMBER(ttl, pThis, routeTTL);
}

EStatus SettingsSetCoverage(Settings* pThis, double coverage)
{
	SET_MEMBER(coverage, pThis, coverage);
}

EStatus SettingsSetDataRate(Settings* pThis, unsigned long dataRate)
{
	SET_MEMBER(dataRate, pThis, dataRate);
}

EStatus SettingsSetWorldSize(Settings* pThis, Size size)
{
	SET_MEMBER(size, pThis, size);
}

EStatus SettingsSetRetryTimeout(Settings* pThis, double timeout)
{
	SET_MEMBER(timeout, pThis, retryTimeout);
}
