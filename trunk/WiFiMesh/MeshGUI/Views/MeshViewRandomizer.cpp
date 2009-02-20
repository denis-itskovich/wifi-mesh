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
 * Randomizer.cpp
 *
 *  Created on: 21/08/2008
 *      Author: denis
 */

#include "MeshViewRandomizer.h"

MeshViewRandomizer::MeshViewRandomizer(QWidget* parent) :
	MeshView(parent)
{
	init();
}

MeshViewRandomizer::~MeshViewRandomizer()
{
}

void MeshViewRandomizer::init()
{
	m_spinStationCount = new QSpinBox;
	m_spinStationCount->setRange(1, 1000);

	m_spinAvgVelocity = new QDoubleSpinBox;
	m_spinAvgVelocity->setSingleStep(0.05);
	m_spinAvgVelocity->setRange(0.01, 99.99);

	m_spinAvgDataSize = new QSpinBox;
	m_spinAvgDataSize->setRange(1, 1 << 30);
	m_spinAvgDataSize->setSuffix(" [Bytes]");

	m_spinAvgPacketCount = new QSpinBox;
	m_spinAvgPacketCount->setRange(0, 100000);

	m_spinDuration = new QDoubleSpinBox;
	m_spinDuration->setRange(0.1, 3600);
	m_spinDuration->setDecimals(1);
	m_spinDuration->setSuffix(tr(" [sec]"));

	m_buttonGenerateWorld = new QPushButton(QIcon(":/generate.png"), tr("Generate world"));
	m_buttonGeneratePackets = new QPushButton(QIcon(":/packet.png"), tr("Generate packets only"));

	QFormLayout* mainLayout = new QFormLayout;

	mainLayout->addRow(tr("Stations count:"), m_spinStationCount);
	mainLayout->addRow(tr("Average velocity:"), m_spinAvgVelocity);
	mainLayout->addRow(tr("Average data size:"), m_spinAvgDataSize);
	mainLayout->addRow(tr("Average packets count:"), m_spinAvgPacketCount);
	mainLayout->addRow(tr("Duration:"), m_spinDuration);

	QGroupBox* group = new QGroupBox("Randomization parameters");
	group->setLayout(mainLayout);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(group);
	layout->addStretch();

//	layout->addWidget(m_buttonGenerateWorld);
//	layout->addWidget(m_buttonGeneratePackets);

	QHBoxLayout* buttonLayout = new QHBoxLayout;

	buttonLayout->addWidget(m_buttonGenerateWorld);
	buttonLayout->addWidget(m_buttonGeneratePackets);

	layout->addItem(buttonLayout);

	setLayout(layout);
}

void MeshViewRandomizer::setDocument(MeshDocument* doc)
{
	disconnect(document());

	connect(m_spinAvgVelocity, SIGNAL(valueChanged(double)), doc, SLOT(setAvgVelocity(double)));
	connect(m_spinAvgDataSize, SIGNAL(valueChanged(int)), doc, SLOT(setAvgDataSize(int)));
	connect(m_spinAvgPacketCount, SIGNAL(valueChanged(int)), doc, SLOT(setAvgPacketCount(int)));
	connect(m_spinStationCount, SIGNAL(valueChanged(int)), doc, SLOT(setStationCount(int)));
	connect(m_buttonGenerateWorld, SIGNAL(clicked()), doc, SLOT(generateWorld()));
	connect(m_buttonGeneratePackets, SIGNAL(clicked()), doc, SLOT(generatePackets()));
	connect(m_spinDuration, SIGNAL(valueChanged(double)), doc, SLOT(setDuration(double)));
	connect(doc, SIGNAL(simulationStarted()), this, SLOT(disable()));
	connect(doc, SIGNAL(simulationStopped()), this, SLOT(enable()));

	MeshView::setDocument(doc);
}

void MeshViewRandomizer::updateView()
{
    MeshDocument* doc = document();
    m_spinAvgVelocity->setValue(doc->avgVelocity());
    m_spinAvgDataSize->setValue(doc->avgDataSize());
    m_spinAvgPacketCount->setValue(doc->avgPacketCount());
    m_spinStationCount->setValue(doc->stationCount());
    m_spinDuration->setValue(doc->duration());
}
