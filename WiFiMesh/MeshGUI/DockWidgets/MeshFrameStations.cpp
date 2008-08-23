/*
 * MeshFrameStations.cpp
 *
 *  Created on: 20/08/2008
 *      Author: denis
 */

#include "MeshFrameStations.h"

MeshFrameStations::MeshFrameStations(QWidget* parent) : MeshFrame(parent)
{
	init();
}

MeshFrameStations::~MeshFrameStations()
{
}

void MeshFrameStations::init()
{
	QBoxLayout* layout = new QVBoxLayout();
	m_listWidget = new QListWidget(this);
	layout->addWidget(m_listWidget);
	this->setLayout(layout);
}

void MeshFrameStations::addStation(MeshStation* station)
{
	m_listWidget->addItem(tr("Station %1").arg(station->id()));
}

void MeshFrameStations::clear()
{
	m_listWidget->clear();
}
