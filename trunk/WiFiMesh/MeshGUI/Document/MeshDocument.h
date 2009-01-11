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

	Station* currentStation();

public slots:
	void setDataRate(int dataRate);
	void setCoverage(double coverage);
	void setRouteTTL(double routeTTL);

	void setStationsCount(int count);
	void setAvgDataSize(int dataSize);
	void setAvgVelocity(double avgVelocity);
	void setAvgMessagesCount(int avgMsgCount);

	void setCurrentStation(Station* pStation);

	void addStation();
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

	void updatedStations();
	void updatedTimeLine();

private:
	Simulator*	m_pSimulator;
	Settings*	m_pSettings;
	TimeLine*	m_pTimeLine;
	Station*	m_pCurStation;
	int			m_stationsCount;
	int			m_avgDataSize;
	int			m_avgMsgCount;
	double		m_avgVelocity;
	bool		m_bStarted;
	bool		m_bPaused;
};

#endif /* MESHDOCUMENT_H_ */
