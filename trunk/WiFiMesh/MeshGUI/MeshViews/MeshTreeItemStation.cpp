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
	QTreeWidgetItem* locItem = new QTreeWidgetItem(QStringList() << "Location");
	locItem->addChild(new QTreeWidgetItem(QStringList() << "x"));
	locItem->addChild(new QTreeWidgetItem(QStringList() << "y"));

	QTreeWidgetItem* velItem = new QTreeWidgetItem(QStringList() << "Velocity");
	velItem->addChild(new QTreeWidgetItem(QStringList() << "x"));
	velItem->addChild(new QTreeWidgetItem(QStringList() << "y"));

	QTreeWidgetItem* routeItem = new QTreeWidgetItem(QStringList() << "Routing");
	QTreeWidgetItem* scheduleItem = new QTreeWidgetItem(QStringList() << "Schedule");

	addChild(locItem);
	addChild(velItem);
	addChild(routeItem);
	addChild(scheduleItem);

	updateStation();
}

void MeshTreeItemStation::updateStation()
{
	if (!isLocationValid()) initLocationNode(child(0));
	if (!isVelocityValid()) initVelocityNode(child(1));
	if (!isRoutingValid()) initRoutingNode(child(2));
	if (!isSchedulerValid()) initSchedulerNode(child(3));
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

void MeshTreeItemStation::initRoutingNode(QTreeWidgetItem* item)
{
	item->takeChildren();

	const RouteMap& routeMap = routing();
	for (RouteMap::const_iterator i = routeMap.begin(); i != routeMap.end(); ++i)
	{
		QString name = QString("Station %1").arg(i->destination);
		QTreeWidgetItem* routeItem = NULL;
		if (i->transit != INVALID_STATION_ID)
		{
			QString brief = QString("(to: %1 via: Station %2, hops: %3, expires at: %4)").arg(name).arg(i->transit).arg(i->length).arg(i->expires);
			routeItem = new QTreeWidgetItem(QStringList() << name << brief);
			routeItem->addChild(new QTreeWidgetItem(QStringList() << "destination" << QString("%1").arg(i->destination)));
			routeItem->addChild(new QTreeWidgetItem(QStringList() << "transit" << QString("%1").arg(i->transit)));
			routeItem->addChild(new QTreeWidgetItem(QStringList() << "hops" << QString("%1").arg(i->length)));
			routeItem->addChild(new QTreeWidgetItem(QStringList() << "expiration" << QString("%1").arg(i->expires)));

		}
		else
		{
			QString brief = QString("(pending to: %1, retry at: %2)").arg(name).arg(i->expires);
			routeItem = new QTreeWidgetItem(QStringList() << name << brief);
			routeItem->addChild(new QTreeWidgetItem(QStringList() << "destination" << QString("%1").arg(i->destination)));
			routeItem->addChild(new QTreeWidgetItem(QStringList() << "retry" << QString("%1").arg(i->expires)));
		}
		item->addChild(routeItem);
	}

	int count = routeMap.count();
	item->setText(1, (count > 0) ? QString("(%1 entries)").arg(count) : "(empty)");
}

void MeshTreeItemStation::initSchedulerNode(QTreeWidgetItem* item)
{
	item->takeChildren();

	const ScheduleList& scheduleList = schedule();
	for (ScheduleList::const_iterator i = scheduleList.begin(); i != scheduleList.end(); ++i)
	{
		QString brief = QString("(at: %1, to: Station %2, size: %3 bytes)").arg(i->time).arg(i->destination).arg(i->size);
		QTreeWidgetItem* scheduleItem = new QTreeWidgetItem(QStringList() << "Message" << brief);
		scheduleItem->addChild(new QTreeWidgetItem(QStringList() << "time" << QString("%1").arg(i->time)));
		scheduleItem->addChild(new QTreeWidgetItem(QStringList() << "destination" << QString("Station %1").arg(i->destination)));
		scheduleItem->addChild(new QTreeWidgetItem(QStringList() << "size" << QString("%1 bytes").arg(i->size)));
		item->addChild(scheduleItem);
	}

	int count = scheduleList.count();
	item->setText(1, (count > 0) ? QString("(%1 messages)").arg(count) : "(empty)");
}
