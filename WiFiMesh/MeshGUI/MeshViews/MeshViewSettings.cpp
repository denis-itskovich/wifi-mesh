/**
 * \file MeshViewSettings.cpp
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 11/01/2009
 * \author Denis Itskovich
 */

#include "MeshViewSettings.h"

MeshViewSettings::MeshViewSettings(QWidget* parent) :
	MeshView(parent)
{
	init();
}

MeshViewSettings::~MeshViewSettings()
{

}

void MeshViewSettings::init()
{
	m_spinCoverage = new QDoubleSpinBox;
	m_spinCoverage->setSingleStep(0.5);

	m_spinRouteTTL = new QDoubleSpinBox;
	m_spinRouteTTL->setSingleStep(1);

	m_spinDataRate = new QSpinBox;
	m_comboDataUnits = new QComboBox;
	m_comboDataUnits->addItems(QStringList() << tr("Bit/s") << tr("KBit/s") << tr("MBit/s"));
	connect(m_spinDataRate, SIGNAL(valueChanged(int)), this, SLOT(setDataRate(int)));
	connect(m_comboDataUnits, SIGNAL(currentIndexChanged(int)), this, SLOT(setDataRate(int)));

	m_spinHeight = new QSpinBox;
	m_spinWidth = new QSpinBox;

	QLayout* dataRateLayout = new QHBoxLayout;
	dataRateLayout->addWidget(m_spinDataRate);
	dataRateLayout->addWidget(m_comboDataUnits);

	QFormLayout* sizeLayout = new QFormLayout;

	sizeLayout->addRow(tr("Width:"), m_spinWidth);
	sizeLayout->addRow(tr("Height:"), m_spinHeight);

	QFormLayout* settingsLayout = new QFormLayout;
	settingsLayout->addRow(tr("Coverage:"), m_spinCoverage);
	settingsLayout->addRow(tr("Route TTL:"), m_spinRouteTTL);
	settingsLayout->addRow(tr("Data rate:"), dataRateLayout);

	QGroupBox* settingsGroup = new QGroupBox(tr("Simulator settings"));
	QGroupBox* sizeGroup = new QGroupBox(tr("World size"));

	settingsGroup->setLayout(settingsLayout);
	sizeGroup->setLayout(sizeLayout);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(settingsGroup);
	layout->addWidget(sizeGroup);
	layout->addStretch();

	setLayout(layout);
}

void MeshViewSettings::setDataRate(int)
{
	emit updateDataRate(dataRate());
}

void MeshViewSettings::setDocument(MeshDocument* doc)
{
	disconnect(document());

	connect(m_spinCoverage, SIGNAL(valueChanged(double)), doc, SLOT(setCoverage(double)));
	connect(m_spinRouteTTL, SIGNAL(valueChanged(double)), doc, SLOT(setRouteTTL(double)));
	connect(this, SIGNAL(updateDataRate(unsigned long)), doc, SLOT(setDataRate(unsigned long)));

	MeshView::setDocument(doc);
}

int MeshViewSettings::dataRate() const
{
	int units = m_comboDataUnits->currentIndex();
	int rate = m_spinDataRate->value();
	return (unsigned long)(rate * (1 << (units * 10)));
}
