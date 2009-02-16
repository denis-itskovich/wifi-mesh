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
 * @file MeshViewChart.cpp
 * @date 16/02/2009
 * @author Denis Itskovich
 */

#include "MeshViewChart.h"

MeshViewChart::MeshViewChart(QWidget* parent) :
    MeshView(parent)
{
    init();
}

void MeshViewChart::init()
{

}

void MeshViewChart::addItem(MeshChartItem* item)
{
    if (item) m_items << item;
}

void MeshViewChart::removeItem(MeshChartItem* item)
{
    if (item) m_items.removeAll(item);
}

void MeshViewChart::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QPainter painter;
    updateItems();

    painter.begin(this);

    int index = 0;
    foreach(MeshChartItem* item, m_items)
    {
        paintItem(&painter, item, itemRect(index++), item->value()/m_maxVal);
    }

    paintLegend(&painter);

    painter.end();
}

const QRect& MeshViewChart::itemsRect() const
{
    return m_itemsRect;
}

const QRect& MeshViewChart::legendRect() const
{
    return m_legendRect;
}

void MeshViewChart::updateItems()
{
    int width = 0;
    int height = 0;
    m_maxVal = 0;

    foreach(MeshChartItem* item, m_items)
    {
        const QFont& font = item->font();
        QFontMetrics metrics(font);
        QSize size(metrics.size(Qt::TextSingleLine, item->title()));
        size += QSize(metrics.height() + 4, 4);
        height += size.height();
        if (size.width() > width) width = size.width();
        if (item->value() > m_maxVal) m_maxVal = item->value();
    }

    QRect widgetRect(rect());
    QRect rect(widgetRect);

    rect.setLeft(rect.right() - width);
    m_legendRect = rect.adjusted(m_spacing, m_spacing, -m_spacing, -m_spacing);

    m_itemsRect = QRect(widgetRect.topLeft(), QSize(widgetRect.width() - m_legendRect.width(), widgetRect.height()))
                  .adjusted(m_spacing, m_spacing, -m_spacing, -m_spacing);
}

QRect MeshViewChart::itemRect(int index)
{
    int count = m_items.count();
    int itemWidth = (int)(((double)m_itemsRect.width() - (double)((count - 1) * m_spacing)) / (double)count + 0.5);

    return QRect(QPoint(m_itemsRect.left() + (itemWidth + m_spacing)*index, m_itemsRect.top()), QSize(itemWidth, m_itemsRect.height()));
}

void MeshViewChart::paintItem(QPainter* painter, MeshChartItem* item, const QRect& boundingRect, double normalizedVal)
{
    QRect rect(boundingRect);
    rect.setTop(rect.bottom() - (int)((double)rect.height() * normalizedVal + 0.5));

    QRect rightShadow(rect.right(), rect.top() + 2, 2, rect.height());
    QRect bottomShadow(rect.left() + 2, rect.bottom(), rect.width(), 2);

    painter->fillRect(rightShadow, Qt::darkGray);
    painter->fillRect(bottomShadow, Qt::darkGray);

    painter->setBrush(QBrush(item->color()));
    painter->setPen(Qt::black);

    painter->drawRect(rect);
}

void MeshViewChart::paintLegend(QPainter* painter)
{
    painter->setFont(font());
    painter->drawText(m_legendRect, Qt::AlignCenter | Qt::AlignTop | Qt::TextSingleLine, tr("Legend"));

    QPoint point(m_legendRect.topLeft());
    point.setY(point.y() + painter->fontMetrics().height() + 10);

    foreach(MeshChartItem* item, m_items)
    {
        painter->setFont(item->font());
        int fontHeight = painter->fontMetrics().height();
        painter->fillRect(point.x(), point.y(), fontHeight, fontHeight, item->color());
        painter->drawText(point.x() + fontHeight + 4, point.y(), item->title());
    }
}
