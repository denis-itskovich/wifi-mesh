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
	m_itemToStation[pItem] = pStation;
	m_stationToItem[pStation] = pItem;
}

void MeshViewStations::unregisterStation(Station* pStation)
{
	MeshItemStation* pItem = findItem(pStation);
	if (!pItem) return;
	m_itemToStation.remove(pItem);
	m_stationToItem.remove(pStation);
}

void MeshViewStations::setDocument(MeshDocument* doc)
{
	disconnect(doc);
	connect(this, SIGNAL(currentChanged(Station*)), doc, SLOT(setCurrentStation(Station*)));
	connect(doc, SIGNAL(currentStationChanged(Station*)), this, SLOT(setCurrent(Station*)));
	connect(doc, SIGNAL(stationAdded(Station*)), this, SLOT(addStation(Station*)));
	connect(doc, SIGNAL(stationRemoved(Station*)), this, SLOT(removeStation(Station*)));
	connect(doc, SIGNAL(stationUpdated(Station*)), this, SLOT(updateStation(Station*)));
	MeshView::setDocument(doc);
}

MeshItemStation* MeshViewStations::findItem(Station* pStation)
{
	StationToItem::const_iterator i = m_stationToItem.find(pStation);
	if (i != m_stationToItem.end()) return *i;
	return NULL;
}

Station* MeshViewStations::findStation(MeshItemStation* pItem)
{
	ItemToStation::const_iterator i = m_itemToStation.find(pItem);
	if (i != m_itemToStation.end()) return *i;
	return NULL;
}
