/**
 * \file MeshTreeItemStation.cpp
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 13/01/2009
 * \author Denis Itskovich
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
	m_bLastState = isActive();
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

	QTreeWidgetItem* routeItem = new QTreeWidgetItem(QStringList() << "Routing");
	routeItem->setIcon(0, folderIcon);
	QTreeWidgetItem* scheduleItem = new QTreeWidgetItem(QStringList() << "Schedule");
	scheduleItem->setIcon(0, folderIcon);

	addChild(locItem);
	addChild(velItem);
	addChild(routeItem);
	addChild(scheduleItem);

	updateStation();
}

void MeshTreeItemStation::updateIcon()
{
	if (m_bLastState) setIcon(0, QIcon(":/station.png"));
	else setIcon(0, QIcon(":/disabledstation.png"));
}

void MeshTreeItemStation::updateStation()
{
	if (!isLocationValid()) initLocationNode(child(0));
	if (!isVelocityValid()) initVelocityNode(child(1));

	if (m_bLastState != isActive())
	{
		m_bLastState = isActive();
		updateIcon();
	}

	MeshItemStation::updateStation();
}

void MeshTreeItemStation::initLocationNode(QTreeWidgetItem* item)
{
	QPointF loc = location();
	item->setText(1, QString("(%1, %2)").arg(loc.x()).arg(loc.y()));
	item->child(0)->setText(1, QString("%1").arg(loc.x()));
	item->child(1)->setText(1, QString("%1").arg(loc.y()));
}

void MeshTreeItemStation::initVelocityNode(QTreeWidgetItem* item)
{
	QPointF vel = velocity();
	item->setText(1, QString("(%1, %2)").arg(vel.x()).arg(vel.y()));
	item->child(0)->setText(1, QString("%1").arg(vel.x()));
	item->child(1)->setText(1, QString("%1").arg(vel.y()));
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
	QString brief = QString("(at: %1, to: Station %2, size: %3 bytes)").arg(time).arg(pPacket->originalDstId).arg(pPacket->size);
	QTreeWidgetItem* scheduleItem = new QTreeWidgetItem(QStringList() << "Packet" << brief);
	scheduleItem->addChild(new QTreeWidgetItem(QStringList() << "time" << QString("%1").arg(time)));
	scheduleItem->addChild(new QTreeWidgetItem(QStringList() << "destination" << QString("Station %1").arg(pPacket->originalDstId)));
	scheduleItem->addChild(new QTreeWidgetItem(QStringList() << "size" << QString("%1 bytes").arg(pPacket->size)));
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
	unsigned seqNum = pPacket->sequenceNum;
	assert(m_scheduleMap.count(seqNum) == 0);
	QTreeWidgetItem* item = createScheduleItem(time, pPacket);
	m_scheduleMap[seqNum] = item;
	child(3)->addChild(item);
	child(3)->setText(1, QString("(%1 entries)").arg(m_scheduleMap.count()));
}

void MeshTreeItemStation::removeScheduleEntry(const Packet* pPacket)
{
	unsigned seqNum = pPacket->sequenceNum;
	assert(m_scheduleMap.count(seqNum) != 0);
	delete m_scheduleMap[seqNum];
	m_scheduleMap.remove(seqNum);
	if (m_scheduleMap.isEmpty()) child(3)->setText(1, "(empty)");
	else child(3)->setText(1, QString("(%1 entries)").arg(m_scheduleMap.count()));
}

void MeshTreeItemStation::deliverScheduleEntry(const Packet* pPacket)
{

}

void MeshTreeItemStation::reset()
{
}
