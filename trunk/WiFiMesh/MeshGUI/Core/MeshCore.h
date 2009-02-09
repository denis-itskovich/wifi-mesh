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
	#include "../../MeshCore/inc/Log.h"
	#include "../../MeshCore/inc/Packet.h"
}

#include "MeshException.h"
#include <QtGui>

#define CHECK(res) \
	do { \
		EStatus status = res; \
		if (status != eSTATUS_COMMON_OK) \
		{ \
			QMessageBox::critical(0, "MeshCore failure", QString(StatusGetMessage(status)) + "\nin function " + __PRETTY_FUNCTION__ ); \
		} \
	} while (0)

#endif /* MESHCORE_H_ */
