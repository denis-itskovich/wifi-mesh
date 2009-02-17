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

/*
 * MeshDocument.cpp
 *
 *  Created on: 21/08/2008
 *      Author: denis
 */

#include "MeshDocument.h"
#include "../Dialogs/MeshDlgAddPacket.h"
#include <cmath>
#include <cstdlib>

template <typename _Module, typename _Type>
static _Type getMemberValue(_Module* pModule, EStatus(*getter)(const _Module*, _Type* pVal))
{
    _Type tmp;
    CHECK(getter(pModule, &tmp));
    return tmp;
}

template <typename _Module, typename _Type>
static void setMemberValue(_Module* pModule, EStatus(*setter)(_Module*, _Type val), _Type val)
{
    CHECK(setter(pModule, val));
}

static const int RND_RESOLUTION = RAND_MAX;

MeshDocument::MeshDocument() :
	m_pCurStation(NULL),
	m_stationsCount(50),
	m_avgDataSize(65536),
	m_avgMsgCount(100),
	m_avgVelocity(30.0),
	m_duration(60),
	m_bStarted(false),
	m_bPaused(false),
	m_timerId(0),
	m_delay(1)
{
	CHECK(SettingsNew(&m_pSettings));
	CHECK(TimeLineNew(&m_pTimeLine));
	CHECK(SimulatorNew(&m_pSimulator, m_pSettings, m_pTimeLine));
	CHECK(SimulatorSetStationTracker(m_pSimulator, (StationTracker)&MeshDocument::stationTracker, this));
	CHECK(SimulatorSetPacketSniffer(m_pSimulator, (PacketSniffer)&MeshDocument::packetSniffer, this));
	CHECK(TimeLineSetEventTracker(m_pTimeLine, (EventTracker)&MeshDocument::eventTracker, this));
}

MeshDocument::~MeshDocument()
{
	CHECK(SimulatorDelete(&m_pSimulator));
	CHECK(TimeLineDelete(&m_pTimeLine));
	CHECK(SettingsDelete(&m_pSettings));
}

int     MeshDocument::dataRate() const { return getMemberValue(m_pSettings, &SettingsGetDataRate); }
double  MeshDocument::coverage() const { return getMemberValue(m_pSettings, &SettingsGetCoverage); }
Size    MeshDocument::worldSize() const {	return getMemberValue(m_pSettings, &SettingsGetWorldSize); }
double  MeshDocument::time() const { return getMemberValue(m_pTimeLine, &TimeLineGetTime); }
double  MeshDocument::packetRetryTimeout() const { return getMemberValue(m_pSettings, &SettingsGetPacketRetryTimeout); }
int     MeshDocument::packetRetryThreshold() const { return getMemberValue(m_pSettings, &SettingsGetPacketRetryThreshold); }
int     MeshDocument::packetHopsThreshold() const { return getMemberValue(m_pSettings, &SettingsGetPacketHopsThreshold); }
double  MeshDocument::routeExpirationTimeout() const { return getMemberValue(m_pSettings, &SettingsGetRouteExpirationTimeout); }
double  MeshDocument::routeRetryTimeout() const { return getMemberValue(m_pSettings, &SettingsGetRouteRetryTimeout); }

void    MeshDocument::setDataRate(int rate) { setMemberValue(m_pSettings, &SettingsSetDataRate, (unsigned long)rate); }
void    MeshDocument::setCoverage(double coverage) { setMemberValue(m_pSettings, &SettingsSetCoverage, coverage); }
void    MeshDocument::setWorldSize(Size size) { setMemberValue(m_pSettings, &SettingsSetWorldSize, size); emit worldSizeChanged(); }
void    MeshDocument::setPacketRetryTimeout(double timeout) { setMemberValue(m_pSettings, &SettingsSetPacketRetryTimeout, timeout); }
void    MeshDocument::setPacketRetryThreshold(int threshold) { setMemberValue(m_pSettings, &SettingsSetPacketRetryThreshold, threshold); }
void    MeshDocument::setPacketHopsThreshold(int threshold) { setMemberValue(m_pSettings, &SettingsSetPacketHopsThreshold, threshold); }
void    MeshDocument::setRouteExpirationTimeout(double timeout) { setMemberValue(m_pSettings, &SettingsSetRouteExpirationTimeout, timeout); }
void    MeshDocument::setRouteRetryTimeout(double timeout) { setMemberValue(m_pSettings, &SettingsSetRouteRetryTimeout, timeout); }

int     MeshDocument::stationCount() const { return m_stationsCount; }
int     MeshDocument::avgPacketCount() const { return m_avgMsgCount; }
int     MeshDocument::avgDataSize() const { return m_avgDataSize; }
double  MeshDocument::avgVelocity() const { return m_avgVelocity; }
double  MeshDocument::duration() const { return m_duration; }

