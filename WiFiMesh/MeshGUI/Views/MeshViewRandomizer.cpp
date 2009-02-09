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
	m_sliderStationsCount = new QSlider(Qt::Horizontal);
	m_sliderStationsCount->setTickPosition(QSlider::TicksBelow);
	m_sliderStationsCount->setRange(1, 1000);
	m_sliderStationsCount->setTickInterval(100);

	m_spinStationsCount = new QSpinBox;
	m_spinStationsCount->setRange(1, 1000);

	connect(m_sliderStationsCount, SIGNAL(valueChanged(int)), m_spinStationsCount, SLOT(setValue(int)));
	connect(m_spinStationsCount, SIGNAL(valueChanged(int)), m_sliderStationsCount, SLOT(setValue(int)));

	m_spinAvgVelocity = new QDoubleSpinBox;
	m_spinAvgVelocity->setSingleStep(0.05);
	m_spinAvgVelocity->setRange(0.01, 99.99);

	m_spinAvgDataSize = new QSpinBox;
	m_spinAvgDataSize->setRange(1, 1 << 30);
	m_spinAvgDataSize->setSuffix(" [Bytes]");

	m_spinAvgPacketsCount = new QSpinBox;
	m_spinAvgPacketsCount->setRange(1, 100000);

	m_spinDuration = new QDoubleSpinBox;
	m_spinDuration->setRange(0.1, 3600);
	m_spinDuration->setDecimals(1);
	m_spinDuration->setSuffix(tr(" [sec]"));

	m_buttonGenerate = new QPushButton(QIcon(":/generate.png"), tr("Generate mesh"));

	QHBoxLayout* hlayout = new QHBoxLayout;
	hlayout->addWidget(m_sliderStationsCount);
	hlayout->addWidget(m_spinStationsCount);

	QFormLayout* mainLayout = new QFormLayout;

	mainLayout->addRow(tr("Stations count:"), hlayout);
	mainLayout->addRow(tr("Average velocity:"), m_spinAvgVelocity);
	mainLayout->addRow(tr("Average data size:"), m_spinAvgDataSize);
	mainLayout->addRow(tr("Average packets count:"), m_spinAvgPacketsCount);
	mainLayout->addRow(tr("Duration:"), m_spinDuration);

	QGroupBox* group = new QGroupBox("Randomization parameters");
	group->setLayout(mainLayout);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(group);
	layout->addStretch();
	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(m_buttonGenerate, 0, Qt::AlignHCenter);

	layout->addItem(buttonLayout);

	setLayout(layout);
}

void MeshViewRandomizer::setDocument(MeshDocument* doc)
{
	disconnect(document());

	connect(m_spinAvgVelocity, SIGNAL(valueChanged(double)), doc, SLOT(setAvgVelocity(double)));
	connect(m_spinAvgDataSize, SIGNAL(valueChanged(int)), doc, SLOT(setAvgDataSize(int)));
	connect(m_spinAvgPacketsCount, SIGNAL(valueChanged(int)), doc, SLOT(setAvgPacketsCount(int)));
	connect(m_spinStationsCount, SIGNAL(valueChanged(int)), doc, SLOT(setStationsCount(int)));
	connect(m_buttonGenerate, SIGNAL(clicked()), doc, SLOT(generate()));
	connect(m_spinDuration, SIGNAL(valueChanged(double)), doc, SLOT(setDuration(double)));
	connect(doc, SIGNAL(simulationStarted()), this, SLOT(disable()));
	connect(doc, SIGNAL(simulationStopped()), this, SLOT(enable()));

	m_spinAvgVelocity->setValue(doc->avgVelocity());
	m_spinAvgDataSize->setValue(doc->avgDataSize());
	m_spinAvgPacketsCount->setValue(doc->avgPacketsCount());
	m_spinStationsCount->setValue(doc->stationsCount());
	m_spinDuration->setValue(doc->duration());

	MeshView::setDocument(doc);
}
