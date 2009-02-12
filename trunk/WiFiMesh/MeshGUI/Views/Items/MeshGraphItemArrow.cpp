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
 * \file MeshGraphItemArrow.cpp
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 09/02/2009
 * \author Denis Itskovich
 */

#include "MeshGraphItemArrow.h"

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

MeshGraphItemArrow::MeshGraphItemArrow(QColor color, int arrowSize, int penWidth, qreal srcRadius, qreal dstRadius, QGraphicsItem* parent) :
    QGraphicsItem(parent),
    m_color(color),
    m_arrowSize(arrowSize),
    m_penWidth(penWidth),
    m_srcRadius(srcRadius),
    m_dstRadius(dstRadius)
{
}

QRectF MeshGraphItemArrow::boundingRect() const
{
    qreal extra = m_penWidth + m_arrowSize;
    return QRectF(m_src, QSizeF(m_dst.x() - m_src.x(), m_dst.y() - m_src.y()))
           .normalized()
           .adjusted(-extra, -extra, extra, extra);
}

void MeshGraphItemArrow::setEdges(QPointF src, QPointF dst)
{
    prepareGeometryChange();
    QLineF line(src, dst);
    QPointF srcOffset(line.dx() * m_srcRadius / line.length(), line.dy() * m_srcRadius / line.length());
    QPointF dstOffset(-line.dx() * m_dstRadius / line.length(), -line.dy() * m_dstRadius / line.length());
    m_src = src + srcOffset;
    m_dst = dst + dstOffset;
}

void MeshGraphItemArrow::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget* widget)
{
    // draw the line
    QLineF line(m_src, m_dst);
    painter->setPen(QPen(m_color, m_penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    // draw the arrow
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0) angle = TwoPi - angle;

    QPointF dstArrowP1 = m_dst + QPointF(sin(angle - Pi / 3) * m_arrowSize, cos(angle - Pi / 3) * m_arrowSize);
    QPointF dstArrowP2 = m_dst + QPointF(sin(angle - Pi + Pi / 3) * m_arrowSize, cos(angle - Pi + Pi / 3) * m_arrowSize);

    painter->setBrush(m_color);
    painter->drawPolygon(QPolygonF() << line.p2() << dstArrowP1 << dstArrowP2);
}

