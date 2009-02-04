/*
 * MeshDocument.h
 *
 *  Created on: 21/08/2008
 *      Author: denis
 */

#ifndef MESHDOCUMENT_H_
#define MESHDOCUMENT_H_

#include "../CoreWrappers/MeshCore.h"
#include <QtCore>

class MeshDocument : public QObject
{
	Q_OBJECT

public:
	MeshDocument();
	virtual ~MeshDocument();

	Station* currentStation() const;

	int dataRate() const;
	double coverage() const;
	double routeTTL() const;
	Size worldSize() const;
	double duration() const;
	double time() const;

	int stationsCount() const;
	double avgVelocity() const;
	int avgDataSize() const;
	int avgMessagesCount() const;


public slots:
	void setDataRate(int dataRate);
	void setCoverage(double coverage);
	void setRouteTTL(double routeTTL);
	void setWorldSize(Size size);
	void setDuration(double duration);

	void setStationsCount(int count);
	void setAvgDataSize(int dataSize);
	void setAvgVelocity(double avgVelocity);
	void setAvgMessagesCount(int avgMsgCount);

	void setCurrentStation(Station* pStation);
	void updateStation(Station* pStation);

	void addStation();
	void addStation(Location loc);
	void removeStation();
	void generate();

	void start();
	void stop();
	void pause();
	void resume();
	void step();

signals:
	void currentStationChanged(Station* pStation);
	void stationRemoved(Station* pStation);
	void stationAdded(Station* pStation);
	void stationUpdated(Station* pStation);

	void routeEntryAdded(const Station* pStation, StationId dst, StationId trans, double expires, int length);
	void routeEntryUpdated(const Station* pStation, StationId dst, StationId trans, double expires, int length);
	void routeEntryExpired(const Station* pStation, StationId dst);

	void scheduleEntryAdded(const Station* pStation, double time, const Message* pMessage);
	void scheduleEntryRemoved(const Station* pStation, double time, const Message* pMessage);

	void messageDispatched(const Message* pMsg, StationId deliveredId);
	void worldSizeChanged();
	void worldChanged();

	void simulationStarted();
	void simulationCleared();
	void simulationStopped();

	void updatedStations();
	void updatedTimeLine();

private:
	virtual void timerEvent(QTimerEvent* event);
	Velocity generateVelocity() const;
	Location generateLocation() const;

	static double rand(double limit);
	static int rand(int limit);

	// callbacks
	static void stationTracker(Station* pStation, StationEventType eventType, MeshDocument* pThis);
	static void messageSniffer(double time, const Message* pMessage, const Station* pSrc, const Station* pDst, MeshDocument* pThis);
	static void eventTracker(double time, const Message* pMessage, bool isAdded, MeshDocument* pThis);

	static void routingHandler(	const Station* pStation,
								StationId destId,
								StationId transId,
								double expirationTime,
								int length,
								ERouteEntryUpdate updateAction,
								MeshDocument *pThis);

	static void schedulerHandler(	const Station* pStation,
									double time,
									const Message* pMessage,
									Boolean bAdded,
									MeshDocument* pThis);

	Simulator*	m_pSimulator;
	Settings*	m_pSettings;
	TimeLine*	m_pTimeLine;
	Station*	m_pCurStation;
	int			m_stationsCount;
	int			m_avgDataSize;
	int			m_avgMsgCount;
	double		m_avgVelocity;
	double		m_duration;
	bool		m_bStarted;
	bool		m_bPaused;
	int			m_timerId;
	int			m_messages;
};

#endif /* MESHDOCUMENT_H_ */