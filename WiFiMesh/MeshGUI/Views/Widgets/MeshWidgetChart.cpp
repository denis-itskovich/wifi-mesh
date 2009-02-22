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
    m_shadowSize = 3;
    setFont(QFont("Sans Serif", 11, QFont::Bold));
    m_action = new QAction(m_title, this);
    m_action->setCheckable(true);
    m_action->setChecked(true);

    connect(m_action, SIGNAL(toggled(bool)), this, SLOT(setVisible(bool)));
}

QAction* MeshWidgetChart::toggleViewAction()
{
    return m_action;
}

void MeshWidgetChart::addItem(MeshChartItem* item)
{
    if (item)
    {
        item->setParent(this);
        m_items << item;
        connect(item, SIGNAL(updateRequired()), this, SLOT(update()));
        updateBounds();
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

    double maxVal = 0;
    foreach(MeshChartItem* item, m_items) maxVal = std::max(maxVal, item->value());

    int index = 0;
    foreach(MeshChartItem* item, m_items)
    {
        paintItem(&painter, item, itemRect(index++),maxVal != 0 ? item->value()/maxVal : 0);
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

void MeshWidgetChart::updateBounds()
{
    QRect itemsRect(calculateItemsRect());
    QRect legendRect(calculateLegendRect());
    QRect titleRect(calculateTitleRect());

    int height = std::max(itemsRect.height(), legendRect.height()) + 4 * m_spacing + titleRect.height();
    int width = std::max(itemsRect.width() + legendRect.width() + 2 * m_spacing, titleRect.width()) + 2 * m_spacing;

    setMinimumSize(QSize(width, height));
}

QRect MeshWidgetChart::calculateItemsRect() const
{
    int width = 0;
    width += (m_items.count() + 1) * m_spacing;
    width += m_items.count() * 20;
    return QRect(QPoint(0, 0), QSize(width, 50));
}

QRect MeshWidgetChart::calculateLegendRect() const
{
    QFontMetrics metrics(legendFont());

    int width = metrics.width("Legend");
    int height = metrics.height() + 10 + m_spacing;

    foreach(MeshChartItem* item, m_items)
    {
        const QFont& font = item->font();
        QFontMetrics metrics(font);
        QSize size(metrics.size(0, item->title()));
        size.setWidth(size.width() + size.height());
        if (size.width() > width) width = size.width();
        height += size.height() + 4;
    }

    width += 4;
    return QRect(QPoint(0, 0), QSize(height, width));
}

QRect MeshWidgetChart::calculateTitleRect() const
{
    QFontMetrics metrics(font());
    return metrics.boundingRect(m_title);
}

void MeshWidgetChart::updateItems()
{
    QRect widgetRect(rect());
    QRect legendRect(calculateLegendRect());

    int height = QFontMetrics(font()).height();
    m_titleRect = widgetRect;
    m_titleRect.setBottom(m_titleRect.top() + height + m_spacing * 2);

    widgetRect.setTop(m_titleRect.bottom());

    QRect rect(widgetRect);
    rect.setLeft(rect.right() - legendRect.width());
    rect.adjust(-m_spacing * 2, 0, 0, m_spacing * 2);

    m_legendRect = rect;
    m_itemsRect = QRect(widgetRect.topLeft(), QSize(widgetRect.width() - rect.width(), widgetRect.height()))
                  .adjusted(m_spacing, m_spacing, -m_spacing, -m_spacing);
}

QRect MeshWidgetChart::itemRect(int index)
{
    int count = m_items.count();
    int itemWidth = (int)(((double)m_itemsRect.width() - (double)((count - 1) * m_spacing - m_shadowSize)) / (double)count);

    return QRect(QPoint(m_itemsRect.left() + (itemWidth + m_spacing)*index, m_itemsRect.top()), QSize(itemWidth, m_itemsRect.height()));
}

QString MeshWidgetChart::itemText(MeshChartItem* item) const
{
    double val = item->value();
    if (val < (1 << 17)) return QString::number(val);
    val /= 1024.0; if (val < (1 << 17)) return QString("%1K").arg(val);
    val /= 1024.0; if (val < (1 << 17)) return QString("%1M").arg(val);
    val /= 1024.0; return QString("%1G").arg(val);
}

void MeshWidgetChart::paintItem(QPainter* painter, MeshChartItem* item, const QRect& boundingRect, double normalizedVal)
{
    QRect itemRect(boundingRect);

    int fontHeight = QFontMetrics(item->font()).height();
    itemRect.setTop(itemRect.bottom() - (int)((double)(itemRect.height() - fontHeight - 4) * normalizedVal + 0.5));
    itemRect.moveTop(itemRect.top() - m_shadowSize);
    QRect textRect(boundingRect.left(), itemRect.top() - fontHeight, boundingRect.width(), fontHeight);

    QRect rightShadow(itemRect.right() + 1, itemRect.top() + m_shadowSize, m_shadowSize, itemRect.height());
    QRect bottomShadow(itemRect.left() + m_shadowSize, itemRect.bottom() + 1, itemRect.width(), m_shadowSize);

    itemRect.adjust(0, 0, -1, -1);

    painter->setBrush(QBrush(item->color()));
    painter->setPen(Qt::black);

    painter->drawRect(itemRect);
    painter->fillRect(rightShadow, Qt::darkGray);
    painter->fillRect(bottomShadow, Qt::darkGray);

    QFont font = item->font();
    font.setPointSize(font.pointSize() - 1);
    painter->setFont(font);
    painter->drawText(textRect, Qt::AlignHCenter | Qt::AlignTop, itemText(item));
}

QFont MeshWidgetChart::legendFont() const
{
    QFont fnt(font());
    fnt.setPointSizeF(fnt.pointSizeF() / 1.25);
    return fnt;
}

void MeshWidgetChart::paintLegend(QPainter* painter)
{
    painter->setFont(legendFont());
    painter->drawText(m_legendRect, Qt::AlignHCenter | Qt::AlignTop, tr("Legend"));

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
