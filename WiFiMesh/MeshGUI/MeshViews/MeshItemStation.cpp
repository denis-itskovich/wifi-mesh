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

MeshItemStation::MeshItemStation(MeshViewStations* pContainer, Station* pStation) :
	m_pContainer(pContainer),
	m_pStation(pStation)
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
}

void MeshItemStation::setVelocity(QPointF vel)
{
	Location v;
	v.x = vel.x();
	v.y = vel.y();
	CHECK(StationSetLocation(m_pStation, v));
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
