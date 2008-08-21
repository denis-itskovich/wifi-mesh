/*
 * MeshStation.h
 *
 *  Created on: 20/08/2008
 *      Author: denis
 */

#ifndef MESHSTATION_H_
#define MESHSTATION_H_

extern "C"
{
	#include "../../MeshCore/Station.h"
};

class MeshStation
{
public:
	MeshStation(Station station);
	virtual ~MeshStation();

	unsigned long id();

private:
	Station	m_station;
};

#endif /* MESHSTATION_H_ */
