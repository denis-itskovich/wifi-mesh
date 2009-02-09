/**
 * \file MeshGraphItemVelocityHandle.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 09/02/2009
 * \author Denis Itskovich
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
