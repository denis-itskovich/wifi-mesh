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

struct _Settings
{
	double		coverage;
	unsigned	dataRate;
	double		routeTTL;
};

EStatus SettingsNew(Settings** ppThis, double coverage, unsigned dataRate, double routeTTL)
{
	CONSTRUCT(ppThis, Settings, coverage, dataRate, routeTTL);
}

EStatus SettingsDelete(Settings** ppThis)
{
	DESTRUCT(ppThis, Settings);
}

EStatus SettingsInit(Settings* pThis, double coverage, unsigned dataRate, double routeTTL)
{
	VALIDATE_ARGUMENTS(pThis && (coverage > 0) && (dataRate > 0) && (routeTTL > 0));
	pThis->coverage = coverage;
	pThis->dataRate = dataRate;
	pThis->routeTTL = routeTTL;
	return eSTATUS_COMMON_OK;
}

EStatus SettingsDestroy(Settings* pThis)
{
	VALIDATE_ARGUMENTS(pThis);
	return eSTATUS_COMMON_OK;
}

EStatus SettingsGetTransmitTime(Settings* pThis, Message* pMessage, double* pTime)
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

EStatus SettingsGetRoutingTTL(Settings* pThis, double* pTTL)
{
	GET_MEMBER(pTTL, pThis, routeTTL);
}

EStatus SettingsGetCoverage(Settings* pThis, double* pCoverage)
{
	GET_MEMBER(pCoverage, pThis, coverage);
}
