/**
 * \file MeshTreeItemStation.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 13/01/2009
 * \author Denis Itskovich
 */


#ifndef MESHTREEITEMSTATION_H_
#define MESHTREEITEMSTATION_H_

#include "MeshItemStation.h"
#include <QtGui>

class MeshTreeItemStation : public MeshItemStation, public QTreeWidgetItem
{
	typedef QMap<StationId, QTreeWidgetItem*> 	RouteMap;
	typedef QMap<unsigned, QTreeWidgetItem*>	ScheduleMap;

public:
	MeshTreeItemStation(MeshViewStations* pContainer, Station* pStation);

protected:
	virtual void reset();
	virtual void addRouteEntry(StationId dst, StationId trans, double expires, int length);
	virtual void updateRouteEntry(StationId dst, StationId trans, double expires, int length);
	virtual void removeRouteEntry(StationId dst);

	virtual void addScheduleEntry(double time, const Packet* pPacket);
	virtual void removeScheduleEntry(const Packet* pPacket);
	virtual void deliverScheduleEntry(const Packet* pPacket);
	virtual void updateStation();

private:
	void initLocationNode(QTreeWidgetItem* item);
	void initVelocityNode(QTreeWidgetItem* item);
	void updateIcon();

	void init();

	QTreeWidgetItem* createRouteItem(StationId dst, StationId transit, double expires, int length);
	QTreeWidgetItem* createScheduleItem(double time, const Packet* pPacket);

	RouteMap	  m_routeMap;
	ScheduleMap	  m_scheduleMap;
	bool		  m_isActive;
	bool          m_isTransmitting;
};

#endif /* MESHTREEITEMSTATION_H_ */
