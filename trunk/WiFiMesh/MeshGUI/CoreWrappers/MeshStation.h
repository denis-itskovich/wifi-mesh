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
	MeshStation(MeshSimulator& simulator, StationId id, Velocity velocity, Location location);
	virtual ~MeshStation();

	unsigned long id();

private:
	Station* m_pStation;
};

#endif /* MESHSTATION_H_ */
