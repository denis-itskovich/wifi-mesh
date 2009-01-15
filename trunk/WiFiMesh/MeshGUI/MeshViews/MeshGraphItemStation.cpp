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

MeshGraphItemStation::MeshGraphItemStation(MeshViewStations* pContainer, Station* pStation) :
	MeshItemStation(pContainer, pStation)
{
    setFlag(ItemIsMovable);
    setCacheMode(DeviceCoordinateCache);
}

QRectF MeshGraphItemStation::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-10 - adjust, -10 - adjust,
                  23 + adjust, 23 + adjust);
}

QPainterPath MeshGraphItemStation::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

void MeshGraphItemStation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-7, -7, 20, 20);

    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, QColor(Qt::yellow).light(120));
        gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
    } else {
        gradient.setColorAt(0, Qt::yellow);
        gradient.setColorAt(1, Qt::darkYellow);
    }
    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);
}

void MeshGraphItemStation::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::update();
    QGraphicsItem::mousePressEvent(event);
}

void MeshGraphItemStation::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::update();
    QGraphicsItem::mouseReleaseEvent(event);
}

QVariant MeshGraphItemStation::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change)
    {
    case QGraphicsItem::ItemPositionHasChanged:
		setLocation(pos());
		stationChanged();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void MeshGraphItemStation::updateStation()
{
	if (pos() != location())
	{
		setPos(location());
		update();
	}
}
