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
 * @file MeshTreeItemStation.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * @date 13/01/2009
 * @author Denis Itskovich
 */


#ifndef MESHTREEITEMSTATION_H_
#define MESHTREEITEMSTATION_H_

#include "MeshItemStation.h"
#include <QtGui/QtGui>

class MeshTreeItemStation : public MeshItemStation, public QTreeWidgetItem
{
	typedef QMap<StationId, QTreeWidgetItem*> 	    RouteMap;
	typedef QMap<const Packet*, QTreeWidgetItem*>	ScheduleMap;

public:
	MeshTreeItemStation(MeshViewStations* pContainer, Station* pStation);

protected:
    virtual void addRouteEntry(StationId dst, StationId trans, double expires, int length);
    virtual void addRouteEntry(StationId dst, double nextRetry, int retriesLeft);
    virtual void updateRouteEntry(StationId dst, StationId trans, double expires, int length);
    virtual void updateRouteEntry(StationId dst, double nextRetry, int retriesLeft);
	virtual void removeRouteEntry(StationId dst);

	virtual void addScheduleEntry(double time, const Packet* pPacket);
	virtual void removeScheduleEntry(const Packet* pPacket);
	virtual void deliverScheduleEntry(const Packet* pPacket);
	virtual void updateStation();

private:
	void initLocationItem();
	void initVelocityItem();
	void updateIcon();

	void init();

	void updateRouteItem(StationId dst, QTreeWidgetItem* item);
	QTreeWidgetItem* createRouteItem(StationId dst, StationId transit, double expires, int length);
    QTreeWidgetItem* createPendingItem(StationId dst, double nextRetry, int retriesLeft);
	QTreeWidgetItem* createScheduleItem(double time, const Packet* pPacket);

	RouteMap           m_routeMap;
	ScheduleMap        m_scheduleMap;
	bool               m_isActive;
	bool               m_isTransmitting;
	QTreeWidgetItem*   m_itemBuffer;
	QTreeWidgetItem*   m_itemLocation;
	QTreeWidgetItem*   m_itemVelocity;
	QTreeWidgetItem*   m_itemRouting;
	QTreeWidgetItem*   m_itemSchedule;
	QIcon              m_iconTransmitting;
	QIcon              m_iconActive;
	QIcon              m_iconInactive;
};

#endif /* MESHTREEITEMSTATION_H_ */
