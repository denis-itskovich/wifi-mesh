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
 * @file MeshWidgetChart.cpp
 * @date 16/02/2009
 * @author Denis Itskovich
 */

#include "MeshWidgetChart.h"

MeshWidgetChart::MeshWidgetChart(const QString& title, QWidget* parent) :
    QWidget(parent),
    m_title(title)
{
    init();
}

MeshWidgetChart::MeshWidgetChart(QWidget* parent) :
    QWidget(parent)
{
    init();
}

void MeshWidgetChart::init()
{
    m_spacing = 7;
    setFont(QFont("Tahoma", 12, QFont::Bold));
}

void MeshWidgetChart::addItem(MeshChartItem* item)
{
    if (item)
    {
        item->setParent(this);
        m_items << item;
        connect(item, SIGNAL(updateRequired()), this, SLOT(update()));
    }
}

void MeshWidgetChart::removeItem(MeshChartItem* item)
{
    if (item)
    {
        m_items.removeAll(item);
        disconnect(item);
        if (item->parent() == this) delete item;
    }
}

void MeshWidgetChart::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QPainter painter;
    updateItems();

    painter.begin(this);

    setFont(font());
    painter.setRenderHints(QPainter::TextAntialiasing);
    painter.drawText(m_titleRect, Qt::AlignCenter, m_title);

    int index = 0;
    foreach(MeshChartItem* item, m_items)
    {
        paintItem(&painter, item, itemRect(index++), m_maxVal != 0 ? item->value()/m_maxVal : 0);
    }

    paintLegend(&painter);

    painter.end();
}

const QRect& MeshWidgetChart::itemsRect() const
{
    return m_itemsRect;
}

const QRect& MeshWidgetChart::legendRect() const
{
    return m_legendRect;
}

void MeshWidgetChart::updateItems()
{
    int width = 0;
    int height = 0;
    m_maxVal = 0;

    foreach(MeshChartItem* item, m_items)
    {
        const QFont& font = item->font();
        QFontMetrics metrics(font);
        QSize size(metrics.size(Qt::TextSingleLine, item->title()));
        size.setWidth(size.width() + m_spacing * 2 + size.height() + 4);
        height += size.height() + 4;
        if (size.width() > width) width = size.width();
        if (item->value() > m_maxVal) m_maxVal = item->value();
    }

    QRect widgetRect(rect());

    height = QFontMetrics(font()).height();
    m_titleRect = widgetRect;
    m_titleRect.setBottom(m_titleRect.top() + height + m_spacing * 2);

    widgetRect.setTop(m_titleRect.bottom());

    QRect rect(widgetRect);
    rect.setLeft(rect.right() - width);

    m_legendRect = rect.adjusted(m_spacing, m_spacing, -m_spacing, -m_spacing);

    m_itemsRect = QRect(widgetRect.topLeft(), QSize(widgetRect.width() - rect.width(), widgetRect.height()))
                  .adjusted(m_spacing, m_spacing, -m_spacing, -m_spacing);
}

QRect MeshWidgetChart::itemRect(int index)
{
    int count = m_items.count();
    int itemWidth = (int)(((double)m_itemsRect.width() - (double)((count - 1) * m_spacing)) / (double)count + 0.5);

    return QRect(QPoint(m_itemsRect.left() + (itemWidth + m_spacing)*index, m_itemsRect.top()), QSize(itemWidth, m_itemsRect.height()));
}

void MeshWidgetChart::paintItem(QPainter* painter, MeshChartItem* item, const QRect& boundingRect, double normalizedVal)
{
    QRect itemRect(boundingRect);

    int fontHeight = QFontMetrics(item->font()).height();
    itemRect.setTop(itemRect.bottom() - (int)((double)(itemRect.height() - fontHeight - 4) * normalizedVal + 0.5));
    QRect textRect(boundingRect.left(), itemRect.top() - fontHeight, boundingRect.width(), fontHeight);

    QRect rightShadow(itemRect.right() + 1, itemRect.top() + 3, 3, itemRect.height());
    QRect bottomShadow(itemRect.left() + 3, itemRect.bottom() + 1, itemRect.width(), 3);

    itemRect.adjust(0, 0, -1, -1);

    painter->setBrush(QBrush(item->color()));
    painter->setPen(Qt::black);

    painter->drawRect(itemRect);
    painter->fillRect(rightShadow, Qt::darkGray);
    painter->fillRect(bottomShadow, Qt::darkGray);

    painter->setFont(item->font());
    painter->drawText(textRect, Qt::AlignHCenter | Qt::AlignTop | Qt::TextSingleLine, QString::number(item->value()));
}

void MeshWidgetChart::paintLegend(QPainter* painter)
{
    QFont fnt = font();
    fnt.setPointSizeF(fnt.pointSizeF() / 1.5);
    fnt.setWeight(QFont::Normal);
    painter->setFont(fnt);
    painter->drawText(m_legendRect, Qt::AlignHCenter | Qt::AlignTop | Qt::TextSingleLine, tr("Legend"));

    QPoint point(m_legendRect.topLeft());
    point.setY(point.y() + painter->fontMetrics().height() + 10 + m_spacing);

    foreach(MeshChartItem* item, m_items)
    {

        painter->setFont(item->font());
        int fontHeight = painter->fontMetrics().height();
        painter->fillRect(point.x(), point.y(), fontHeight, fontHeight, item->color());
        QRect textbox(QPoint(point.x() + fontHeight + 4, point.y()), QSize(m_legendRect.width() - fontHeight - 4, fontHeight));
        painter->drawText(textbox, item->title());
        point.setY(point.y() + fontHeight + 4);
    }
}
