/*
 * Stations.h
 *
 *  Created on: 20/08/2008
 *      Author: denis
 */

#ifndef MESHDOCKSTATIONS_H_
#define MESHDOCKSTATIONS_H_

#include "MeshView.h"
#include "MeshItemStation.h"
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

	virtual void addRouteEntry(const Station* pStation, StationId dst, StationId trans, double expires, int length);
	virtual void updateRouteEntry(const Station* pStation, StationId dst, StationId trans, double expires, int length);
	virtual void removeRouteEntry(const Station* pStation, StationId dst);

	virtual void addScheduleEntry(const Station* pStation, double time, const Message* pMessage);
	virtual void removeScheduleEntry(const Station* pStation, double time, const Message* pMessage);

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

private:
	typedef QMap<Station*, MeshItemStation*> StationToItem;

	StationToItem		m_stationToItem;
};

#endif /* MESHDOCKSTATIONS_H_ */
