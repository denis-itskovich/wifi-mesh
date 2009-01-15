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
public:
	MeshViewStations(QWidget* parent = 0);
	virtual ~MeshViewStations();

	virtual void setDocument(MeshDocument* document);

signals:
	virtual void currentChanged(Station* pStation);
	virtual void addStation();
	virtual void addStation(Location loc);

public slots:
	virtual void addStation(Station* pStation) = 0;
	virtual void removeStation(Station* pStation) = 0;
	virtual void setCurrent(Station* pStation) = 0;
	virtual void updateStation(Station* pStation) = 0;

protected:
	virtual MeshItemStation* findItem(Station* pStation);
	virtual Station* findStation(MeshItemStation* pItem);
	virtual void registerStation(Station* pStation, MeshItemStation* pItem);
	virtual void unregisterStation(Station* pStation);

	static QString stationName(Station* pStation);
	static StationId stationId(Station* pStation);
	static Location stationLocation(Station* pStation);
	static Velocity stationVelocity(Station* pStation);

private:
	typedef QMap<MeshItemStation*, Station*> ItemToStation;
	typedef QMap<Station*, MeshItemStation*> StationToItem;

	ItemToStation	m_itemToStation;
	StationToItem	m_stationToItem;
};

#endif /* MESHDOCKSTATIONS_H_ */
