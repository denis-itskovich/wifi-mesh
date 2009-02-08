/**
 * \file MeshGraphItemStation.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 14/01/2009
 * \author Denis Itskovich
 */


#ifndef MESHGRAPHITEMSTATION_H_
#define MESHGRAPHITEMSTATION_H_

#include "MeshItemStation.h"
#include <QtGui>

class MeshGraphItemStation : public MeshItemStation, public QGraphicsItem
{
public:
	MeshGraphItemStation(MeshViewStations* pContainer, Station* pStation);

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void updateStation();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void focusInEvent(QFocusEvent *event);
    virtual void focusOutEvent(QFocusEvent *event);

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    bool m_isMoving;
};

#endif /* MESHGRAPHITEMSTATION_H_ */
