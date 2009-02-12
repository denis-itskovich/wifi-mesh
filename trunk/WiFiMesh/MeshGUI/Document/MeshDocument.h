/*
 * MeshDocument.h
 *
 *  Created on: 21/08/2008
 *      Author: denis
 */

#ifndef MESHDOCUMENT_H_
#define MESHDOCUMENT_H_

#include "../Core/MeshCore.h"
#include <QtCore>

class MeshDocument : public QObject
{
	Q_OBJECT

public:
	MeshDocument();
	virtual ~MeshDocument();

	Station* currentStation() const;

	int    dataRate() const;
	double coverage() const;
	Size   worldSize() const;
	double duration() const;
	double time() const;

	double routeExpirationTimeout() const;
	double routeRetryTimeout() const;

	int    packetHopsThreshold() const;
	int    packetRetryThreshold() const;
    double packetRetryTimeout() const;

	int    stationsCount() const;
	double avgVelocity() const;
	int    avgDataSize() const;
	int    avgPacketsCount() const;

	int    speed() const;
	int    maximumSpeed() const { return 200; }


public slots:
	void setDataRate(int dataRate);
	void setCoverage(double coverage);
	void setWorldSize(Size size);
	void setDuration(double duration);

	void setRouteExpirationTimeout(double timeout);
	void setRouteRetryTimeout(double timeout);

	void setPacketHopsThreshold(int threshold);
    void setPacketRetryThreshold(int threshold);
	void setPacketRetryTimeout(double timeout);

	void setStationsCount(int count);
	void setAvgDataSize(int dataSize);
	void setAvgVelocity(double avgVelocity);
	void setAvgPacketsCount(int avgMsgCount);

	void setCurrentStation(Station* pStation);
	void updateStation(Station* pStation);

	void addStation();
	void addStation(Location loc);
	void addPacket();
	void removeStation();
	void generateWorld();
	void generatePackets();
	void generateStations();

	void start();
	void stop();
	void pause();
	void resume();
	void step();
    void clear();
    void reset();
	void togglePause(bool paused);
	void setSpeed(int speed);

signals:
	void currentStationChanged(Station* pStation);
	void stationRemoved(Station* pStation);
	void stationAdded(Station* pStation);
	void stationUpdated(Station* pStation);

	void routeEntryAdded(const Station* pStation, StationId dst, StationId trans, double expires, int length);
	void routeEntryUpdated(const Station* pStation, StationId dst, StationId trans, double expires, int length);
	void routeEntryExpired(const Station* pStation, StationId dst);

	void scheduleEntryAdded(const Station* pStation, double time, const Packet* pPacket);
	void scheduleEntryDelivered(const Station* pStation, const Packet* pPacket);
	void scheduleEntryRemoved(const Station* pStation, const Packet* pPacket);

	void beginTransmit(const Station* pFrom, const Station* pTo, const Packet* pPacket);
	void endTransmit(const Station* pTo);

	void packetDispatched(const Packet* pPacket, StationId deliveredId, EPacketStatus status);

	void worldSizeChanged();
	void worldChanged();

	void simulationStarted();
	void simulationCleared();
	void simulationStopped();

	void updatedStations();
	void updatedTimeLine();

private:
    void prepare();
	virtual void timerEvent(QTimerEvent* event);
	Velocity generateVelocity() const;
	Location generateLocation() const;

	static void addPacket(Station* pStation, double time, StationId dst, unsigned long size);
	static double rand(double limit);
	static int rand(int limit);

	// callbacks
	static void stationTracker(Station* pStation, EStationEvent event, MeshDocument* pThis);
	static void packetSniffer(const Packet* pPacket, const Station* pSrc, const Station* pDst, EPacketStatus status, MeshDocument* pThis);
	static void eventTracker(double time, const Packet* pPacket, bool isAdded, MeshDocument* pThis);

	static void routingHandler(	const Station* pStation,
								StationId destId,
								StationId transId,
								double expirationTime,
								int length,
								ERouteEntryUpdate updateAction,
								MeshDocument *pThis);

	static void schedulerHandler(	const Station* pStation,
									double time,
									const Packet* pPacket,
									ESchedulerEvent event,
									MeshDocument* pThis);

	QList<StationId>   m_stationIds;
	QList<Station*>    m_stations;
	Simulator*         m_pSimulator;
	Settings*          m_pSettings;
	TimeLine*          m_pTimeLine;
	Station*           m_pCurStation;
	int                m_stationsCount;
	int                m_avgDataSize;
	int                m_avgMsgCount;
	double             m_avgVelocity;
	double             m_duration;
	bool               m_bStarted;
	bool               m_bPaused;
	int                m_timerId;
	int                m_packets;
	int                m_delay;
};

#endif /* MESHDOCUMENT_H_ */
