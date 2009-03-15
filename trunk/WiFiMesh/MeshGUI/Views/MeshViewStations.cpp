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
    initActions();
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
    connect(doc, SIGNAL(routeEntryAdded(const Station*, StationId, double, int)), this, SLOT(addRouteEntry(const Station*, StationId, double, int)));
    connect(doc, SIGNAL(routeEntryUpdated(const Station*, StationId, StationId, double, int)), this, SLOT(updateRouteEntry(const Station*, StationId, StationId, double, int)));
    connect(doc, SIGNAL(routeEntryUpdated(const Station*, StationId, double, int)), this, SLOT(updateRouteEntry(const Station*, StationId, double, int)));
	connect(doc, SIGNAL(routeEntryExpired(const Station*, StationId)), this, SLOT(removeRouteEntry(const Station*, StationId)));

	connect(doc, SIGNAL(scheduleEntryAdded(const Station*, double, const Packet*)), this, SLOT(addScheduleEntry(const Station*, double, const Packet*)));
	connect(doc, SIGNAL(scheduleEntryDelivered(const Station*, const Packet*)), SLOT(deliverScheduleEntry(const Station*, const Packet*)));
	connect(doc, SIGNAL(scheduleEntryRemoved(const Station*, const Packet*)), this, SLOT(removeScheduleEntry(const Station*, const Packet*)));

	connect(doc, SIGNAL(worldChanged()), this, SLOT(updateAll()));
    connect(doc, SIGNAL(beginTransmit(const Station*, const Station*, const Packet*)), this, SLOT(beginTransmit(const Station*, const Station*, const Packet*)));
    connect(doc, SIGNAL(endTransmit(const Station*)), this, SLOT(endTransmit(const Station*)));

    connect(doc, SIGNAL(simulationCleared()), this, SLOT(resetStations()));
    connect(doc, SIGNAL(simulationReset()), this, SLOT(resetStations()));

    connect(this, SIGNAL(addStation()), doc, SLOT(addStation()));
    connect(this, SIGNAL(addStation(Location)), doc, SLOT(addStation(Location)));

    connect(m_actRemoveStation, SIGNAL(triggered()), doc, SLOT(removeStation()));
    connect(m_actAddPacket, SIGNAL(triggered()), doc, SLOT(addPacket()));

	MeshView::setDocument(doc);
}

bool MeshViewStations::isCurrent(const MeshItemStation* item) const
{
	return m_currentItem == item;
}

void MeshViewStations::addStationTriggered()
{
    emit addStation();
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
    QList<const Station*> dests = m_transmits.keys(pStation);
    foreach (const Station* pDst, dests) endTransmit(pDst);
    while (m_transmits.count(pStation)) endTransmit(pStation);

	unregisterStation(pStation);
}

void MeshViewStations::setCurrent(Station* pStation)
{
    if (m_currentItem) m_currentItem->setCurrent(false);
    m_currentItem = findItem(pStation);
    if (m_currentItem) m_currentItem->setCurrent(true);
    m_actRemoveStation->setEnabled(pStation != NULL);
    m_actAddPacket->setEnabled(pStation != NULL);
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

void MeshViewStations::addRouteEntry(const Station* pStation, StationId dst, double nextRetry, int retriesLeft)
{
    findItem(pStation)->addRouteEntry(dst, nextRetry, retriesLeft);
}

void MeshViewStations::updateRouteEntry(const Station* pStation, StationId dst, StationId trans, double expires, int length)
{
	findItem(pStation)->updateRouteEntry(dst, trans, expires, length);
}

void MeshViewStations::updateRouteEntry(const Station* pStation, StationId dst, double nextRetry, int retriesLeft)
{
    findItem(pStation)->updateRouteEntry(dst, nextRetry, retriesLeft);
}

void MeshViewStations::removeRouteEntry(const Station* pStation, StationId dst)
{
    MeshItemStation* item = findItem(pStation);
	if (item) findItem(pStation)->removeRouteEntry(dst);
}

void MeshViewStations::addScheduleEntry(const Station* pStation, double time, const Packet* pPacket)
{
	findItem(pStation)->addScheduleEntry(time, pPacket);
}

void MeshViewStations::removeScheduleEntry(const Station* pStation, const Packet* pPacket)
{
    MeshItemStation* item = findItem(pStation);
	if (item) item->removeScheduleEntry(pPacket);
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
    //assert(m_transmits.count(pDst) != 0);
    if (m_transmits.count(pDst) == 0) return;
    const Station* pSrc = m_transmits.value(pDst);
    m_transmits.remove(pDst);
    MeshItemStation* item = findItem(pSrc);
    if (item) item->endTransmit();
}

void MeshViewStations::resetStations()
{
    for (StationToItem::const_iterator i = m_stationToItem.begin(); i != m_stationToItem.end(); ++i)
    {
        while (m_transmits.count(i.key())) endTransmit(i.key());
        i.value()->reset();
    }
}

void MeshViewStations::initActions()
{
    m_actAddPacket = new QAction(QIcon(":/packet.png"), tr("Add &packet"), this);
    m_actAddStation = new QAction(QIcon(":/add.png"), tr("&Add"), this);
    connect(m_actAddStation, SIGNAL(triggered()), this, SLOT(addStationTriggered()));
    m_actAddPacket->setEnabled(false);
    m_actRemoveStation = new QAction(QIcon(":/remove.png"), tr("&Remove"), this);
    m_actRemoveStation->setEnabled(false);
}
