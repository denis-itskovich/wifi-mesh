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
 * @file MeshGraphItemVelocityHandle.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * @date 09/02/2009
 * @author Denis Itskovich
 */


#ifndef MESHGRAPHITEMVELOCITYHANDLE_H_
#define MESHGRAPHITEMVELOCITYHANDLE_H_

#include "MeshGraphItemArrow.h"
#include "MeshGraphItemStation.h"

class MeshGraphItemVelocityHandle : public QGraphicsItem
{
public:
    MeshGraphItemVelocityHandle(MeshGraphItemStation* item);
    ~MeshGraphItemVelocityHandle();

protected:
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    void init();
    void updateText();

    MeshGraphItemStation*   m_station;
    MeshGraphItemArrow*     m_arrow;
    QGraphicsTextItem*      m_text;
};

#endif /* MESHGRAPHITEMVELOCITYHANDLE_H_ */
