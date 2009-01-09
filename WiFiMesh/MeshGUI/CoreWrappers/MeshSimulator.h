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

class MeshSimulator
{
public:
	MeshSimulator();
	virtual ~MeshSimulator();

	Simulator* GetSimulator() { return m_pSimulator; }

private:
	Settings*	m_pSettings;
	Simulator*	m_pSimulator;

	double		m_coverage;
	int			m_dataRate;
	double		m_routeTTL;
};

#endif /* MESHSIMULATOR_H_ */
