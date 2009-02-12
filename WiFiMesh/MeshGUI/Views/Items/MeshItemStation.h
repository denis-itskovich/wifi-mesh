/*********************************************************************************
WiFi Mesh Simulator PRO
Copyright (C) 2009 Denis Itskovich

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*********************************************************************************/

/**
 * \file MeshStationItem.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 13/01/2009
 * \author Denis Itskovich
 */


#ifndef MESHSTATIONITEM_H_
#define MESHSTATIONITEM_H_

#include "../../Core/MeshCore.h"

class MeshViewStations;
class MeshDocument;

class MeshItemStation
{
public:
	MeshItemStation(MeshViewStations* pContainer, Station* pStation);
	virtual ~MeshItemStation() {}

	virtual void updateStation();
	virtual void stationChanged();
	virtual void reset();

	virtual void addRouteEntry(StationId dst, StationId trans, double expires, int length) {}
	virtual void updateRouteEntry(StationId dst, StationId trans, double expires, int length) {}
	virtual void removeRouteEntry(StationId dst) {}

	virtual void addScheduleEntry(double time, const Packet* pPacket) {}
	virtual void removeScheduleEntry(const Packet* pPacket) {}
	virtual void deliverScheduleEntry(const Packet* pPacket) {}

    virtual void beginTransmit();
    virtual void endTransmit();

	Station* station() const;
    QString name() const;
    StationId id() const;
    QPointF location() const;
    QPointF velocity() const;

    void setLocation(QPointF loc);
    void setVelocity(QPointF vel);
    QString locationString() const;
    QString velocityString() const;

    virtual void setCurrent(bool current) {}

protected:
	bool isCurrent() const;
	bool isActive() const;
	bool isTransmitting() const;
	void makeCurrent();

	MeshDocument* document() const;

private:
	MeshViewStations*  m_pContainer;
	Station*           m_pStation;
	unsigned           m_validFlags;
	int                m_transmitting;
};

#endif /* MESHSTATIONITEM_H_ */
