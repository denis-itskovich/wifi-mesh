/*
 * Stations.cpp
 *
 *  Created on: 20/08/2008
 *      Author: denis
 */

#include "DockStations.h"

DockStations::DockStations(QWidget* parent) : DockFrame(parent)
{
	init();
}

DockStations::~DockStations()
{
}

void DockStations::init()
{
	QBoxLayout* layout = new QVBoxLayout();
	m_listWidget = new QListWidget(this);
	layout->addWidget(m_listWidget);
	this->setLayout(layout);
}

void DockStations::addStation(MeshStation* station)
{
	m_listWidget->addItem(tr("Station %1").arg(station->id()));
}

void DockStations::clear()
{
	m_listWidget->clear();
}
