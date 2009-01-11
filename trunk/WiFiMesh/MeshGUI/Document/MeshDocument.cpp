/*
 * MeshDocument.cpp
 *
 *  Created on: 21/08/2008
 *      Author: denis
 */

#include "MeshDocument.h"

MeshDocument::MeshDocument()
{
	CHECK(SettingsNew(&m_pSettings));
	CHECK(TimeLineNew(&m_pTimeLine));
	CHECK(SimulatorNew(&m_pSimulator, m_pSettings, m_pTimeLine));
}

MeshDocument::~MeshDocument()
{
	CHECK(SimulatorDelete(&m_pSimulator));
	CHECK(TimeLineDelete(&m_pTimeLine));
	CHECK(SettingsDelete(&m_pSettings));
}


void MeshDocument::setCoverage(double cov)
{
	CHECK(SettingsSetCoverage(m_pSettings, cov));
}

void MeshDocument::setDataRate(int rate)
{
	CHECK(SettingsSetDataRate(m_pSettings, (unsigned long)rate));
}

void MeshDocument::setRouteTTL(double ttl)
{
	CHECK(SettingsSetRoutingTTL(m_pSettings, ttl));
}

void MeshDocument::setStationsCount(int count)
{
	m_stationsCount = count;
}

void MeshDocument::setAvgDataSize(int dataSize)
{
	m_avgDataSize = dataSize;
}

void MeshDocument::setAvgVelocity(double avgVelocity)
{
	m_avgVelocity = avgVelocity;
}

void MeshDocument::setAvgMessagesCount(int avgMsgCount)
{
	m_avgMsgCount = avgMsgCount;
}

void MeshDocument::setCurrentStation(Station* pStation)
{
	m_pCurStation = pStation;
	emit currentStationChanged(pStation);
}

void MeshDocument::addStation()
{
	Station* pStation;
	Velocity velocity = {0};
	Location location = {0};
	CHECK(StationNew(&pStation, velocity, location, m_pTimeLine, m_pSettings));
	CHECK(SimulatorAddStation(m_pSimulator, pStation));
	emit stationAdded(pStation);
}

void MeshDocument::removeStation()
{
	if (!m_pCurStation) return;

	emit stationRemoved(m_pCurStation);
	CHECK(SimulatorRemoveStation(m_pSimulator, m_pCurStation));
	CHECK(StationDelete(&m_pCurStation));
}

void MeshDocument::generate()
{

}

void MeshDocument::start()
{
	if (m_bStarted) step();
	m_bStarted = true;
}

void MeshDocument::stop()
{
	m_bStarted = false;
}

void MeshDocument::pause()
{
	m_bPaused = true;
}

void MeshDocument::resume()
{
	m_bPaused = false;
}

void MeshDocument::step()
{
	if (m_bStarted && !m_bPaused)
	{
		CHECK(SimulatorProcess(m_pSimulator));
	}
}
