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
class MeshDocument;

class MeshItemStation
{
protected:
	enum ValidFlags
	{
		LocationFlag = 	1,
		VelocityFlag = 	2,
		RoutingFlag =	4,
		SchedulerFlag = 8,
		AllValid =		LocationFlag | VelocityFlag | RoutingFlag | SchedulerFlag
	};
public:
	MeshItemStation(MeshViewStations* pContainer, Station* pStation);
	virtual ~MeshItemStation() {}

	virtual void updateStation();
	virtual void stationChanged();
	virtual void reset();

	virtual void addRouteEntry(StationId dst, StationId trans, double expires, int length);
	virtual void updateRouteEntry(StationId dst, StationId trans, double expires, int length);
	virtual void removeRouteEntry(StationId dst);

	virtual void addScheduleEntry(double time, const Message* pMessage);
	virtual void removeScheduleEntry(double time, const Message* pMessage);

	Station* station() const;

protected:
	struct RouteEntry
	{
		RouteEntry() {}
		RouteEntry(StationId _dst, StationId _trans, double _expires, int _length);

		StationId 	destination;
		StationId 	transit;
		double		expires;
		int			length;
	};

	struct ScheduleEntry
	{
		ScheduleEntry() {}
		ScheduleEntry(double _time, StationId _dst, int _size);

		double 		time;
		StationId	destination;
		int			size;
	};

	QString name() const;
	StationId id() const;
	QPointF location() const;
	QPointF velocity() const;
	bool isCurrent() const;
	bool isActive() const;
	bool isValid(ValidFlags flag) const { return m_validFlags & flag; }
	bool isLocationValid() const { return isValid(LocationFlag); }
	bool isVelocityValid() const { return isValid(VelocityFlag); }
	bool isRoutingValid() const { return isValid(RoutingFlag); }
	bool isSchedulerValid() const { return isValid(SchedulerFlag); }
	void invalidate(ValidFlags flags) { m_validFlags &= ~flags; }
	void updateStation(ValidFlags flags) { invalidate(flags); updateStation(); }
	void makeCurrent();

	void setLocation(QPointF loc);
	void setVelocity(QPointF vel);

	MeshDocument* document() const;

protected:
	typedef QMap<StationId, RouteEntry> RouteMap;
	typedef QList<ScheduleEntry> ScheduleList;

	const RouteMap& routing() const { return m_routing; }
	const ScheduleList& schedule() const { return m_schedule; }

private:
	MeshViewStations*	m_pContainer;
	Station* 			m_pStation;
	RouteMap			m_routing;
	ScheduleList		m_schedule;

	unsigned			m_validFlags;
};

#endif /* MESHSTATIONITEM_H_ */
