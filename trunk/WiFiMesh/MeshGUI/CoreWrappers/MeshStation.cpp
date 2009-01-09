/*
 * MeshStation.cpp
 *
 *  Created on: 20/08/2008
 *      Author: denis
 */

#include "MeshStation.h"

MeshStation::MeshStation(MeshSimulator& simulator, StationId id, Velocity velocity, Location location)
{
	CHECK(SimulatorAddStation(simulator.GetSimulator(), &m_pStation, id, location, velocity));
}

MeshStation::~MeshStation()
{
}

unsigned long MeshStation::id()
{
	StationId id;
	StationGetId(m_pStation, &id);
	return id;
}
