/**
 * \file MeshCore.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 07/01/2009
 * \author Denis Itskovich
 */


#ifndef MESHCORE_H_
#define MESHCORE_H_

extern "C"
{
	#include "../../MeshCore/inc/Station.h"
	#include "../../MeshCore/inc/Simulator.h"
	#include "../../MeshCore/inc/Settings.h"
}

#include "MeshException.h"

#define CHECK(res) \
	do { \
		EStatus status = res; \
		if (status != eSTATUS_COMMON_OK) \
		{ \
			throw new MeshException("MeshCore failure: %s (%s:%d)", GetStatusMessage(status), __FILE__, __LINE__); \
		} \
	} while (0)

#endif /* MESHCORE_H_ */