void    MeshDocument::setStationCount(int count) { m_stationsCount = count; }
void    MeshDocument::setAvgDataSize(int dataSize) { m_avgDataSize = dataSize; }
void    MeshDocument::setAvgVelocity(double avgVelocity) { m_avgVelocity = avgVelocity; }
void    MeshDocument::setAvgPacketCount(int avgMsgCount) { m_avgMsgCount = avgMsgCount; }
void    MeshDocument::setDuration(double duration) { m_duration = duration; }

Station*    MeshDocument::currentStation() const { return m_pCurStation; }

void        MeshDocument::setCurrentStation(Station* pStation)
{
    if (pStation == m_pCurStation) return;
    m_pCurStation = pStation;
    emit currentStationChanged(pStation);
}

void MeshDocument::updateStation(Station* pStation) { emit stationUpdated(pStation); }
void MeshDocument::addStation() { Location location = {0}; addStation(location); }

void MeshDocument::addPacket()
{
	StationId id;
	if (!m_pCurStation) return;
	CHECK(StationGetId(m_pCurStation, &id));

	MeshDlgAddPacket addMsgDlg(m_stationIds, id);

	if (addMsgDlg.exec() == QDialog::Rejected) return;
	addPacket(m_pCurStation, addMsgDlg.time(), addMsgDlg.destination(), addMsgDlg.dataSize());
}

void MeshDocument::addStation(Location location)
{
	Station* pStation;
	Velocity velocity = generateVelocity();
	CHECK(StationNew(&pStation, velocity, location, m_pTimeLine, m_pSettings));
	CHECK(SimulatorAddStation(m_pSimulator, pStation));
	setCurrentStation(pStation);
}

void MeshDocument::removeStation()
{
	if (!m_pCurStation) return;

	CHECK(SimulatorRemoveStation(m_pSimulator, m_pCurStation));
	setCurrentStation(NULL);
}

void MeshDocument::clear()
{
    emit simulationCleared();
    CHECK(SimulatorClear(m_pSimulator));
}

void MeshDocument::generateStations()
{
    clear();
    QProgressDialog progress;
    progress.setMinimumWidth(400);
    progress.setLabel(new QLabel("Generating stations..."));
    progress.setCancelButtonText("Abort");
    progress.setRange(0, m_stationsCount);
    progress.setWindowModality(Qt::WindowModal);

    for (int i = 0; i < m_stationsCount; ++i)
    {
        progress.setValue(i);
        if (progress.wasCanceled()) break;

        Station* pStation;
        Velocity v = generateVelocity();
        Location l = generateLocation();

        CHECK(StationNew(&pStation, v, l, m_pTimeLine, m_pSettings));
        CHECK(SimulatorAddStation(m_pSimulator, pStation));
    }
}

void MeshDocument::generatePackets()
{
    int stationsCount = m_stations.count();
    if (stationsCount < 2) return;
    int msgCount = m_avgMsgCount * stationsCount;

    QProgressDialog progress;
    progress.setMinimumWidth(400);
    progress.setLabel(new QLabel("Generating packets..."));
    progress.setCancelButtonText("Abort");
    progress.setRange(0, msgCount);
    progress.setWindowModality(Qt::WindowModal);

    for (int i = 0; i < msgCount; ++i)
    {
        progress.setValue(i);
        if (progress.wasCanceled()) break;

        StationId dst;
        int srcIdx = rand(stationsCount);
        int dstIdx = rand(stationsCount - 1);
        if (dstIdx >= srcIdx) ++dstIdx;

        CHECK(StationGetId(m_stations[dstIdx], &dst));

        int dataSize = rand(m_avgDataSize * stationsCount * 2 / msgCount + 1);
        double time = rand(m_duration);

        addPacket(m_stations[srcIdx], time, dst, dataSize);
    }

    progress.setLabelText("Updating views...");
    emit worldChanged();
    progress.setValue(msgCount);
}

void MeshDocument::generateWorld()
{
    generateStations();
    generatePackets();
}

void MeshDocument::addPacket(Station* pStation, double time, StationId dst, unsigned long size)
{
	StationId src;
	CHECK(StationGetId(pStation, &src));
	Packet* pPacket;
	CHECK(PacketNewData(&pPacket, src, dst, size));
	CHECK(StationSchedulePacket(pStation, pPacket, time));
}

void MeshDocument::reset()
{
    emit simulationReset();
    CHECK(SimulatorReset(m_pSimulator));
}

void MeshDocument::prepare()
{
    reset();
    m_bStarted = true;
    emit simulationStarted();
    emit statusChanged("Simulation started...");
}

void MeshDocument::start()
{
    if (!m_bStarted) prepare();
    if (m_bPaused) step();
    else resume();
}

void MeshDocument::stop()
{
	pause();
	m_bStarted = false;
	emit simulationStopped();
	const Statistics* pStatistics;
	CHECK(SimulatorGetStatistics(m_pSimulator, &pStatistics));
	emit statisticsUpdated(pStatistics);
}

