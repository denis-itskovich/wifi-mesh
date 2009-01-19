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
	m_spinStationsCount = new QSpinBox;

	connect(m_sliderStationsCount, SIGNAL(valueChanged(int)), m_spinStationsCount, SLOT(setValue(int)));
	connect(m_spinStationsCount, SIGNAL(valueChanged(int)), m_sliderStationsCount, SLOT(setValue(int)));

	m_spinAvgVelocity = new QDoubleSpinBox;
	m_spinAvgVelocity->setSingleStep(0.05);

	m_spinAvgDataSize = new QSpinBox;
	m_spinAvgMessagesCount = new QSpinBox;
	m_buttonGenerate = new QPushButton(tr("Generate"));

	QHBoxLayout* hlayout = new QHBoxLayout;
	hlayout->addWidget(m_sliderStationsCount);
	hlayout->addWidget(m_spinStationsCount);

	QFormLayout* mainLayout = new QFormLayout;

	mainLayout->addRow(tr("Stations count:"), hlayout);
	mainLayout->addRow(tr("Average velocity:"), m_spinAvgVelocity);
	mainLayout->addRow(tr("Average data size:"), m_spinAvgDataSize);
	mainLayout->addRow(tr("Average messages count:"), m_spinAvgMessagesCount);

	QGroupBox* group = new QGroupBox("Randomization parameters");
	group->setLayout(mainLayout);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(group);
	layout->addStretch();
	layout->addWidget(m_buttonGenerate);

	setLayout(layout);
}

void MeshViewRandomizer::setDocument(MeshDocument* doc)
{
	disconnect(document());

	connect(m_spinAvgVelocity, SIGNAL(valueChanged(double)), doc, SLOT(setAvgVelocity(double)));
	connect(m_spinAvgDataSize, SIGNAL(valueChanged(int)), doc, SLOT(setAvgDataSize(int)));
	connect(m_spinAvgMessagesCount, SIGNAL(valueChanged(int)), doc, SLOT(setAvgMessagesCount(int)));
	connect(m_spinStationsCount, SIGNAL(valueChanged(int)), doc, SLOT(setStationsCount(int)));
	connect(m_buttonGenerate, SIGNAL(clicked()), doc, SLOT(generate()));

	MeshView::setDocument(doc);
}
