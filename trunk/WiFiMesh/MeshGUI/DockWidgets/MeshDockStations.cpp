/*
 * MeshDockStations.cpp
 *
 *  Created on: 20/08/2008
 *      Author: denis
 */

#include "MeshDockStations.h"

MeshDockStations::MeshDockStations(QWidget* parent) : MeshDock(tr("Stations"), parent)
{
	init();
}

MeshDockStations::~MeshDockStations()
{
}

void MeshDockStations::init()
{
	QBoxLayout* layout = new QVBoxLayout();
	m_listWidget = new QListWidget(this);
	layout->addWidget(m_listWidget);
	this->setLayout(layout);
}

void MeshDockStations::addStation(MeshStation* station)
{
	m_listWidget->addItem(tr("Station %1").arg(station->id()));
}

void MeshDockStations::clear()
{
	m_listWidget->clear();
}
