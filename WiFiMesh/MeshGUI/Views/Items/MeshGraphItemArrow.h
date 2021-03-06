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
 * @file MeshGraphItemArrow.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * @date 09/02/2009
 * @author Denis Itskovich
 */


#ifndef MESHGRAPHITEMARROW_H_
#define MESHGRAPHITEMARROW_H_

#include <QtGui/QtGui>

class MeshGraphItemArrow : public QGraphicsItem
{
public:
    MeshGraphItemArrow(QColor color, int arrowSize, int penWidth, qreal srcRadius, qreal dstRadius, QGraphicsItem* parent = 0);
    void setEdges(QPointF src, QPointF dst);

protected:
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget* widget);

private:
    QPointF m_src;
    QPointF m_dst;
    QColor  m_color;
    int     m_arrowSize;
    int     m_penWidth;
    qreal   m_srcRadius;
    qreal   m_dstRadius;
};

#endif /* MESHGRAPHITEMARROW_H_ */
