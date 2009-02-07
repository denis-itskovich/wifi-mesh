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
	m_spinCoverage->setRange(0.01, 1000);

	m_spinRouteTTL = new QDoubleSpinBox;
	m_spinRouteTTL->setSingleStep(1);
	m_spinRouteTTL->setMaximum(3600);
	m_spinRouteTTL->setSuffix(tr(" [sec]"));

	m_spinDataRate = new QDoubleSpinBox;
	m_spinDataRate->setRange(1, 1 << 30);
	m_spinDataRate->setDecimals(0);

	m_comboDataUnits = new QComboBox;
	m_comboDataUnits->addItems(QStringList() << tr("Bit/s") << tr("KBit/s") << tr("MBit/s"));
	connect(m_spinDataRate, SIGNAL(valueChanged(double)), this, SLOT(setDataRate(double)));
	connect(m_comboDataUnits, SIGNAL(currentIndexChanged(int)), this, SLOT(setUnits(int)));

	m_spinRetryTimeout = new QDoubleSpinBox;
	m_spinRetryTimeout->setRange(0.0, 60.0);
	m_spinRetryTimeout->setDecimals(1);
	m_spinRetryTimeout->setSuffix(tr(" [sec]"));

	m_spinHeight = new QSpinBox;
	m_spinWidth = new QSpinBox;
	m_spinHeight->setRange(0, 10000);
	m_spinWidth->setRange(0, 10000);
	connect(m_spinHeight, SIGNAL(valueChanged(int)), this, SLOT(setHeight(int)));
	connect(m_spinWidth, SIGNAL(valueChanged(int)), this, SLOT(setWidth(int)));

	QLayout* dataRateLayout = new QHBoxLayout;
	dataRateLayout->addWidget(m_spinDataRate);
	dataRateLayout->addWidget(m_comboDataUnits);

	QFormLayout* sizeLayout = new QFormLayout;

	sizeLayout->addRow(tr("Width:"), m_spinWidth);
	sizeLayout->addRow(tr("Height:"), m_spinHeight);

	QFormLayout* settingsLayout = new QFormLayout;
	settingsLayout->addRow(tr("Coverage:"), m_spinCoverage);
	settingsLayout->addRow(tr("Route TTL:"), m_spinRouteTTL);
	settingsLayout->addRow(tr("Retry timeout:"), m_spinRetryTimeout);
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

void MeshViewSettings::setDataRate(double)
{
	emit updateDataRate(dataRate() / 8);
}

void MeshViewSettings::setUnits(int units)
{
	m_spinDataRate->setValue(m_dataRate / (1 << (10 *units)));
	// m_spinDataRate->setDecimals(units);
}

void MeshViewSettings::setHeight(int height)
{
	Size size = {(double)m_spinWidth->value(), (double)height};
	emit updateSize(size);
}

void MeshViewSettings::setWidth(int width)
{
	Size size = {(double)width, (double)m_spinHeight->value()};
	emit updateSize(size);
}

void MeshViewSettings::setDocument(MeshDocument* doc)
{
	disconnect(document());

	connect(m_spinCoverage, SIGNAL(valueChanged(double)), doc, SLOT(setCoverage(double)));
	connect(m_spinRouteTTL, SIGNAL(valueChanged(double)), doc, SLOT(setRouteTTL(double)));
	connect(this, SIGNAL(updateDataRate(int)), doc, SLOT(setDataRate(int)));
	connect(this, SIGNAL(updateSize(Size)), doc, SLOT(setWorldSize(Size)));
	connect(m_spinRetryTimeout, SIGNAL(valueChanged(double)), doc, SLOT(setRetryTimeout(double)));
	connect(doc, SIGNAL(simulationStarted()), this, SLOT(disable()));
	connect(doc, SIGNAL(simulationStopped()), this, SLOT(enable()));

	m_spinCoverage->setValue(doc->coverage());
	m_spinRouteTTL->setValue(doc->routeTTL());
	m_comboDataUnits->setCurrentIndex(2);
	m_spinDataRate->setValue(doc->dataRate() * 8 / (1 << (10 * m_comboDataUnits->currentIndex())));
	m_spinRetryTimeout->setValue(doc->retryTimeout());

	Size size = doc->worldSize();
	m_spinHeight->setValue((int)(size.x + 0.5));
	m_spinWidth->setValue((int)(size.y + 0.5));

	MeshView::setDocument(doc);
}

int MeshViewSettings::dataRate()
{
	int units = m_comboDataUnits->currentIndex();
	m_dataRate = (int)(m_spinDataRate->value() * (double)(1 << (units * 10)));
	return m_dataRate;
}
