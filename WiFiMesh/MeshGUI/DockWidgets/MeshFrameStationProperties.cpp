/*
 * MeshFrameStationProperties.cpp
 *
 *  Created on: 22/08/2008
 *      Author: denis
 */

#include "MeshFrameStationProperties.h"

MeshFrameStationProperties::MeshFrameStationProperties(QWidget* parent) : MeshFrame(parent)
{
	init();
}

MeshFrameStationProperties::~MeshFrameStationProperties()
{
}

void MeshFrameStationProperties::init()
{
	QBoxLayout* layout = new QVBoxLayout(this);

	QGroupBox* groupLocation = new QGroupBox(tr("Station location"), this);
	QGridLayout* layoutLocation = new QGridLayout(groupLocation);
	groupLocation->setLayout(layoutLocation);

	m_spinRow = new QSpinBox(groupLocation);
	m_spinCol = new QSpinBox(groupLocation);

	layoutLocation->addWidget(new QLabel(tr("Row:")), 0, 0);
	layoutLocation->addWidget(m_spinRow, 0, 1);
	layoutLocation->addWidget(new QLabel(tr("Column:")), 1, 0);
	layoutLocation->addWidget(m_spinCol, 1, 1);


	QGroupBox* groupVelocity = new QGroupBox(tr("Station velocity"), this);
	QGridLayout* layoutVelocity = new QGridLayout();
	QBoxLayout* layoutDirection = new QVBoxLayout(groupVelocity);
	groupVelocity->setLayout(layoutDirection);

	m_spinVelocity = new QDoubleSpinBox(groupVelocity);
	m_spinAngle = new QDoubleSpinBox(groupVelocity);
	m_dialAngle = new QDial(groupVelocity);

	m_dialAngle->setWrapping(true);
	m_dialAngle->setNotchesVisible(true);
	m_dialAngle->setNotchTarget(15);
	m_dialAngle->setMaximum(360);

	layoutVelocity->addWidget(new QLabel(tr("Velocity:"), groupVelocity), 0, 0);
	layoutVelocity->addWidget(m_spinVelocity, 0, 1);


	layoutVelocity->addWidget(new QLabel(tr("Direction:"), groupVelocity), 1, 0);
	layoutVelocity->addWidget(m_spinAngle, 1, 1);

	layoutDirection->addItem(layoutVelocity);
	layoutDirection->addWidget(m_dialAngle);

	layout->addWidget(groupLocation);
	layout->addWidget(groupVelocity);
	layout->addStretch();
}
