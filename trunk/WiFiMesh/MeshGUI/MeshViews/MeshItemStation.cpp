/**
 * \file MeshItemStation.cpp
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 13/01/2009
 * \author Denis Itskovich
 */

#include "MeshViewStations.h"
#include <cassert>

MeshItemStation::MeshItemStation(MeshViewStations* pContainer, Station* pStation) :
	m_pContainer(pContainer),
	m_pStation(pStation),
	m_validFlags(0)
{
}

MeshItemStation::RouteEntry::RouteEntry(StationId _dst, StationId _trans, double _expires, int _length) :
	destination(_dst),
	transit(_trans),
	expires(_expires),
	length(_length)
{
}

MeshItemStation::ScheduleEntry::ScheduleEntry(double _time, StationId _dst, int _size) :
	time(_time),
	destination(_dst),
	size(_size)
{
}

QString MeshItemStation::name() const
{
	return QString("Station ") + QString::number(id());
}

StationId MeshItemStation::id() const
{
	StationId _id;
	CHECK(StationGetId(m_pStation, &_id));
	return _id;
}

QPointF MeshItemStation::location() const
{
	Location loc;
	CHECK(StationGetLocation(m_pStation, &loc));
	return QPointF(loc.x, loc.y);
}

QPointF MeshItemStation::velocity() const
{
	Velocity vel;
	CHECK(StationGetVelocity(m_pStation, &vel));
	return QPointF(vel.x, vel.y);
}

void MeshItemStation::setLocation(QPointF loc)
{
	Location l;
	l.x = loc.x();
	l.y = loc.y();
	CHECK(StationSetLocation(m_pStation, l));
	updateStation(LocationFlag);
}

void MeshItemStation::setVelocity(QPointF vel)
{
	Location v;
	v.x = vel.x();
	v.y = vel.y();
	CHECK(StationSetLocation(m_pStation, v));
	updateStation(VelocityFlag);
}

void MeshItemStation::stationChanged()
{
	m_pContainer->updateItem(this);
}

Station* MeshItemStation::station() const
{
	return m_pStation;
}

bool MeshItemStation::isActive() const
{
	Boolean bIsActive;
	CHECK(StationIsActive(m_pStation, &bIsActive));
	return bIsActive != FALSE;
}

bool MeshItemStation::isCurrent() const
{
	return m_pContainer->isCurrent(this);
}

void MeshItemStation::makeCurrent()
{
	m_pContainer->currentChanged(this);
}

void MeshItemStation::addRouteEntry(StationId dst, StationId trans, double expires, int length)
{
	assert(m_routing.count(dst) == 0);
	m_routing[dst] = RouteEntry(dst, trans, expires, length);
	invalidate(RoutingFlag);
}

void MeshItemStation::updateRouteEntry(StationId dst, StationId trans, double expires, int length)
{
	assert(m_routing.count(dst) != 0);
	m_routing[dst] = RouteEntry(dst, trans, expires, length);
	invalidate(RoutingFlag);
}

void MeshItemStation::removeRouteEntry(StationId dst)
{
	assert(m_routing.count(dst) != 0);
	invalidate(RoutingFlag);
}

void MeshItemStation::addScheduleEntry(double time, const Message* pMessage)
{
	m_schedule << ScheduleEntry(time, pMessage->originalDstId, pMessage->size);
	invalidate(SchedulerFlag);
}

void MeshItemStation::removeScheduleEntry(double time, const Message* pMessage)
{
	invalidate(SchedulerFlag);
}

void MeshItemStation::reset()
{
	m_routing.clear();
	updateStation();
}

void MeshItemStation::updateStation()
{
	m_validFlags = SchedulerFlag | RoutingFlag;
}

MeshDocument* MeshItemStation::document() const
{
	return m_pContainer->document();
}
