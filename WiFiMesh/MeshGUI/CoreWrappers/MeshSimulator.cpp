/**
 * \file MeshSimulator.cpp
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 09/01/2009
 * \author Denis Itskovich
 */

#include "MeshSimulator.h"

MeshSimulator::MeshSimulator() :
	m_pSettings(NULL),
	m_pSimulator(NULL),
	m_coverage(10),
	m_dataRate(65536),
	m_routeTTL(20)
{
}

MeshSimulator::~MeshSimulator()
{

}

void MeshSimulator::run()
{
	if (isStarted())
	{
		step();
		return;
	}
	CHECK(SettingsNew(&m_pSettings, m_coverage, m_dataRate, m_routeTTL));
	CHECK(SimulatorNew(&m_pSimulator, m_pSettings));
}

void MeshSimulator::stop()
{
	if (!isStarted()) return;
	CHECK(SimulatorDelete(&m_pSimulator));
	CHECK(SettingsDelete(&m_pSettings));
}

void MeshSimulator::pause()
{
	m_paused = true;
}

void MeshSimulator::resume()
{
	m_paused = false;
}

void MeshSimulator::step()
{
	if (!isStarted() || !isRunning()) return;
	CHECK(SimulatorProcess(m_pSimulator));
}

bool MeshSimulator::isStarted() const
{
	return (m_pSimulator != NULL);
}

bool MeshSimulator::isRunning() const
{
	return !m_paused;
}
