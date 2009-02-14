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
 * @file MeshTreeItemStation.cpp
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * @date 13/01/2009
 * @author Denis Itskovich
 */

#include "MeshTreeItemStation.h"

MeshTreeItemStation::MeshTreeItemStation(MeshViewStations* pContainer, Station* pStation) :
	MeshItemStation(pContainer, pStation),
	QTreeWidgetItem(QStringList() << name())
{
	init();
}

void MeshTreeItemStation::init()
{
	m_isActive = !isActive();
	m_isTransmitting = !isTransmitting();
	updateIcon();

	QIcon folderIcon(QIcon(":/folder.png"));
	QTreeWidgetItem* locItem = new QTreeWidgetItem(QStringList() << "Location");
	locItem->addChild(new QTreeWidgetItem(QStringList() << "x"));
	locItem->addChild(new QTreeWidgetItem(QStringList() << "y"));
	locItem->setIcon(0, folderIcon);

	QTreeWidgetItem* velItem = new QTreeWidgetItem(QStringList() << "Velocity");
	velItem->addChild(new QTreeWidgetItem(QStringList() << "x"));
	velItem->addChild(new QTreeWidgetItem(QStringList() << "y"));
	velItem->setIcon(0, folderIcon);

	QTreeWidgetItem* routeItem = new QTreeWidgetItem(QStringList() << "Routing" << "(empty)");
	routeItem->setIcon(0, folderIcon);
	QTreeWidgetItem* scheduleItem = new QTreeWidgetItem(QStringList() << "Schedule" << "(empty)");
	scheduleItem->setIcon(0, folderIcon);

	addChild(locItem);
	addChild(velItem);
	addChild(routeItem);
	addChild(scheduleItem);

	updateStation();
}

void MeshTreeItemStation::updateIcon()
{
    if ((m_isActive == isActive()) &&
        (m_isTransmitting == isTransmitting())) return;

    m_isActive = isActive();
    m_isTransmitting = isTransmitting();

    QIcon icon;
    QColor color;

	if (m_isActive)
    {
	    if (m_isTransmitting)
        {
	        icon = QIcon(":/txstation.png");
	        color = Qt::red;
        }
	    else
        {
	        icon = QIcon(":/station.png");
	        color = Qt::black;
        }
    }
	else
    {
	    icon = QIcon(":/disabledstation.png");
	    color = Qt::gray;
    }

	setIcon(0, icon);
	this->setForeground(0, QBrush(color));
}

void MeshTreeItemStation::updateStation()
{
	initLocationNode(child(0));
    initVelocityNode(child(1));

    updateIcon();

	MeshItemStation::updateStation();
}

void MeshTreeItemStation::initLocationNode(QTreeWidgetItem* item)
{
	QPointF loc = location();
	item->setText(1, locationString());
	item->child(0)->setText(1, QString("%1").arg(loc.x(), 0, 'f', 2));
	item->child(1)->setText(1, QString("%1").arg(loc.y(), 0, 'f', 2));
}

void MeshTreeItemStation::initVelocityNode(QTreeWidgetItem* item)
{
	QPointF vel = velocity();
	item->setText(1, velocityString());
	item->child(0)->setText(1, QString("%1").arg(vel.x(), 0, 'f', 2));
	item->child(1)->setText(1, QString("%1").arg(vel.y(), 0, 'f', 2));
}

