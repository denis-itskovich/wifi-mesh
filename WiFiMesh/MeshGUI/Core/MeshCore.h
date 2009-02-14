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
 * @file MeshCore.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * @date 07/01/2009
 * @author Denis Itskovich
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
