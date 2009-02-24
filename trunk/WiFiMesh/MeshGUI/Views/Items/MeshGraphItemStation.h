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
 * @file MeshGraphItemStation.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * @date 14/01/2009
 * @author Denis Itskovich
 */


#ifndef MESHGRAPHITEMSTATION_H_
#define MESHGRAPHITEMSTATION_H_

#include "MeshItemStation.h"
#include <QtGui>

class MeshGraphItemStation : public MeshItemStation, public QGraphicsItem
{
public:
	MeshGraphItemStation(MeshViewStations* pContainer, Station* pStation);
	~MeshGraphItemStation();

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void updateStation();

protected:
    virtual void setCurrent(bool isCurrent);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    bool updateColors();

    QGraphicsItem*  m_handle;
    bool            m_isMoving;
    QColor          m_lastInnerColor;
    QColor          m_lastOuterColor;
};

#endif /* MESHGRAPHITEMSTATION_H_ */
