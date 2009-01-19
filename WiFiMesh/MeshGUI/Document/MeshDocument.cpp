/*
 * MeshDocument.cpp
 *
 *  Created on: 21/08/2008
 *      Author: denis
 */

#include "MeshDocument.h"
#include <cmath>
#include <cstdlib>

static const int RND_RESOLUTION = 10000;

MeshDocument::MeshDocument() :
	m_stationsCount(0),
	m_avgDataSize(0),
	m_avgMsgCount(0),
	m_avgVelocity(0.0)
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

Station* MeshDocument::currentStation() const
{
	return m_pCurStation;
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

void MeshDocument::setWorldSize(Size size)
{
	CHECK(SettingsSetWorldSize(m_pSettings, size));
}

int MeshDocument::dataRate() const
{
	unsigned long rate;
	CHECK(SettingsGetDataRate(m_pSettings, &rate));
	return (int)rate;
}

double MeshDocument::coverage() const
{
	double coverage;
	CHECK(SettingsGetCoverage(m_pSettings, &coverage));
	return coverage;
}

double MeshDocument::routeTTL() const
{
	double ttl;
	CHECK(SettingsGetRoutingTTL(m_pSettings, &ttl));
	return ttl;
}

Size MeshDocument::worldSize() const
{
	Size size;
	CHECK(SettingsGetWorldSize(m_pSettings, &size));
	return size;
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

void MeshDocument::updateStation(Station* pStation)
{
	emit stationUpdated(pStation);
}

void MeshDocument::addStation()
{
	Location location = {0};
	addStation(location);
}

void MeshDocument::addStation(Location location)
{
	Station* pStation;
	Velocity velocity = generateVelocity();
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

double MeshDocument::rand(double limit)
{
	return (double)(::rand() % RND_RESOLUTION) * limit / (double)RND_RESOLUTION;
}

Velocity MeshDocument::generateVelocity() const
{
	Velocity vel;
	double limit = m_avgVelocity * 4.0 / sqrt(2.0);
	vel.x = rand(limit) - limit / 2.0;
	vel.y = rand(limit) - limit / 2.0;
	return vel;
}
