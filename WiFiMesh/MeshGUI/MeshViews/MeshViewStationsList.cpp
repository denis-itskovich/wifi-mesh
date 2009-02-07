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

void MeshViewStationsList::init()
{
	QBoxLayout* layout = new QVBoxLayout(this);
	QBoxLayout* buttonsLayout = new QHBoxLayout();
	m_treeStations = new QTreeWidget(this);
	m_treeStations->setColumnCount(2);
	m_treeStations->setHeaderLabels(QStringList() << tr("Station") << tr("Properties"));

	m_buttonAdd = new QPushButton(QIcon(":/add.png"), tr("&Add"), this);
	m_buttonRemove = new QPushButton(QIcon(":/remove.png"), tr("&Remove"), this);
	m_buttonRemove->setEnabled(false);
	m_buttonPacket = new QPushButton(QIcon(":/packet.png"), tr("&Add packet"), this);
	m_buttonPacket->setEnabled(false);

	layout->addWidget(m_treeStations);

	buttonsLayout->addWidget(m_buttonPacket);
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(m_buttonAdd);
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
	MeshViewStations::addStation(pStation);
}

void MeshViewStationsList::currentChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
	m_buttonRemove->setEnabled(current != NULL);
	m_buttonPacket->setEnabled(current != NULL);
	if (!current) return;
	while (current->parent()) current = current->parent();
	MeshTreeItemStation* cur = dynamic_cast<MeshTreeItemStation*>(current);
	MeshViewStations::currentChanged(cur);
}

MeshTreeItemStation* MeshViewStationsList::currentItem() const
{
	QTreeWidgetItem* item = m_treeStations->currentItem();
	if (!item) return NULL;
	while (item->parent()) item = item->parent();
	return dynamic_cast<MeshTreeItemStation*>(item);
}

void MeshViewStationsList::setDocument(MeshDocument* doc)
{
	MeshViewStations::setDocument(doc);
	connect(m_buttonAdd, SIGNAL(clicked()), doc, SLOT(addStation()));
	connect(m_buttonRemove, SIGNAL(clicked()), doc, SLOT(removeStation()));
	connect(m_buttonPacket, SIGNAL(clicked()), doc, SLOT(addPacket()));
}

void MeshViewStationsList::setCurrent(Station* pStation)
{
	MeshTreeItemStation* newCur = findItem(pStation);
	if (newCur == currentItem()) return;
	m_treeStations->setCurrentItem(newCur);
}

void MeshViewStationsList::removeStation(Station* pStation)
{
	QTreeWidgetItem* item = findItem(pStation);
	if (item)
	{
		m_treeStations->invisibleRootItem()->removeChild(item);
	}
	MeshViewStations::removeStation(pStation);
}

MeshTreeItemStation* MeshViewStationsList::findItem(Station* pStation)
{
	return dynamic_cast<MeshTreeItemStation*>(MeshViewStations::findItem(pStation));
}
