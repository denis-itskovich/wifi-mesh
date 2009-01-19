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

#include "../CoreWrappers/MeshCore.h"

class MeshViewStations;

class MeshItemStation
{
public:
	MeshItemStation(MeshViewStations* pContainer, Station* pStation);

	virtual void updateStation() = 0;
	virtual void stationChanged();
	Station* station() const;

protected:
	QString name() const;
	StationId id() const;
	QPointF location() const;
	QPointF velocity() const;
	bool isCurrent() const;
	bool isActive() const;
	void makeCurrent();

	void setLocation(QPointF loc);
	void setVelocity(QPointF vel);

private:
	MeshViewStations*	m_pContainer;
	Station* 			m_pStation;
};

#endif /* MESHSTATIONITEM_H_ */
