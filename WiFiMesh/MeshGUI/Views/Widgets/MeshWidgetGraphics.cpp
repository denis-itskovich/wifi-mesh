/*********************************************************************************
MeshGUI
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
 * TODO Fill file purpose and description
 * @file MeshWidgetGraphics.cpp
 * @date 17/02/2009
 * @author Denis Itskovich
 */

#include "MeshWidgetGraphics.h"

MeshWidgetGraphics::MeshWidgetGraphics(QWidget* parent) :
    QGraphicsView(parent)
{
}

MeshWidgetGraphics::~MeshWidgetGraphics()
{
    delete m_menu;
}

QPointF MeshWidgetGraphics::pos() const
{
    return m_pos;
}

void MeshWidgetGraphics::setMenu(QMenu* menu)
{
    m_menu = menu;
}

void MeshWidgetGraphics::contextMenuEvent(QContextMenuEvent* event)
{
    m_pos = mapToScene(event->pos());
    if (m_menu) m_menu->exec(event->globalPos());
}

void MeshWidgetGraphics::wheelEvent(QWheelEvent* event)
{
    qreal scaleFactor = pow(1.2, -event->delta() / 240.0);
    scale(scaleFactor, scaleFactor);
}

void MeshWidgetGraphics::mouseDoubleClickEvent(QMouseEvent* event)
{
    emit doubleClicked(mapToScene(event->pos()));
}

void MeshWidgetGraphics::mousePressEvent(QMouseEvent* event)
{
    if (itemAt(event->pos()) == NULL)
    {
        emit focusCleared();
    }
    QGraphicsView::mousePressEvent(event);
}

void MeshWidgetGraphics::keyPressEvent(QKeyEvent* event)
{
    qreal scaleFactor = 1.0;

    switch (event->key())
    {
    case Qt::Key_Plus: scaleFactor = 1.2; break;
    case Qt::Key_Minus: scaleFactor = 1/1.2; break;
    default: break;
    }

    if (scaleFactor != 1.0)
        scale(scaleFactor, scaleFactor);
    QGraphicsView::keyPressEvent(event);
}

void MeshWidgetGraphics::addItem(QGraphicsItem* item)
{
    if (item && scene()) scene()->addItem(item);
}

void MeshWidgetGraphics::removeItem(QGraphicsItem* item)
{
    if (item && scene()) scene()->removeItem(item);
}

void MeshWidgetGraphics::drawBackground(QPainter *painter, const QRectF &rect)
{
    QRectF sceneRect = this->sceneRect();

    // Shadow
//    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
//    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
//    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
//        painter->fillRect(rightShadow, Qt::darkGray);
//    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
//        painter->fillRect(bottomShadow, Qt::darkGray);
//
//    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
//    gradient.setColorAt(0, QRgb(0x00dfdfdf));
//    gradient.setColorAt(1, Qt::lightGray);
//    painter->fillRect(rect.intersect(sceneRect), gradient);
//    painter->setBrush(Qt::NoBrush);
//    painter->drawRect(sceneRect);

    painter->setPen(QColor(Qt::black));
    painter->fillRect(sceneRect, QBrush(QRgb(0x00dfdfdf)));
    painter->drawRect(sceneRect);
}

