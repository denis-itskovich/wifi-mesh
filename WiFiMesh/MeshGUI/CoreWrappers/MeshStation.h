/*
 * MeshStation.h
 *
 *  Created on: 20/08/2008
 *      Author: denis
 */

#ifndef MESHSTATION_H_
#define MESHSTATION_H_

#include "MeshCore.h"
#include "MeshSimulator.h"

class MeshStation
{
public:
	MeshStation(StationId id, Velocity velocity, Location location);
	virtual ~MeshStation();

	StationId id();
	Location location();
private:
	Location	m_location;
	Station*	m_pStation;
};

#endif /* MESHSTATION_H_ */
