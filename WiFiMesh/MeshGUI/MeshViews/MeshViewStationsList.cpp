/**
 * \file MeshViewStationsList.cpp
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 13/01/2009
 * \author Denis Itskovich
 */

#include "MeshViewStationsList.h"

MeshViewStationsList::MeshViewStationsList(QWidget* parent) :
	MeshViewStations(parent)
{
	init();
}

MeshViewStationsList::~MeshViewStationsList()
{
}

void MeshViewStationsList::init()
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
	buttonsLayout->addSpacing(8);
	buttonsLayout->addWidget(m_buttonRemove);

	layout->addItem(buttonsLayout);

	this->setLayout(layout);

	connect(m_treeStations, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
			this, SLOT(currentChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
}

void MeshViewStationsList::addStation(Station* pStation)
{
	MeshTreeItemStation* item = new MeshTreeItemStation(this, pStation);
	m_treeStations->insertTopLevelItem(m_treeStations->topLevelItemCount(), item);
	registerStation(pStation, item);
}

void MeshViewStationsList::currentChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
	MeshTreeItemStation* cur = dynamic_cast<MeshTreeItemStation*>(current);
	Station* pStation = findStation(cur);
	if (pStation) emit MeshViewStations::currentChanged(pStation);
}

void MeshViewStationsList::setDocument(MeshDocument* doc)
{
	MeshViewStations::setDocument(doc);
	connect(m_buttonAdd, SIGNAL(clicked()), doc, SLOT(addStation()));
	connect(m_buttonRemove, SIGNAL(clicked()), doc, SLOT(removeStation()));
}

void MeshViewStationsList::setCurrent(Station* pStation)
{
	m_treeStations->setCurrentItem(findItem(pStation));
}

void MeshViewStationsList::removeStation(Station* pStation)
{
	QTreeWidgetItem* item = findItem(pStation);
	if (item)
	{
		m_treeStations->invisibleRootItem()->removeChild(item);
		unregisterStation(pStation);
	}
}

void MeshViewStationsList::updateStation(Station* pStation)
{
	MeshItemStation* item = findItem(pStation);
	if (item) item->updateStation();
}

MeshTreeItemStation* MeshViewStationsList::findItem(Station* pStation)
{
	return dynamic_cast<MeshTreeItemStation*>(MeshViewStations::findItem(pStation));
}
