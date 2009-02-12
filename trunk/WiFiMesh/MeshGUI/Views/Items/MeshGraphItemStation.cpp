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
 * \file MeshGraphItemStation.cpp
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 14/01/2009
 * \author Denis Itskovich
 */

#include "MeshGraphItemStation.h"
#include "MeshGraphItemVelocityHandle.h"
#include "../../Document/MeshDocument.h"
#include <cmath>

MeshGraphItemStation::MeshGraphItemStation(MeshViewStations* pContainer, Station* pStation) :
	MeshItemStation(pContainer, pStation),
	m_handle(NULL)
{
    setFlag(ItemIsMovable);
    setAcceptHoverEvents(true);
//    setCacheMode(ItemCoordinateCache);
    setPos(location());
    setZValue(1);
    setCursor(Qt::OpenHandCursor);
}

MeshGraphItemStation::~MeshGraphItemStation()
{
    if (m_handle) delete m_handle;
}

QRectF MeshGraphItemStation::boundingRect() const
{
	qreal size = std::max(document()->coverage(), 10.0);
    qreal adjust = 2;
    return QRectF(- size - adjust, - size - adjust, size * 2.0 + adjust, size * 2.0 + adjust);
}

QPainterPath MeshGraphItemStation::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

void MeshGraphItemStation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QColor colIn(Qt::darkBlue), colOut(Qt::cyan);

	if (!isActive())
	{
		colOut = Qt::lightGray;
		colIn = Qt::gray;
	}

    if (isCurrent())
    {
        colIn = Qt::darkGreen;
        colOut = Qt::green;
    }

    if (isTransmitting())
	{
	    colOut = Qt::red;
	}


    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken)
    {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, colOut.light(120));
        gradient.setColorAt(0, colIn.light(120));
    }
    else
    {
        gradient.setColorAt(0, colOut);
        gradient.setColorAt(1, colIn);
    }

    painter->setPen(QPen(Qt::NoPen));

    if (isActive() && (option->state & QStyle::State_MouseOver || isCurrent()))
    {
        colOut.lighter(100);
        colOut.setAlpha(100);
        qreal coverage = document()->coverage();
        QRectF ellipseBounds(-coverage, -coverage, coverage * 2.0, coverage * 2.0);
        QRadialGradient gradient(0, 0, coverage);
        gradient.setColorAt(10.0 / coverage, colOut);
        colOut.setAlpha(20);
        gradient.setColorAt(1.0, colOut);
        painter->setBrush(gradient);
        painter->drawEllipse(ellipseBounds);
    }

    painter->setBrush(gradient);
    painter->drawEllipse(-10, -10, 20, 20);
    painter->setPen(QColor(Qt::white));
    QRectF rect(-10.0, -10.0, 20.0, 20.0);
    painter->setFont(QFont("Tahoma"));
    painter->drawText(rect, Qt::AlignCenter, QString("%1").arg(id()));
}

void MeshGraphItemStation::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    makeCurrent();
    m_isMoving = true;
    QGraphicsItem::update();
    setCursor(Qt::ClosedHandCursor);
    QGraphicsItem::mousePressEvent(event);
}


void MeshGraphItemStation::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_isMoving = false;
	QGraphicsItem::update();
    setCursor(Qt::OpenHandCursor);
    QGraphicsItem::mouseReleaseEvent(event);
}

QVariant MeshGraphItemStation::itemChange(GraphicsItemChange change, const QVariant &value)
{
	switch (change)
    {
    case QGraphicsItem::ItemPositionHasChanged:
		{
			if (location() == value.toPointF()) break;
			if (m_isMoving) setLocation(value.toPointF());
			stationChanged();
		}
        break;
    default:
        break;
    };
	return QGraphicsItem::itemChange(change, value);
}

void MeshGraphItemStation::setCurrent(bool isCurrent)
{
    if (isCurrent)
    {
        if (!m_handle) m_handle = new MeshGraphItemVelocityHandle(this);
        setZValue(2);
    }
    else
    {
        if (m_handle)
        {
            delete m_handle;
            m_handle = NULL;
        }
        setZValue(1);
    }
    QGraphicsItem::update();
}

void MeshGraphItemStation::updateStation()
{
	if (pos() != location())
	{
	    prepareGeometryChange();
		setPos(location());
	}
	MeshItemStation::updateStation();
}
