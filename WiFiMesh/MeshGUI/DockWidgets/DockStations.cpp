/*
 * Stations.cpp
 *
 *  Created on: 20/08/2008
 *      Author: denis
 */

#include "DockStations.h"

DockStations::DockStations(MeshDocument* doc, QWidget* parent) :
	DockFrame(doc, parent)
{
	init();
	if (doc) setDocument(doc);
}

DockStations::~DockStations()
{
}

void DockStations::init()
{
	QBoxLayout* layout = new QVBoxLayout(this);
	QBoxLayout* buttonsLayout = new QHBoxLayout();
	m_treeStations = new QTreeWidget(this);
	m_treeStations->setColumnCount(2);
	m_treeStations->setHeaderLabels(QStringList() << tr("Station") << tr("Properties"));

	m_buttonAdd = new QPushButton(tr("&Add"), this);
	m_buttonRemove = new QPushButton(tr("&Remove"), this);

	layout->addWidget(m_treeStations);

	buttonsLayout->addWidget(m_buttonAdd);
	buttonsLayout->addSpacing(20);
	buttonsLayout->addWidget(m_buttonRemove);

	layout->addItem(buttonsLayout);

	this->setLayout(layout);

	connect(m_treeStations, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
			this, SLOT(currentChanged(QTreeWidgetItem *, QTreeWidgetItem *)));

}

void DockStations::addStation(Station* pStation)
{
	Location location = stationLocation(pStation);
	Velocity velocity = stationVelocity(pStation);

	QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << stationName(pStation));

	QTreeWidgetItem* locationItem = new QTreeWidgetItem(QStringList() << tr("Location"));
	locationItem->addChild(new QTreeWidgetItem(QStringList() << tr("x") << tr("%1").arg(location.x)));
	locationItem->addChild(new QTreeWidgetItem(QStringList() << tr("y") << tr("%1").arg(location.y)));

	QTreeWidgetItem* velocityItem = new QTreeWidgetItem(QStringList() << tr("Velocity"));
	velocityItem->addChild(new QTreeWidgetItem(QStringList() << tr("x") << tr("%1").arg(velocity.x)));
	velocityItem->addChild(new QTreeWidgetItem(QStringList() << tr("y") << tr("%1").arg(velocity.y)));

	item->addChild(locationItem);
	item->addChild(velocityItem);
	m_treeStations->insertTopLevelItem(m_treeStations->topLevelItemCount(), item);
	m_itemToStation.insert(item, pStation);
}

void DockStations::currentChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
	ItemToStation::const_iterator i = m_itemToStation.find(current);
	if (i == m_itemToStation.end()) return;
	emit currentChanged(*i);
}

void DockStations::setDocument(MeshDocument* doc)
{
	disconnect(doc);
	connect(this, SIGNAL(currentChanged(Station*)), doc, SLOT(setCurrentStation(Station*)));
	connect(m_buttonAdd, SIGNAL(clicked()), doc, SLOT(addStation()));
	connect(m_buttonRemove, SIGNAL(clicked()), doc, SLOT(removeStation()));
	connect(doc, SIGNAL(currentStationChanged(Station*)), this, SLOT(setCurrent(Station*)));
	connect(doc, SIGNAL(stationAdded(Station*)), this, SLOT(addStation(Station*)));
	connect(doc, SIGNAL(stationRemoved(Station*)), this, SLOT(removeStation(Station*)));
	connect(doc, SIGNAL(stationUpdated(Station*)), this, SLOT(updateStation(Station*)));
	DockFrame::setDocument(doc);
}

void DockStations::setCurrent(Station* pStation)
{
	m_treeStations->setCurrentItem(findItem(pStation));
}

void DockStations::removeStation(Station* pStation)
{
	QTreeWidgetItem* item = findItem(pStation);
	if (item)
	{
		m_treeStations->invisibleRootItem()->removeChild(item);
		m_itemToStation.remove(item);
	}
}

void DockStations::updateStation(Station* pStation)
{
	QTreeWidgetItem* item = findItem(pStation);
	QTreeWidgetItem* locationItem = item->child(0);
	QTreeWidgetItem* velocityItem = item->child(1);

	Location location = stationLocation(pStation);
	Velocity velocity = stationVelocity(pStation);

	locationItem->child(0)->setText(1, tr("%1").arg(location.x));
	locationItem->child(1)->setText(1, tr("%1").arg(location.y));

	velocityItem->child(0)->setText(1, tr("%1").arg(velocity.x));
	velocityItem->child(1)->setText(1, tr("%1").arg(velocity.y));
}

QTreeWidgetItem* DockStations::findItem(Station* pStation)
{
	QList<QTreeWidgetItem*> items = m_treeStations->findItems(stationName(pStation), Qt::MatchExactly);
	if (items.empty()) return NULL;
	return items.front();
}

QString DockStations::stationName(Station* pStation)
{
	return tr("Station %1").arg(stationId(pStation));
}

StationId DockStations::stationId(Station* pStation)
{
	StationId id;
	CHECK(StationGetId(pStation, &id));
	return id;
}

Location DockStations::stationLocation(Station* pStation)
{
	Location location;
	CHECK(StationGetLocation(pStation, &location));
	return location;
}

Velocity DockStations::stationVelocity(Station* pStation)
{
	Velocity velocity;
	CHECK(StationGetVelocity(pStation, &velocity));
	return velocity;
}
