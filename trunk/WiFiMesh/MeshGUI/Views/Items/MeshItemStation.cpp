/**
 * \file MeshItemStation.cpp
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 13/01/2009
 * \author Denis Itskovich
 */

#include "MeshItemStation.h"
#include "../MeshViewStations.h"
#include <cassert>
#include <cmath>

MeshItemStation::MeshItemStation(MeshViewStations* pContainer, Station* pStation) :
	m_pContainer(pContainer),
	m_pStation(pStation),
	m_validFlags(0),
	m_transmitting(0)
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
	Velocity v;
	v.x = vel.x();
	v.y = vel.y();
	CHECK(StationSetVelocity(m_pStation, v));
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

bool MeshItemStation::isTransmitting() const
{
    return m_transmitting > 0;
}

bool MeshItemStation::isCurrent() const
{
	return m_pContainer->isCurrent(this);
}

void MeshItemStation::makeCurrent()
{
    if (!isCurrent()) m_pContainer->currentChanged(this);
}

void MeshItemStation::updateStation()
{
}

MeshDocument* MeshItemStation::document() const
{
	return m_pContainer->document();
}

QString MeshItemStation::locationString() const
{
    QPointF l = location();
    return QString("%1, %2").arg(l.x(), 0, 'f', 2).arg(l.y(), 0, 'f', 2);
}

QString MeshItemStation::velocityString() const
{
    QLineF line(QPointF(0, 0), velocity());
    qreal angle = line.angle();
    if (angle < 0) angle += 360;
    return QString("%1, %2\xb0").arg(line.length(), 0, 'f', 2).arg(round(angle));
}

void MeshItemStation::beginTransmit()
{
    if (!m_transmitting++) updateStation();
}

void MeshItemStation::endTransmit()
{
    assert(m_transmitting > 0);
    if (!--m_transmitting) updateStation();
}
