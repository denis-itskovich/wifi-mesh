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

#include <QtGui>
#include "../Items/MeshChartItem.h"

class MeshWidgetChart : public QWidget
{
public:
    MeshWidgetChart(QWidget* parent = NULL);

    void addItem(MeshChartItem* item);
    void removeItem(MeshChartItem* item);

protected:
    void paintEvent(QPaintEvent* event);
    void paintItem(QPainter* painter, MeshChartItem* item, const QRect& rect, double normalizedVal);
    void paintLegend(QPainter* painter);

    QRect itemRect(int index);
    const QRect& legendRect() const;
    const QRect& itemsRect() const;
    void updateItems();

private:
    void init();
    typedef QList<MeshChartItem*> ChartItems;

    ChartItems  m_items;
    int         m_spacing;
    double      m_maxVal;
    QRect       m_legendRect;
    QRect       m_itemsRect;
};

#endif /* MESHWIDGETCHART_H_ */
