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
 * TODO Fill file purpose and description
 * @file MeshWidgetGtaphics.h
 * @date 17/02/2009
 * @author Denis Itskovich
 */

#ifndef MESHWIDGETGTAPHICS_H_
#define MESHWIDGETGTAPHICS_H_

#include <QtGui>

class MeshWidgetGraphics: public QGraphicsView
{
    Q_OBJECT
public:
    MeshWidgetGraphics(QWidget* parent = 0);
    ~MeshWidgetGraphics();

    void addItem(QGraphicsItem* item);
    void removeItem(QGraphicsItem* item);
    void setMenu(QMenu* menu);
    QPointF pos() const;

signals:
    void doubleClicked(QPointF pos);
    void focusCleared();

protected:
    virtual void contextMenuEvent(QContextMenuEvent* event);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void wheelEvent(QWheelEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void drawBackground(QPainter *painter, const QRectF &rect);

private:
    QMenu*  m_menu;
    QPointF m_pos;
};

#endif /* MESHWIDGETGTAPHICS_H_ */
