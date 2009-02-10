/*
 * Stations.cpp
 *
 *  Created on: 20/08/2008
 *      Author: denis
 */

#include "MeshViewStations.h"

MeshViewStations::MeshViewStations(QWidget* parent) :
	MeshView(parent),
	m_currentItem(NULL)
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

	connect(doc, SIGNAL(scheduleEntryAdded(const Station*, double, const Packet*)), this, SLOT(addScheduleEntry(const Station*, double, const Packet*)));
	connect(doc, SIGNAL(scheduleEntryDelivered(const Station*, const Packet*)), SLOT(deliverScheduleEntry(const Station*, const Packet*)));
	connect(doc, SIGNAL(scheduleEntryRemoved(const Station*, const Packet*)), this, SLOT(removeScheduleEntry(const Station*, const Packet*)));

	connect(doc, SIGNAL(worldChanged()), this, SLOT(updateAll()));
    connect(doc, SIGNAL(beginTransmit(const Station*, const Station*, const Packet*)), this, SLOT(beginTransmit(const Station*, const Station*, const Packet*)));
    connect(doc, SIGNAL(endTransmit(const Station*)), this, SLOT(endTransmit(const Station*)));


	MeshView::setDocument(doc);
}

bool MeshViewStations::isCurrent(const MeshItemStation* item) const
{
	return m_currentItem == item;
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
    if (m_currentItem) m_currentItem->setCurrent(false);
    m_currentItem = findItem(pStation);
    if (m_currentItem) m_currentItem->setCurrent(true);
}

void MeshViewStations::updateStation(Station* pStation)
{
	MeshItemStation* item = findItem(pStation);
	if (item) item->updateStation();
}

MeshItemStation* MeshViewStations::currentItem() const
{
	return m_currentItem;
}

void MeshViewStations::currentChanged(MeshItemStation* item)
{
//    if (m_currentItem) m_currentItem->setCurrent(false);
//    m_currentItem = item;
//    if (m_currentItem) m_currentItem->setCurrent(true);
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

void MeshViewStations::addScheduleEntry(const Station* pStation, double time, const Packet* pPacket)
{
	findItem(pStation)->addScheduleEntry(time, pPacket);
}

void MeshViewStations::removeScheduleEntry(const Station* pStation, const Packet* pPacket)
{
	findItem(pStation)->removeScheduleEntry(pPacket);
}

void MeshViewStations::deliverScheduleEntry(const Station* pStation, const Packet* pPacket)
{
	findItem(pStation)->deliverScheduleEntry(pPacket);
}

void MeshViewStations::beginTransmit(const Station* pSrc, const Station* pDst, const Packet* pPacket)
{
    assert(m_transmits.count(pDst) == 0);
    m_transmits[pDst] = pSrc;
    MeshItemStation* item = findItem(pSrc);
    assert(item != NULL);
    item->beginTransmit();
}

void MeshViewStations::endTransmit(const Station* pDst)
{
    assert(m_transmits.count(pDst) != 0);
    const Station* pSrc = m_transmits.value(pDst);
    m_transmits.remove(pDst);
    MeshItemStation* item = findItem(pSrc);
    assert(item != NULL);
    item->endTransmit();
}
