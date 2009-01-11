/*
 * Randomizer.cpp
 *
 *  Created on: 21/08/2008
 *      Author: denis
 */

#include "DockRandomizer.h"

DockRandomizer::DockRandomizer(MeshDocument* document, QWidget* parent) :
	DockFrame(document, parent)
{
	init();
}

DockRandomizer::~DockRandomizer()
{
}

void DockRandomizer::init()
{
	QFormLayout* mainLayout = new QFormLayout();

	m_sliderStationsCount = new QSlider(Qt::Horizontal, this);
	m_sliderStationsCount->setTickPosition(QSlider::TicksBelow);
	m_spinStationsCount = new QSpinBox(this);

	connect(m_sliderStationsCount, SIGNAL(valueChanged(int)), m_spinStationsCount, SLOT(setValue(int)));
	connect(m_spinStationsCount, SIGNAL(valueChanged(int)), m_sliderStationsCount, SLOT(setValue(int)));

	m_spinAvgVelocity = new QDoubleSpinBox(this);
	m_spinAvgVelocity->setSingleStep(0.05);

	m_spinAvgDataSize = new QSpinBox(this);
	m_spinAvgMessagesCount = new QSpinBox(this);
	m_buttonGenerate = new QPushButton(tr("Generate"), this);

	QHBoxLayout* hlayout = new QHBoxLayout();
	hlayout->addWidget(m_sliderStationsCount);
	hlayout->addWidget(m_spinStationsCount);

	mainLayout->addRow(tr("Stations count:"), hlayout);
	mainLayout->addRow(tr("Average velocity:"), m_spinAvgVelocity);
	mainLayout->addRow(tr("Average data size:"), m_spinAvgDataSize);
	mainLayout->addRow(tr("Average messages count:"), m_spinAvgMessagesCount);
	mainLayout->addRow(m_buttonGenerate);

	setLayout(mainLayout);
}

void DockRandomizer::setDocument(MeshDocument* doc)
{
	disconnect(document());

	connect(m_spinAvgVelocity, SIGNAL(valueChanged(double)), doc, SLOT(setAvgVelocity(double)));
	connect(m_spinAvgDataSize, SIGNAL(valueChanged(int)), doc, SLOT(setAvgDataSize(int)));
	connect(m_spinAvgMessagesCount, SIGNAL(valueChanged(int)), doc, SLOT(setAvgMessagesCount(int)));
	connect(m_spinStationsCount, SIGNAL(valueChanged(int)), doc, SLOT(setStationsCount(int)));
	connect(m_buttonGenerate, SIGNAL(clicked()), doc, SLOT(generate()));

	DockFrame::setDocument(doc);
}