void MeshDocument::togglePause(bool paused)
{
	if (m_bPaused == paused) return;
	m_bPaused = paused;
	if (!m_bStarted) return;
	if (!m_bPaused) resume();
	else pause();
}

void MeshDocument::setSpeed(int speed)
{
    m_delay = maximumSpeed() + 1 - speed;
    if (m_timerId)
    {
        pause();
        resume();
    }
}

int MeshDocument::speed() const
{
    return maximumSpeed() + 1 - m_delay;
}

void MeshDocument::pause()
{
	killTimer(m_timerId);
	m_timerId = 0;
}


void MeshDocument::resume()
{
	m_timerId = startTimer(m_delay);
}


void MeshDocument::step()
{
    if (SimulatorProcess(m_pSimulator) == eSTATUS_COMMON_OK)
        emit timeChanged(QString::QString("Time: %1 [msec]").arg(time() * 1000, 0, 'f', 5));
    else stop();
}

double MeshDocument::rand(double limit)
{
	return (double)(::rand() % RND_RESOLUTION) * limit / (double)RND_RESOLUTION;
}

int MeshDocument::rand(int limit)
{
	return ::rand() % limit;
}

Velocity MeshDocument::generateVelocity() const
{
	Velocity vel;
	double limit = m_avgVelocity * 4.0 / sqrt(2.0);
	vel.x = rand(limit) - limit / 2.0;
	vel.y = rand(limit) - limit / 2.0;
	return vel;
}

Location MeshDocument::generateLocation() const
{
	Location l;
	Size s = worldSize();
	l.x = rand(s.x) - s.x / 2.0;
	l.y = rand(s.y) - s.y / 2.0;
	return l;
}

void MeshDocument::stationTracker(Station* pStation, EStationEvent event, MeshDocument* pThis)
{
	StationId id;
	CHECK(StationGetId(pStation, &id));
	switch (event)
	{
	case eSTATION_ADDED:
		pThis->m_stationIds.push_back(id);
		pThis->m_stations.push_back(pStation);

		emit pThis->stationAdded(pStation);
		CHECK(StationRegisterRoutingHandler(pStation, (StationRoutingHandler)&MeshDocument::routingHandler, pThis));
		CHECK(StationRegisterSchedulerHandler(pStation, (StationSchedulerHandler)&MeshDocument::schedulerHandler, pThis));
		break;
	case eSTATION_REMOVED:
		pThis->m_stationIds.removeOne(id);
		pThis->m_stations.removeOne(pStation);
		CHECK(StationRegisterRoutingHandler(pStation, NULL, NULL));
		CHECK(StationRegisterSchedulerHandler(pStation, NULL, NULL));
		emit pThis->currentStationChanged(NULL);
		emit pThis->stationRemoved(pStation);
		break;
	case eSTATION_UPDATED: emit pThis->stationUpdated(pStation); break;
	}
}

void MeshDocument::packetSniffer(const Packet* pPacket, const Station* pSrc, const Station* pDst, EPacketStatus status, MeshDocument* pThis)
{
    StationId id;

    if (pDst) CHECK(StationGetId(pDst, &id));
    else id = pPacket->header.transitDstId;

    switch (status)
    {
    case ePKT_STATUS_PENDING:
        emit pThis->beginTransmit(pSrc, pDst, pPacket);
        break;
    case ePKT_STATUS_DELIVERED:
    case ePKT_STATUS_COLLISION:
        emit pThis->packetDispatched(pPacket, id, status);
        emit pThis->endTransmit(pDst);
        break;
    default: break;
    }
}

void MeshDocument::eventTracker(double time, const Packet* pPacket, bool isAdded, MeshDocument* pThis)
{

}

void MeshDocument::routingHandler(	const Station* pStation,
									StationId destId,
									StationId transId,
									double expirationTime,
									int length,
									ERouteEntryUpdate updateAction,
									MeshDocument *pThis)
{
	switch (updateAction)
	{
	case eROUTE_ADD: emit pThis->routeEntryAdded(pStation, destId, transId, expirationTime, length); break;
	case eROUTE_UPDATE: emit pThis->routeEntryUpdated(pStation, destId, transId, expirationTime, length); break;
	case eROUTE_REMOVE: emit pThis->routeEntryExpired(pStation, destId); break;
	}
}

void MeshDocument::schedulerHandler(	const Station* pStation,
										double time,
										const Packet* pPacket,
										ESchedulerEvent event,
										MeshDocument* pThis)
{
	switch (event)
	{
	case eSCHEDULE_ADDED: emit pThis->scheduleEntryAdded(pStation, time, pPacket); break;
	case eSCHEDULE_DELIVERED: emit pThis->scheduleEntryDelivered(pStation, pPacket); break;
	case eSCHEDULE_REMOVED: emit pThis->scheduleEntryRemoved(pStation, pPacket); break;
	}
}

void MeshDocument::timerEvent(QTimerEvent*)
{
	step();
}
