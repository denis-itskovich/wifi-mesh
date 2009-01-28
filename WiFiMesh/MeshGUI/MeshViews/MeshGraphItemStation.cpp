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
#include "../Document/MeshDocument.h"
#include <cmath>

MeshGraphItemStation::MeshGraphItemStation(MeshViewStations* pContainer, Station* pStation) :
	MeshItemStation(pContainer, pStation)
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsFocusable);
    setAcceptHoverEvents(true);
//    setCacheMode(DeviceCoordinateCache);
    setPos(location());
    setZValue(1);
    setCursor(Qt::OpenHandCursor);
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
	QColor colIn(Qt::darkBlue), colOut(Qt::blue);

	if (!isActive())
	{
		colOut = Qt::lightGray;
		colIn = Qt::gray;
	}

    if (option->state & QStyle::State_HasFocus)
    {
//        QPointF vel = velocity();
//        qreal len = sqrt(pow(vel.x(), 2.0) + pow(vel.y(), 2.0));
//        if (len > 0)
//        {
//            QPointF srcPnt(vel.x() / len, vel.y() / len);
//            painter->drawLine(srcPnt, srcPnt + vel);
//        }
        colIn = Qt::darkGreen;
        colOut = Qt::green;
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

    if (option->state & QStyle::State_MouseOver)
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
    QGraphicsItem::update();
    setCursor(Qt::ClosedHandCursor);
    QGraphicsItem::mousePressEvent(event);
}


void MeshGraphItemStation::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::update();
    setCursor(Qt::OpenHandCursor);
    QGraphicsItem::mouseReleaseEvent(event);
}

void MeshGraphItemStation::focusInEvent(QFocusEvent *event)
{
	makeCurrent();
	setZValue(2);
	QGraphicsItem::focusInEvent(event);
	QGraphicsItem::update();
}

void MeshGraphItemStation::focusOutEvent(QFocusEvent *event)
{
	setZValue(1);
	QGraphicsItem::focusOutEvent(event);
	QGraphicsItem::update();
}

QVariant MeshGraphItemStation::itemChange(GraphicsItemChange change, const QVariant &value)
{
	switch (change)
    {
    case QGraphicsItem::ItemPositionHasChanged:
		setLocation(value.toPointF());
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
	}
	update();
}
