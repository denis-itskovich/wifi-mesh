/*
 * Randomizer.cpp
 *
 *  Created on: 21/08/2008
 *      Author: denis
 */

#include "DockRandomizer.h"

DockRandomizer::DockRandomizer(QWidget* parent) : DockFrame(parent)
{
	init();
}

DockRandomizer::~DockRandomizer()
{
}

void DockRandomizer::init()
{
	QBoxLayout* mainLayout = new QVBoxLayout(this);
	QBoxLayout* layoutStations = new QHBoxLayout();

	QGroupBox* groupVelocity = new QGroupBox(tr("Velocity parameters"), this);
	QGridLayout* layoutVelocity = new QGridLayout(groupVelocity);

	QGroupBox* groupDataAmount = new QGroupBox(tr("Data amount parameters"), this);
	QGridLayout* layoutDataAmount = new QGridLayout(groupDataAmount);

	groupVelocity->setLayout(layoutVelocity);

	m_sliderStationsCount = new QSlider(Qt::Horizontal, this);
	m_sliderStationsCount->setTickPosition(QSlider::TicksBelow);
	m_spinStationsCount = new QSpinBox(this);

	m_spinMinVelocity = new QDoubleSpinBox(this);
	m_spinMaxVelocity = new QDoubleSpinBox(this);
	m_spinMinDataAmount = new QSpinBox(this);
	m_spinMaxDataAmount = new QSpinBox(this);
	m_buttonGenerate = new QPushButton(tr("Generate"), this);

	mainLayout->addWidget(new QLabel(tr("Stations count:"), this));
	layoutStations->setSpacing(4);
	layoutStations->addWidget(m_sliderStationsCount);
	layoutStations->addWidget(m_spinStationsCount);
	mainLayout->addItem(layoutStations);

	layoutVelocity->addWidget(new QLabel(tr("Minimal velocity:"), this), 0, 0);
	layoutVelocity->addWidget(m_spinMinVelocity, 0, 1);
	layoutVelocity->addWidget(new QLabel(tr("Maximal velocity:"), this), 1, 0);
	layoutVelocity->addWidget(m_spinMaxVelocity, 1, 1);

	layoutDataAmount->addWidget(new QLabel(tr("Minimal data amount:"), this), 0, 0);
	layoutDataAmount->addWidget(m_spinMinDataAmount, 0, 1);
	layoutDataAmount->addWidget(new QLabel(tr("Maximal data amount:"), this), 1, 0);
	layoutDataAmount->addWidget(m_spinMaxDataAmount, 1, 1);

	mainLayout->addWidget(groupVelocity);
	mainLayout->addWidget(groupDataAmount);
	mainLayout->addWidget(m_buttonGenerate);
	mainLayout->addStretch();

	setLayout(mainLayout);
}
