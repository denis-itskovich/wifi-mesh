/*
 * StationProperties.cpp
 *
 *  Created on: 22/08/2008
 *      Author: denis
 */

#include "DockStationProperties.h"

DockStationProperties::DockStationProperties(MeshDocument* doc, QWidget* parent) :
	DockFrame(doc, parent)
{
	init();
	if (doc) setDocument(doc);
}

DockStationProperties::~DockStationProperties()
{
}

void DockStationProperties::init()
{
	QBoxLayout* layout = new QVBoxLayout(this);

	QGroupBox* groupLocation = new QGroupBox(tr("Station location"), this);
	QGridLayout* layoutLocation = new QGridLayout(groupLocation);
	groupLocation->setLayout(layoutLocation);

	m_spinRow = new QSpinBox(groupLocation);
	m_spinCol = new QSpinBox(groupLocation);

	layoutLocation->setSpacing(4);
	layoutLocation->addWidget(new QLabel(tr("Row:")), 0, 0);
	layoutLocation->addWidget(m_spinRow, 0, 1);
	layoutLocation->addWidget(new QLabel(tr("Column:")), 1, 0);
	layoutLocation->addWidget(m_spinCol, 1, 1);


	QGroupBox* groupVelocity = new QGroupBox(tr("Station velocity"), this);
	QGridLayout* layoutVelocity = new QGridLayout();
	QBoxLayout* layoutDirection = new QVBoxLayout(groupVelocity);
	layoutVelocity->setSpacing(4);
	groupVelocity->setLayout(layoutDirection);

	m_spinVelocity = new QDoubleSpinBox(groupVelocity);
	m_spinVelocity->setSingleStep(0.05);

	m_spinAngle = new QSpinBox(groupVelocity);
	m_dialAngle = new QDial(groupVelocity);

	connect(m_spinAngle, SIGNAL(valueChanged(int)), this, SLOT(spinAngleChanged(int)));
	connect(m_dialAngle, SIGNAL(valueChanged(int)), this, SLOT(dialAngleChanged(int)));

	m_spinAngle->setMaximum(359);
	m_dialAngle->setWrapping(true);
	m_dialAngle->setNotchesVisible(true);
	m_dialAngle->setNotchTarget(15);
	m_dialAngle->setMaximum(359);
	spinAngleChanged(m_spinAngle->value());

	layoutVelocity->addWidget(new QLabel(tr("Velocity:"), groupVelocity), 0, 0);
	layoutVelocity->addWidget(m_spinVelocity, 0, 1);


	layoutVelocity->addWidget(new QLabel(tr("Direction:"), groupVelocity), 1, 0);
	layoutVelocity->addWidget(m_spinAngle, 1, 1);

	layoutDirection->addItem(layoutVelocity);
	layoutDirection->addWidget(m_dialAngle);

	layout->addWidget(groupLocation);
	layout->addWidget(groupVelocity);
	layout->addWidget(new QPushButton(tr("Apply"), this));

	layout->addStretch();
}

void DockStationProperties::dialAngleChanged(int angle)
{
	angle = (angle + 90) % 360;
	m_spinAngle->setValue(angle);
}

void DockStationProperties::spinAngleChanged(int angle)
{
	angle = (angle + 270) % 360;
	m_dialAngle->setValue(angle);
}
