/*
 * MeshDockRandomizer.cpp
 *
 *  Created on: 21/08/2008
 *      Author: denis
 */

#include "MeshDockRandomizer.h"

MeshDockRandomizer::MeshDockRandomizer()
{
}

MeshDockRandomizer::~MeshDockRandomizer()
{
}

void MeshDockRandomizer::init()
{
	QBoxLayout* mainLayout = new QVBoxLayout(this);
	QGridLayout* randomParamsLayout = new QGridLayout();
	randomParams->

	m_stationsCount = new QSlider(Qt::Horizontal, this);

	m_minmalVelocity = new QTextLine();
	m_maximalVelocity;

	m_minimalDataAmount;
	m_maximalDataAmount;

	mainLayout->addWidget(new QLabel(tr("Stations count")));
}
