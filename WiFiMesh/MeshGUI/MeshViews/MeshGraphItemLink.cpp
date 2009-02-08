/**
 * \file MeshGraphItemLink.cpp
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 08/02/2009
 * \author Denis Itskovich
 */

#include "MeshGraphItemLink.h"

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

MeshGraphItemLink::MeshGraphItemLink(MeshGraphItemStation* src, MeshGraphItemStation* dst) :
    m_src(src), m_dst(dst), m_arrowSize(10), m_penWidth(1)
{
    setAcceptedMouseButtons(0);
    updateLink();
}

QRectF MeshGraphItemLink::boundingRect() const
{
    qreal extra = (m_penWidth + m_arrowSize) / 2.0;
    return QRectF(m_srcPoint, QSizeF(m_dstPoint.x() - m_srcPoint.x(),
                                  m_dstPoint.y() - m_srcPoint.y()))
           .normalized()
           .adjusted(-extra, -extra, extra, extra);
}

void MeshGraphItemLink::updateLink()
{
    prepareGeometryChange();
    m_srcPoint = m_src->location();
    m_dstPoint = m_dst->location();
    QLineF line(m_srcPoint, m_dstPoint);
    QPointF offset(line.dx() * 10.0 / line.length(), line.dy() * 10.0 / line.length());
    m_srcPoint += offset;
    m_dstPoint -= offset;
}

void MeshGraphItemLink::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget* widget)
{
    // draw the line
    QLineF line(m_srcPoint, m_dstPoint);
    painter->setPen(QPen(Qt::red, m_penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    // draw the arrow
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0) angle = TwoPi - angle;

    QPointF dstArrowP1 = m_dstPoint + QPointF(sin(angle - Pi / 3) * m_arrowSize, cos(angle - Pi / 3) * m_arrowSize);
    QPointF dstArrowP2 = m_dstPoint + QPointF(sin(angle - Pi + Pi / 3) * m_arrowSize, cos(angle - Pi + Pi / 3) * m_arrowSize);

    painter->setBrush(Qt::red);
    painter->drawPolygon(QPolygonF() << line.p2() << dstArrowP1 << dstArrowP2);
}
