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
 * Represents customizable chart
 *
 * @file MeshWidgetChart.h
 * @date 15/02/2009
 * @author Denis Itskovich
 */


#ifndef MESHWIDGETCHART_H_
#define MESHWIDGETCHART_H_

#include <QtGui/QtGui>
#include "../Items/MeshChartItem.h"

class MeshWidgetChart : public QWidget
{
    Q_OBJECT
public:
    MeshWidgetChart(const QString& title, QWidget* parent = NULL);
    MeshWidgetChart(QWidget* parent = NULL);

    void addItem(MeshChartItem* item);
    void removeItem(MeshChartItem* item);
    const QString& title() const;

    QAction* toggleViewAction();

public slots:
    void setTitle(const QString& title);
    void setVisible(bool visible);

protected:
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event);
    void paintItem(QPainter* painter, MeshChartItem* item, const QRect& rect, double normalizedVal);
    void paintLegend(QPainter* painter);

    const QRect& legendRect() const;
    const QRect& itemsRect() const;
    const QRect& titleRect() const;

private slots:
    void updateItems();
    void updateBounds();

private:
    QString itemText(MeshChartItem* item) const;
    QRect calculateItemsRect() const;
    QRect calculateLegendRect() const;
    QRect calculateTitleRect() const;
    QFont legendFont() const;
    void init();

    QString         m_title;
    QAction*        m_action;
    int             m_spacing;
    QRect           m_legendRect;
    QRect           m_itemsRect;
    QRect           m_titleRect;
    MeshChartItem   m_topItem;
};

#endif /* MESHWIDGETCHART_H_ */
