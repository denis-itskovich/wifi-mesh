/*
 * MeshStation.cpp
 *
 *  Created on: 20/08/2008
 *      Author: denis
 */

#include "MeshStation.h"

MeshStation::MeshStation(Station station) : m_station(station)
{
}

MeshStation::~MeshStation()
{
}

unsigned long MeshStation::id()
{
	return m_station.id;
}
