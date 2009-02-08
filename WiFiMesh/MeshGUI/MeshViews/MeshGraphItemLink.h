/**
 * \file MeshItemLink.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 08/02/2009
 * \author Denis Itskovich
 */


#ifndef MESHITEMLINK_H_
#define MESHITEMLINK_H_

#include <QtGui>
#include "MeshGraphItemStation.h"

class MeshGraphItemLink : public QGraphicsItem
{
public:
    MeshGraphItemLink(MeshGraphItemStation* src, MeshGraphItemStation* dst);
    void updateLink();

protected:
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget* widget);

private:
    MeshGraphItemStation*   m_src;
    MeshGraphItemStation*   m_dst;
    qreal                   m_arrowSize;
    qreal                   m_penWidth;
    QPointF                 m_srcPoint;
    QPointF                 m_dstPoint;
};

#endif /* MESHITEMLINK_H_ */
