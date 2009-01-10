/**
 * \file MeshSimulator.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 07/01/2009
 * \author Denis Itskovich
 */


#ifndef MESHSIMULATOR_H_
#define MESHSIMULATOR_H_

#include "MeshCore.h"
#include <QtCore/QObject>

class MeshSimulator : public QObject
{
	Q_OBJECT

public:
	MeshSimulator();
	virtual ~MeshSimulator();

	Simulator* getSimulator() { return m_pSimulator; }

public slots:
	void run();
	void pause();
	void resume();
	void stop();
	void step();

private:
	bool isStarted() const;
	bool isRunning() const;

	Settings*	m_pSettings;
	Simulator*	m_pSimulator;

	double		m_coverage;
	int			m_dataRate;
	double		m_routeTTL;
	bool		m_paused;
};

#endif /* MESHSIMULATOR_H_ */
