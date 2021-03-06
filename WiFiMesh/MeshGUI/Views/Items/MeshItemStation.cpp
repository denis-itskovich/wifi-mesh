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
 * @file MeshItemStation.cpp
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * @date 13/01/2009
 * @author Denis Itskovich
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
    return QString("%1, %2\xb0").arg(line.length(), 0, 'f', 2).arg(floor(angle));
}

int MeshItemStation::freeBufferSize() const
{
    int buffSize;
    CHECK(StationGetFreeBuffer(m_pStation, &buffSize));
    return buffSize;
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

void MeshItemStation::reset()
{
}
