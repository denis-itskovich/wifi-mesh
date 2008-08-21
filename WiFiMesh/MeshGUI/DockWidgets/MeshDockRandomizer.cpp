/*
 * MeshDockRandomizer.cpp
 *
 *  Created on: 21/08/2008
 *      Author: denis
 */

#include "MeshDockRandomizer.h"

MeshDockRandomizer::MeshDockRandomizer(QWidget* parent) : MeshDock(tr("Randomizer"), parent)
{
}

MeshDockRandomizer::~MeshDockRandomizer()
{
}

void MeshDockRandomizer::init()
{
	QBoxLayout* mainLayout = new QVBoxLayout(this);
	QGridLayout* randomParamsLayout = new QGridLayout();

	m_sliderStationsCount = new QSlider(Qt::Horizontal, this);

	m_textMinVelocity = new QTextLine();
	m_textMaxVelocity = new QTextLine();

	mainLayout->addWidget(new QLabel(tr("Stations count")));
}
