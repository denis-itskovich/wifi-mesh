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
	QFormLayout* layout = new QFormLayout(this);

	m_spinCoverage = new QDoubleSpinBox;
	m_spinCoverage->setSingleStep(0.5);

	m_spinRouteTTL = new QDoubleSpinBox;
	m_spinRouteTTL->setSingleStep(1);

	m_spinDataRate = new QSpinBox;
	m_comboDataUnits = new QComboBox;
	m_comboDataUnits->addItems(QStringList() << tr("Bit/s") << tr("KBit/s") << tr("MBit/s"));
	connect(m_spinDataRate, SIGNAL(valueChanged(int)), this, SLOT(setDataRate(int)));
	connect(m_comboDataUnits, SIGNAL(currentIndexChanged(int)), this, SLOT(setDataRate(int)));

	QLayout* dataRateLayout = new QHBoxLayout;
	dataRateLayout->addWidget(m_spinDataRate);
	dataRateLayout->addWidget(m_comboDataUnits);

	layout->addRow(tr("Coverage:"), m_spinCoverage);
	layout->addRow(tr("Route TTL:"), m_spinRouteTTL);
	layout->addRow(tr("Data rate:"), dataRateLayout);

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
