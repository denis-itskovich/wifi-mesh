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
	QBoxLayout* layout = new QVBoxLayout(this);
	QBoxLayout* buttonsLayout = new QHBoxLayout();
	m_listWidget = new QListWidget(this);

	m_buttonAdd = new QPushButton(tr("&Add"), this);
	m_buttonRemove = new QPushButton(tr("&Remove"), this);
	m_buttonProperties = new QPushButton(tr("&Properties"), this);

	layout->addWidget(m_listWidget);

	buttonsLayout->addWidget(m_buttonAdd);
	buttonsLayout->addWidget(m_buttonRemove);
	buttonsLayout->addWidget(m_buttonProperties);

	layout->addItem(buttonsLayout);

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
