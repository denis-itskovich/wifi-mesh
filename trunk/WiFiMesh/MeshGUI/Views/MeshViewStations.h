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

/*
 * Stations.h
 *
 *  Created on: 20/08/2008
 *      Author: denis
 */

#ifndef MESHDOCKSTATIONS_H_
#define MESHDOCKSTATIONS_H_

#include "MeshView.h"
#include "Items/MeshItemStation.h"
#include <QtCore>

class MeshViewStations : public MeshView
{
	Q_OBJECT

	friend class MeshItemStation;
public:
	MeshViewStations(QWidget* parent = 0);
	virtual ~MeshViewStations();

	virtual void setDocument(MeshDocument* document);

signals:
	virtual void currentChanged(Station* pStation);
	virtual void stationChanged(Station* pStation);
	virtual void addStation();
	virtual void addStation(Location loc);

public slots:
	virtual void addStation(Station* pStation);
	virtual void removeStation(Station* pStation);
	virtual void setCurrent(Station* pStation);
	virtual void updateStation(Station* pStation);
	virtual void updateItem(MeshItemStation* item);
	virtual void updateAll();
	virtual void resetStations();
	virtual void addStationTriggered();

	virtual void addRouteEntry(const Station* pStation, StationId dst, StationId trans, double expires, int length);
	virtual void updateRouteEntry(const Station* pStation, StationId dst, StationId trans, double expires, int length);
	virtual void removeRouteEntry(const Station* pStation, StationId dst);

	virtual void addScheduleEntry(const Station* pStation, double time, const Packet* pPacket);
	virtual void deliverScheduleEntry(const Station* pStation, const Packet* pPacket);
	virtual void removeScheduleEntry(const Station* pStation, const Packet* pPacket);

    virtual void beginTransmit(const Station* pSrc, const Station* pDst, const Packet* pPacket);
    virtual void endTransmit(const Station* pDst);

protected:
	virtual void currentChanged(MeshItemStation* item);
	virtual bool isCurrent(const MeshItemStation* item) const;
	virtual MeshItemStation* currentItem() const;
	virtual MeshItemStation* findItem(const Station* pStation) const;
	virtual void registerStation(Station* pStation, MeshItemStation* pItem);
	virtual void unregisterStation(Station* pStation);

	static QString stationName(Station* pStation);
	static StationId stationId(Station* pStation);
	static Location stationLocation(Station* pStation);
	static Velocity stationVelocity(Station* pStation);

	QAction*   m_actAddPacket;
	QAction*   m_actAddStation;
	QAction*   m_actRemoveStation;

private:
    void initActions();
	typedef QMap<Station*, MeshItemStation*>       StationToItem;
	typedef QMap<const Station*, const Station*>   TransmitMap;

	StationToItem      m_stationToItem;
	MeshItemStation*   m_currentItem;
	TransmitMap        m_transmits;
};

#endif /* MESHDOCKSTATIONS_H_ */
