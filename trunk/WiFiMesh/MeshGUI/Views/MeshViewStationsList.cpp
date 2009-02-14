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
 * @file MeshViewStationsList.cpp
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * @date 13/01/2009
 * @author Denis Itskovich
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

	m_buttonAdd = createButton(m_actAddStation);
	m_buttonRemove = createButton(m_actRemoveStation);
	m_buttonPacket = createButton(m_actAddPacket);

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
//	connect(m_buttonAdd, SIGNAL(clicked()), doc, SLOT(addStation()));
//	connect(m_buttonRemove, SIGNAL(clicked()), doc, SLOT(removeStation()));
//	connect(m_buttonPacket, SIGNAL(clicked()), doc, SLOT(addPacket()));
}

void MeshViewStationsList::setCurrent(Station* pStation)
{
    MeshViewStations::setCurrent(pStation);
	MeshTreeItemStation* newCur = findItem(pStation);
	if (newCur == currentItem()) return;
	m_treeStations->setCurrentItem(newCur);
}

void MeshViewStationsList::removeStation(Station* pStation)
{
	QTreeWidgetItem* item = findItem(pStation);
    MeshViewStations::removeStation(pStation);
	if (item)
	{
		m_treeStations->invisibleRootItem()->removeChild(item);
		delete item;
	}
}

MeshTreeItemStation* MeshViewStationsList::findItem(Station* pStation)
{
	return dynamic_cast<MeshTreeItemStation*>(MeshViewStations::findItem(pStation));
}

QPushButton* MeshViewStationsList::createButton(QAction* action)
{
    QPushButton* button = new QPushButton(action->icon(), action->text(), this);
    connect(button, SIGNAL(clicked()), action, SLOT(trigger()));
    return button;
}
