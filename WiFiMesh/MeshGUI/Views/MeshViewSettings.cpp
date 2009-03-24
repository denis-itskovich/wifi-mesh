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

/**
 * @file MeshViewSettings.cpp
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * @date 11/01/2009
 * @author Denis Itskovich
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
	m_spinCoverage->setSingleStep(1);
	m_spinCoverage->setRange(0.01, 1000);
	m_spinCoverage->setSuffix(" [m]");
	connect(m_spinCoverage, SIGNAL(valueChanged(double)), this, SLOT(setCoverage(double)));

	m_spinMaximumAttenuation = new QDoubleSpinBox;
	m_spinMaximumAttenuation->setSingleStep(1);
	m_spinMaximumAttenuation->setSuffix(" [dB]");
	connect(m_spinMaximumAttenuation, SIGNAL(valueChanged(double)), this, SLOT(setMaxAttenuation(double)));

	m_spinAttenuationCoefficient = new QDoubleSpinBox;
	m_spinAttenuationCoefficient->setSingleStep(1);
	m_spinAttenuationCoefficient->setRange(0.01, 1000);
	connect(m_spinAttenuationCoefficient, SIGNAL(valueChanged(double)), this, SLOT(setAttenuationCoefficient(double)));

	m_spinRouteExpTimeout = new QDoubleSpinBox;
	m_spinRouteExpTimeout->setSingleStep(1);
	m_spinRouteExpTimeout->setMaximum(3600);
	m_spinRouteExpTimeout->setSuffix(tr(" [sec]"));

    m_spinRouteRetryTimeout = new QDoubleSpinBox;
    m_spinRouteRetryTimeout->setRange(0.1, 60.0);
    m_spinRouteRetryTimeout->setDecimals(1);
    m_spinRouteRetryTimeout->setSuffix(tr(" [sec]"));

    m_spinRouteRetryThreshold = new QSpinBox;
    m_spinRouteRetryThreshold->setRange(1, 100);

    m_spinRoutingTableSize = new QSpinBox;
    m_spinRoutingTableSize->setRange(1, 1000);

    m_spinPacketHopsThreshold = new QSpinBox;
    m_spinPacketHopsThreshold->setRange(1, MAX_PATH_LENGTH);

    m_spinPacketRetryTimeout = new QDoubleSpinBox;
    m_spinPacketRetryTimeout->setRange(0.01, 60.0);
    m_spinPacketRetryTimeout->setDecimals(2);
    m_spinPacketRetryTimeout->setSingleStep(0.01);
    m_spinPacketRetryTimeout->setSuffix(tr(" [sec]"));

    m_spinPacketRetryThreshold = new QSpinBox;
    m_spinPacketRetryThreshold->setRange(1, 100);

    m_spinRelayBufferSize = new QSpinBox;
    m_spinRelayBufferSize->setRange(0, 65536);
    m_spinRelayBufferSize->setSuffix(tr(" [bytes]"));

	m_spinDataRate = new QDoubleSpinBox;
	m_spinDataRate->setRange(0.1, 1 << 30);
	m_spinDataRate->setDecimals(1);

	m_checkSmoothenEvents = new QCheckBox("Smoothen events");

	m_comboDataUnits = new QComboBox;
	m_comboDataUnits->addItems(QStringList() << tr("Bit/s") << tr("KBit/s") << tr("MBit/s"));
	m_comboDataUnits->setCurrentIndex(1);
	connect(m_spinDataRate, SIGNAL(valueChanged(double)), this, SLOT(setDataRate(double)));
	connect(m_comboDataUnits, SIGNAL(currentIndexChanged(int)), this, SLOT(setUnits(int)));

	m_spinHeight = new QSpinBox;
    m_spinHeight->setRange(0, 10000);
    m_spinHeight->setSuffix(" [m]");

    m_spinWidth = new QSpinBox;
	m_spinWidth->setRange(0, 10000);
	m_spinWidth->setSuffix(" [m]");


	connect(m_spinHeight, SIGNAL(valueChanged(int)), this, SLOT(setHeight(int)));
	connect(m_spinWidth, SIGNAL(valueChanged(int)), this, SLOT(setWidth(int)));

	QLayout* dataRateLayout = new QHBoxLayout;
	dataRateLayout->addWidget(m_spinDataRate);
	dataRateLayout->addWidget(m_comboDataUnits);

	QFormLayout* sizeLayout = new QFormLayout;

	sizeLayout->addRow(tr("Width:"), m_spinWidth);
	sizeLayout->addRow(tr("Height:"), m_spinHeight);

	QFormLayout* simulatorLayout = new QFormLayout;
	simulatorLayout->addRow(tr("Coverage radius:"), m_spinCoverage);
	simulatorLayout->addRow(tr("Maximum attenuation:"), m_spinMaximumAttenuation);
	simulatorLayout->addRow(tr("Attenuation coefficient:"), m_spinAttenuationCoefficient);
	simulatorLayout->addRow(tr("Data rate:"), dataRateLayout);
	simulatorLayout->addRow(m_checkSmoothenEvents);

	QFormLayout* routeLayout = new QFormLayout;
    routeLayout->addRow(tr("Expiration timeout:"), m_spinRouteExpTimeout);
    routeLayout->addRow(tr("Retry timeout:"), m_spinRouteRetryTimeout);
    routeLayout->addRow(tr("Retry count threshold:"), m_spinRouteRetryThreshold);
    routeLayout->addRow(tr("Routing table size:"), m_spinRoutingTableSize);

    QFormLayout* packetLayout = new QFormLayout;
    packetLayout->addRow(tr("Hops count threshold:"), m_spinPacketHopsThreshold);
    packetLayout->addRow(tr("Retry timeout:"), m_spinPacketRetryTimeout);
    packetLayout->addRow(tr("Retry count threshold:"), m_spinPacketRetryThreshold);
    packetLayout->addRow(tr("Relay buffer size:"), m_spinRelayBufferSize);

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
    int dataRate = m_dataRate;
    double min = pow(10, -units);
    double max = (double)(1 << (10 * (3 - units)));
    double val = (double)dataRate / (double)(1 << (10 * units));

    m_spinDataRate->setDecimals(units);
    m_spinDataRate->setSingleStep(pow(10, -units));
    m_spinDataRate->setRange(min, max);
	m_spinDataRate->setValue(val);
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

	connect(m_spinRouteExpTimeout, SIGNAL(valueChanged(double)), doc, SLOT(setRouteExpirationTimeout(double)));
    connect(m_spinRouteRetryTimeout, SIGNAL(valueChanged(double)), doc, SLOT(setRouteRetryTimeout(double)));
    connect(m_spinRouteRetryThreshold, SIGNAL(valueChanged(int)), doc, SLOT(setRouteRetryThreshold(int)));
    connect(m_spinRoutingTableSize, SIGNAL(valueChanged(int)), doc, SLOT(setRoutingTableSize(int)));
	connect(m_spinPacketHopsThreshold, SIGNAL(valueChanged(int)), doc, SLOT(setPacketHopsThreshold(int)));
	connect(m_spinPacketRetryTimeout, SIGNAL(valueChanged(double)), doc, SLOT(setPacketRetryTimeout(double)));
	connect(m_spinPacketRetryThreshold, SIGNAL(valueChanged(int)), doc, SLOT(setPacketRetryThreshold(int)));
	connect(m_spinRelayBufferSize, SIGNAL(valueChanged(int)), doc, SLOT(setRelayBufferSize(int)));
	connect(m_checkSmoothenEvents, SIGNAL(toggled(bool)), doc, SLOT(setSmoothenMode(bool)));

    connect(this, SIGNAL(updateSize(Size)), doc, SLOT(setWorldSize(Size)));
    connect(doc, SIGNAL(simulationStarted()), this, SLOT(disable()));
    connect(doc, SIGNAL(simulationStopped()), this, SLOT(enable()));
    connect(this, SIGNAL(updateDataRate(int)), doc, SLOT(setDataRate(int)));

	MeshView::setDocument(doc);
}

void MeshViewSettings::updateView()
{
    MeshDocument* doc = document();
    m_spinPacketRetryThreshold->setValue(doc->packetRetryThreshold());
    m_spinPacketRetryTimeout->setValue(doc->packetRetryTimeout());
    m_spinPacketHopsThreshold->setValue(doc->packetHopsThreshold());
    m_spinRelayBufferSize->setValue(doc->relayBufferSize());
    m_spinRouteRetryTimeout->setValue(doc->routeRetryTimeout());
    m_spinRouteRetryThreshold->setValue(doc->routeRetryThreshold());
    m_spinAttenuationCoefficient->setValue(doc->attenuationCoefficient());
    m_spinCoverage->setValue(doc->coverage());
    m_spinRouteExpTimeout->setValue(doc->routeExpirationTimeout());
    m_spinRoutingTableSize->setValue(doc->routingTableSize());
    m_checkSmoothenEvents->setChecked(doc->smoothenMode());
    Size size = doc->worldSize();
    m_spinWidth->setValue((int)(size.x + 0.5));
    m_spinHeight->setValue((int)(size.y + 0.5));
    m_dataRate = doc->dataRate() * 8;
    setUnits(m_comboDataUnits->currentIndex());
}

int MeshViewSettings::dataRate()
{
	int units = m_comboDataUnits->currentIndex();
	m_dataRate = (int)(m_spinDataRate->value() * (double)(1 << (units * 10)));
	return m_dataRate;
}

void MeshViewSettings::setCoverage(double coverage)
{
    double atten = pow(coverage, 2) * document()->attenuationCoefficient();
    document()->setMaxAttenuation(atten);
    m_spinMaximumAttenuation->setValue(atten);
}

void MeshViewSettings::setMaxAttenuation(double atten)
{
    document()->setMaxAttenuation(atten);
    double coverage = sqrt(atten / document()->attenuationCoefficient());
    m_spinCoverage->setValue(coverage);
}

void MeshViewSettings::setAttenuationCoefficient(double coef)
{
    double coverage = document()->coverage();
    document()->setAttenuationCoefficient(coef);
    double atten = pow(coverage, 2) * coef;
    document()->setMaxAttenuation(atten);
    m_spinMaximumAttenuation->setRange(coef * pow(m_spinCoverage->minimum(), 2), coef * pow(m_spinCoverage->maximum(), 2));
    m_spinMaximumAttenuation->setValue(atten);
}
