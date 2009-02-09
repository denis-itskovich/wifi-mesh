/**
 * \file MeshGraphItemArrow.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 09/02/2009
 * \author Denis Itskovich
 */


#ifndef MESHGRAPHITEMARROW_H_
#define MESHGRAPHITEMARROW_H_

#include <QtGui>

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
