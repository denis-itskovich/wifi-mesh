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

	m_spinRouteExpTimeout = new QDoubleSpinBox;
	m_spinRouteExpTimeout->setSingleStep(1);
	m_spinRouteExpTimeout->setMaximum(3600);
	m_spinRouteExpTimeout->setSuffix(tr(" [sec]"));

    m_spinRouteRetryTimeout = new QDoubleSpinBox;
    m_spinRouteRetryTimeout->setRange(0.1, 60.0);
    m_spinRouteRetryTimeout->setDecimals(1);
    m_spinRouteRetryTimeout->setSuffix(tr(" [sec]"));

    m_spinPacketHopsThreshold = new QSpinBox;
    m_spinPacketHopsThreshold->setRange(0, 1000);
    // m_spinPacketHopsThreshold->setSuffix(tr(" [hops]"));

    m_spinPacketRetryTimeout = new QDoubleSpinBox;
    m_spinPacketRetryTimeout->setRange(0.01, 60.0);
    m_spinPacketRetryTimeout->setDecimals(2);
    m_spinPacketRetryTimeout->setSingleStep(0.01);
    m_spinPacketRetryTimeout->setSuffix(tr(" [sec]"));

    m_spinPacketRetryThreshold = new QSpinBox;
    m_spinPacketRetryThreshold->setRange(1, 100);

	m_spinDataRate = new QDoubleSpinBox;
	m_spinDataRate->setRange(1, 1 << 30);
	m_spinDataRate->setDecimals(0);

	m_comboDataUnits = new QComboBox;
	m_comboDataUnits->addItems(QStringList() << tr("Bit/s") << tr("KBit/s") << tr("MBit/s"));
	connect(m_spinDataRate, SIGNAL(valueChanged(double)), this, SLOT(setDataRate(double)));
	connect(m_comboDataUnits, SIGNAL(currentIndexChanged(int)), this, SLOT(setUnits(int)));

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

	QFormLayout* simulatorLayout = new QFormLayout;
	simulatorLayout->addRow(tr("Coverage:"), m_spinCoverage);
	simulatorLayout->addRow(tr("Data rate:"), dataRateLayout);

	QFormLayout* routeLayout = new QFormLayout;
    routeLayout->addRow(tr("Expiration timeout:"), m_spinRouteExpTimeout);
    routeLayout->addRow(tr("Retry timeout:"), m_spinRouteRetryTimeout);

    QFormLayout* packetLayout = new QFormLayout;
    packetLayout->addRow(tr("Hops count threshold:"), m_spinPacketHopsThreshold);
    packetLayout->addRow(tr("Retry timeout:"), m_spinPacketRetryTimeout);
    packetLayout->addRow(tr("Retries count threshold:"), m_spinPacketRetryThreshold);

	QGroupBox* simulatorGroup = new QGroupBox(tr("Simulator settings"));
	QGroupBox* routeGroup = new QGroupBox(tr("Route settings"));
	QGroupBox* packetGroup = new QGroupBox(tr("Packet settings"));
	QGroupBox* sizeGroup = new QGroupBox(tr("World size"));

	simulatorGroup->setLayout(simulatorLayout);
    routeGroup->setLayout(routeLayout);
    packetGroup->setLayout(packetLayout);
	sizeGroup->setLayout(sizeLayout);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(simulatorGroup);
    layout->addWidget(packetGroup);
    layout->addWidget(routeGroup);
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
    m_spinCoverage->setValue(doc->coverage());

	connect(m_spinRouteExpTimeout, SIGNAL(valueChanged(double)), doc, SLOT(setRouteExpirationTimeout(double)));
    m_spinRouteExpTimeout->setValue(doc->routeExpirationTimeout());

    connect(m_spinRouteRetryTimeout, SIGNAL(valueChanged(double)), doc, SLOT(setRouteRertyTimeout(double)));
    m_spinRouteRetryTimeout->setValue(doc->routeRetryTimeout());

	connect(m_spinPacketHopsThreshold, SIGNAL(valueChanged(int)), SLOT(setPacketHopsThreshold(int)));
	m_spinPacketHopsThreshold->setValue(doc->packetHopsThreshold());

	connect(m_spinPacketRetryTimeout, SIGNAL(valueChanged(double)), SLOT(setPacketRetryTimeout(double)));
	m_spinPacketRetryTimeout->setValue(doc->packetRetryTimeout());

	connect(m_spinPacketRetryThreshold, SIGNAL(valueChanged(int)), SLOT(setPacketRetryThreshold(int)));
	m_spinPacketRetryThreshold->setValue(doc->packetRetryThreshold());

    connect(this, SIGNAL(updateSize(Size)), doc, SLOT(setWorldSize(Size)));
    Size size = doc->worldSize();
    m_spinWidth->setValue((int)(size.x + 0.5));
    m_spinHeight->setValue((int)(size.y + 0.5));

    connect(doc, SIGNAL(simulationStarted()), this, SLOT(disable()));
    connect(doc, SIGNAL(simulationStopped()), this, SLOT(enable()));

    connect(this, SIGNAL(updateDataRate(int)), doc, SLOT(setDataRate(int)));
    m_comboDataUnits->setCurrentIndex(2);
    m_spinDataRate->setValue(doc->dataRate() * 8 / (1 << (10 * m_comboDataUnits->currentIndex())));

	MeshView::setDocument(doc);
}

int MeshViewSettings::dataRate()
{
	int units = m_comboDataUnits->currentIndex();
	m_dataRate = (int)(m_spinDataRate->value() * (double)(1 << (units * 10)));
	return m_dataRate;
}
