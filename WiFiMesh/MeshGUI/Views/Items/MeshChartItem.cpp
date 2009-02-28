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
 * @file MeshChartItem.cpp
 * @date 28/02/2009
 * @author Denis Itskovich
 */

#include "MeshChartItem.h"

MeshChartItem::MeshChartItem(const QString& title, QColor color, QObject* parent) :
    QObject(parent),
    m_title(title),
    m_color(color),
    m_font(QFont("Sans Serif", 8, QFont::Normal)),
    m_value(0),
    m_shadowSize(3),
    m_spacing(7)
{
}

MeshChartItem::MeshChartItem(QObject* parent) :
    QObject(parent),
    m_value(0),
    m_shadowSize(3),
    m_spacing(7)
{
}

QSize MeshChartItem::minimumSize() const
{
    QFontMetrics metrics(valueFont());
    QSize size = metrics.boundingRect(text()).size();
    int height = size.height() + 50;
    int width = size.width();
    int childrenWidth = 0;
    foreach(MeshChartItem* item, m_children)
    {
        childrenWidth += item->minimumWidth();
    }
    width = std::max(width, childrenWidth);
    return QSize(width + m_spacing + m_shadowSize, height + m_spacing + m_shadowSize);
}

QFont MeshChartItem::valueFont() const
{
    QFont font(titleFont());
    font.setPointSizeF(font.pointSizeF() - 1.0);
    return font;
}

QString MeshChartItem::text() const
{
    double val = value();
    if (val < 10240.0) return QString::number(val);
    val /= 1024.0; if (val < 10240.0) return QString("%1K").arg(val,0,'f',0);
    val /= 1024.0; if (val < 10240.0) return QString("%1M").arg(val,0,'f',0);
    val /= 1024.0; return QString("%1G").arg(val,0,'f',0);
}

void MeshChartItem::paint(QPainter* painter, const QRect& rect, double referenceVal)
{
    if (!title().isEmpty())
    {
        QRect itemRect(rect);
        itemRect.adjust(0, 0, -m_shadowSize, -m_shadowSize);
        double normalizedVal = referenceVal > 0 ? (value() / referenceVal) : 0.0;

        QFont font = valueFont();
        int fontHeight = QFontMetrics(font).height();
        itemRect.setTop(itemRect.bottom() - (int)((double)(itemRect.height() - fontHeight - 4) * normalizedVal + 0.5));
        itemRect.moveTop(itemRect.top() - m_shadowSize);
        QRect textRect(rect.left(), itemRect.top() - fontHeight, rect.width(), fontHeight);

        QRect rightShadow(itemRect.right() + 1, itemRect.top() + m_shadowSize, m_shadowSize, itemRect.height());
        QRect bottomShadow(itemRect.left() + m_shadowSize, itemRect.bottom() + 1, itemRect.width(), m_shadowSize);

        itemRect.adjust(0, 0, -1, -1);

        painter->setBrush(QBrush(color()));
        painter->setPen(Qt::black);

        painter->drawRect(itemRect);
        painter->fillRect(rightShadow, Qt::darkGray);
        painter->fillRect(bottomShadow, Qt::darkGray);

        painter->setFont(font);
        painter->drawText(textRect, Qt::AlignHCenter | Qt::AlignTop, text());
    }

    int count = itemCount();
    int index = 0;
    if (!count) return;

    QRect itemsRect(rect);

    if (!m_title.isEmpty())
    {
        itemsRect.setLeft(rect.left() + rect.width() / (items().count() + 1));
    }

    int itemWidth = (int)round(((double)itemsRect.width() - (double)((count - 1) * m_spacing)) / (double)count);

    foreach (MeshChartItem* item, m_children)
    {
        int left = itemsRect.right() - (itemWidth * (count - index)) - (count - index - 1) * m_spacing + 1;
        QRect itemRect(QPoint(left, itemsRect.top()), QSize(itemWidth, itemsRect.height()));
        item->paint(painter, itemRect, referenceVal);
        ++index;
    }
}

ChartItemList MeshChartItem::items() const
{
    ChartItemList items;
    foreach (MeshChartItem* item, m_children)
    {
        items << item;
        items << item->items();
    }
    return items;
}

void MeshChartItem::updateItem()
{
    emit updateRequired();
}

void MeshChartItem::addItem(MeshChartItem* item)
{
    m_children << item;
    connect(item, SIGNAL(updateRequired()), this, SLOT(updateItem()));
    emit updateRequired();
}

void MeshChartItem::removeItem(MeshChartItem* item)
{
    m_children.removeAll(item);
    disconnect(item);
    emit updateRequired();
}
