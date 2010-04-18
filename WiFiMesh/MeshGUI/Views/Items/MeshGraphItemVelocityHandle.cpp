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
 * @file MeshGraphItemVelocityHandle.cpp
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * @date 09/02/2009
 * @author Denis Itskovich
 */

#include "MeshGraphItemVelocityHandle.h"
#include <cmath>

MeshGraphItemVelocityHandle::MeshGraphItemVelocityHandle(MeshGraphItemStation* station) :
    QGraphicsItem(station),
    m_station(station)
{
    init();
}

void MeshGraphItemVelocityHandle::init()
{
    m_arrow = new MeshGraphItemArrow(Qt::green, 3, 1, 10.0, 5.0, m_station);
    m_text = new QGraphicsTextItem(this);
    QFont font(m_text->font());
    font.setPointSize(font.pointSize() - 1);
    m_text->setFont(font);
    setFlag(ItemIsMovable);
	setFlag(ItemSendsGeometryChanges);
    setZValue(2);
    setCursor(Qt::OpenHandCursor);
    setPos(m_station->velocity());
    updateText();
}

MeshGraphItemVelocityHandle::~MeshGraphItemVelocityHandle()
{
    delete m_arrow;
}

QRectF MeshGraphItemVelocityHandle::boundingRect() const
{
    qreal size = 5.0;
    qreal adjust = 1;
    return QRectF(- size - adjust, - size - adjust, size * 2.0 + adjust, size * 2.0 + adjust);
}

QPainterPath MeshGraphItemVelocityHandle::shape() const
{
    QPainterPath path;
    path.addEllipse(-5, -5, 10, 10);
    return path;
}

void MeshGraphItemVelocityHandle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QColor colIn(Qt::darkGreen), colOut(Qt::green);
    QRadialGradient gradient(-3, -3, 10);
    gradient.setColorAt(1, colOut.light(120));
    gradient.setColorAt(0, colIn.light(120));
    painter->setPen(QPen(Qt::NoPen));
    painter->setBrush(gradient);
    painter->drawEllipse(-5, -5, 10, 10);
}

void MeshGraphItemVelocityHandle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ClosedHandCursor);
    QGraphicsItem::mousePressEvent(event);
}

void MeshGraphItemVelocityHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::OpenHandCursor);
    QGraphicsItem::mouseReleaseEvent(event);
}

void MeshGraphItemVelocityHandle::updateText()
{
    m_text->setPlainText(m_station->velocityString());
}

QVariant MeshGraphItemVelocityHandle::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change)
    {
    case QGraphicsItem::ItemPositionHasChanged:
        m_station->setVelocity(value.toPointF());
        m_arrow->setEdges(QPointF(0, 0), value.toPointF());
        m_station->stationChanged();
        updateText();
        break;
    default:
        break;
    };
    return QGraphicsItem::itemChange(change, value);
}
