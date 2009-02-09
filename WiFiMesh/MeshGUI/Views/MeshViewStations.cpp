/*
 * Stations.cpp
 *
 *  Created on: 20/08/2008
 *      Author: denis
 */

#include "MeshViewStations.h"

MeshViewStations::MeshViewStations(QWidget* parent) :
	MeshView(parent)
{
}

MeshViewStations::~MeshViewStations()
{
}

void MeshViewStations::registerStation(Station* pStation, MeshItemStation* pItem)
{
	m_stationToItem[pStation] = pItem;
}

void MeshViewStations::unregisterStation(Station* pStation)
{
	m_stationToItem.remove(pStation);
}

void MeshViewStations::setDocument(MeshDocument* doc)
{
	disconnect(doc);

	connect(this, SIGNAL(currentChanged(Station*)), doc, SLOT(setCurrentStation(Station*)));
	connect(this, SIGNAL(stationChanged(Station*)), doc, SLOT(updateStation(Station*)));
	connect(doc, SIGNAL(currentStationChanged(Station*)), this, SLOT(setCurrent(Station*)));

	connect(doc, SIGNAL(stationAdded(Station*)), this, SLOT(addStation(Station*)));
	connect(doc, SIGNAL(stationRemoved(Station*)), this, SLOT(removeStation(Station*)));
	connect(doc, SIGNAL(stationUpdated(Station*)), this, SLOT(updateStation(Station*)));

	connect(doc, SIGNAL(routeEntryAdded(const Station*, StationId, StationId, double, int)), this, SLOT(addRouteEntry(const Station*, StationId, StationId, double, int)));
	connect(doc, SIGNAL(routeEntryUpdated(const Station*, StationId, StationId, double, int)), this, SLOT(updateRouteEntry(const Station*, StationId, StationId, double, int)));
	connect(doc, SIGNAL(routeEntryExpired(const Station*, StationId)), this, SLOT(removeRouteEntry(const Station*, StationId)));

	connect(doc, SIGNAL(scheduleEntryAdded(const Station*, double, const Message*)), this, SLOT(addScheduleEntry(const Station*, double, const Message*)));
	connect(doc, SIGNAL(scheduleEntryRemoved(const Station*, double, const Message*)), this, SLOT(removeScheduleEntry(const Station*, double, const Message*)));

	connect(doc, SIGNAL(worldChanged()), this, SLOT(updateAll()));

	MeshView::setDocument(doc);
}

bool MeshViewStations::isCurrent(const MeshItemStation* item) const
{
	return document()->currentStation() == item->station();
}

void MeshViewStations::updateItem(MeshItemStation* item)
{
	emit stationChanged(item->station());
}

void MeshViewStations::updateAll()
{
	for (StationToItem::const_iterator i = m_stationToItem.begin(); i != m_stationToItem.end(); ++i)
	{
		i.value()->updateStation();
	}
}

void MeshViewStations::addStation(Station* pStation)
{
}

void MeshViewStations::removeStation(Station* pStation)
{
	unregisterStation(pStation);
}

void MeshViewStations::setCurrent(Station* pStation)
{
}

void MeshViewStations::updateStation(Station* pStation)
{
	MeshItemStation* item = findItem(pStation);
	if (item) item->updateStation();
}

MeshItemStation* MeshViewStations::currentItem() const
{
	return findItem(document()->currentStation());
}

void MeshViewStations::currentChanged(MeshItemStation* item)
{
	emit currentChanged(item->station());
}

MeshItemStation* MeshViewStations::findItem(const Station* pStation) const
{
	StationToItem::const_iterator i = m_stationToItem.find((Station*)pStation);
	if (i != m_stationToItem.end()) return *i;
	return NULL;
}

void MeshViewStations::addRouteEntry(const Station* pStation, StationId dst, StationId trans, double expires, int length)
{
	findItem(pStation)->addRouteEntry(dst, trans, expires, length);
}

void MeshViewStations::updateRouteEntry(const Station* pStation, StationId dst, StationId trans, double expires, int length)
{
	findItem(pStation)->updateRouteEntry(dst, trans, expires, length);
}

void MeshViewStations::removeRouteEntry(const Station* pStation, StationId dst)
{
	findItem(pStation)->removeRouteEntry(dst);
}

void MeshViewStations::addScheduleEntry(const Station* pStation, double time, const Message* pMessage)
{
	findItem(pStation)->addScheduleEntry(time, pMessage);
}

void MeshViewStations::removeScheduleEntry(const Station* pStation, double time, const Message* pMessage)
{
	findItem(pStation)->removeScheduleEntry(time, pMessage);
}