QTreeWidgetItem* MeshTreeItemStation::createRouteItem(StationId dst, StationId transit, double expires, int length)
{
	QString name = QString("Station %1").arg(dst);
	QTreeWidgetItem* routeItem = NULL;
	if (transit != INVALID_STATION_ID)
	{
		QString brief = QString("(to: %1 via: Station %2, hops: %3, expires at: %4)").arg(name).arg(transit).arg(length).arg(expires);
		routeItem = new QTreeWidgetItem(QStringList() << name << brief);
		routeItem->addChild(new QTreeWidgetItem(QStringList() << "destination" << QString("%1").arg(name)));
		routeItem->addChild(new QTreeWidgetItem(QStringList() << "transit" << QString("Station %1").arg(transit)));
		routeItem->addChild(new QTreeWidgetItem(QStringList() << "hops" << QString("%1").arg(length)));
		routeItem->addChild(new QTreeWidgetItem(QStringList() << "expiration" << QString("%1").arg(expires)));
		routeItem->setIcon(0, QIcon(":/connected.png"));

	}
	else
	{
		QString brief = QString("(pending to: %1, retry at: %2)").arg(name).arg(expires);
		routeItem = new QTreeWidgetItem(QStringList() << name << brief);
		routeItem->addChild(new QTreeWidgetItem(QStringList() << "destination" << QString("%1").arg(name)));
		routeItem->addChild(new QTreeWidgetItem(QStringList() << "retry" << QString("%1").arg(expires)));
		routeItem->setIcon(0, QIcon(":/disconnected.png"));
	}
	return routeItem;
}

QTreeWidgetItem* MeshTreeItemStation::createScheduleItem(double time, const Packet* pPacket)
{
	QString brief = QString("(at: %1, to: Station %2, size: %3 bytes)").arg(time).arg(pPacket->header.originalDstId).arg(pPacket->payload.size);
	QTreeWidgetItem* scheduleItem = new QTreeWidgetItem(QStringList() << "Packet" << brief);
	scheduleItem->addChild(new QTreeWidgetItem(QStringList() << "time" << QString("%1").arg(time)));
	scheduleItem->addChild(new QTreeWidgetItem(QStringList() << "destination" << QString("Station %1").arg(pPacket->header.originalDstId)));
	scheduleItem->addChild(new QTreeWidgetItem(QStringList() << "size" << QString("%1 bytes").arg(pPacket->payload.size)));
	scheduleItem->setIcon(0, QIcon(":/packet.png"));
	return scheduleItem;
}

void MeshTreeItemStation::addRouteEntry(StationId dst, StationId trans, double expires, int length)
{
	assert(m_routeMap.count(dst) == 0);
	QTreeWidgetItem* item = createRouteItem(dst, trans, expires, length);
	m_routeMap[dst] = item;
	child(2)->addChild(item);
	child(2)->setText(1, QString("(%1 entries)").arg(m_routeMap.count()));
}

void MeshTreeItemStation::updateRouteEntry(StationId dst, StationId trans, double expires, int length)
{
	assert(m_routeMap.count(dst) != 0);
	QTreeWidgetItem* item = m_routeMap[dst];
	int index = child(2)->indexOfChild(item);
	delete item;
	item = createRouteItem(dst, trans, expires, length);
	m_routeMap[dst] = item;
	child(2)->insertChild(index, item);
}

void MeshTreeItemStation::removeRouteEntry(StationId dst)
{
	assert(m_routeMap.count(dst) != 0);
	delete m_routeMap[dst];
	m_routeMap.remove(dst);
	if (m_routeMap.isEmpty()) child(2)->setText(1, "(empty)");
	else child(2)->setText(1, QString("(%1 entries)").arg(m_routeMap.count()));
}

void MeshTreeItemStation::addScheduleEntry(double time, const Packet* pPacket)
{
	assert(m_scheduleMap.count(pPacket) == 0);
	QTreeWidgetItem* item = createScheduleItem(time, pPacket);
	m_scheduleMap[pPacket] = item;
	child(3)->addChild(item);
	child(3)->setText(1, QString("(%1 entries)").arg(m_scheduleMap.count()));
}

void MeshTreeItemStation::removeScheduleEntry(const Packet* pPacket)
{
	assert(m_scheduleMap.count(pPacket) != 0);
	delete m_scheduleMap[pPacket];
	m_scheduleMap.remove(pPacket);
	if (m_scheduleMap.isEmpty()) child(3)->setText(1, "(empty)");
	else child(3)->setText(1, QString("(%1 entries)").arg(m_scheduleMap.count()));
}

void MeshTreeItemStation::deliverScheduleEntry(const Packet* pPacket)
{

}

