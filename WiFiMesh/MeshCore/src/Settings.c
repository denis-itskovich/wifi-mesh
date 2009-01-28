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

#define DEFAULT_COVERAGE	50
#define	DEFAULT_DATA_RATE	1048576
#define DEFAULT_ROUTE_TTL	2000

struct _Settings
{
	double		coverage;
	unsigned	dataRate;
	double		routeTTL;
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
	pThis->size.x = 400.0;
	pThis->size.y = 400.0;
	return eSTATUS_COMMON_OK;
}

EStatus SettingsDestroy(Settings* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	return eSTATUS_COMMON_OK;
}

EStatus SettingsGetTransmitTime(const Settings* pThis, const Message* pMessage, double* pTime)
{
	unsigned size;
	double variance;
	double resolution;
	VALIDATE_ARGUMENTS(pThis && pMessage && pTime);

	resolution = 1000000;
	size = sizeof(*pMessage) + pMessage->size;
	variance = (((double)(rand() % sizeof(*pMessage)) * resolution) + 1) / resolution;
	*pTime = ((double)(size) + variance) / pThis->dataRate;
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
