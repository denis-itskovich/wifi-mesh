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
    setFont(QFont("Sans Serif", 11, QFont::Bold));
    m_action = new QAction(m_title, this);
    m_action->setCheckable(true);
    m_action->setChecked(true);
    m_spacing = 7;

    connect(m_action, SIGNAL(toggled(bool)), this, SLOT(setVisible(bool)));
    connect(&m_topItem, SIGNAL(updateRequired()), this, SLOT(updateItems()));
}

QAction* MeshWidgetChart::toggleViewAction()
{
    return m_action;
}

void MeshWidgetChart::setVisible(bool visible)
{
    QWidget::setVisible(visible);
    m_action->setChecked(visible);
}

const QString& MeshWidgetChart::title() const
{
    return m_title;
}

void MeshWidgetChart::addItem(MeshChartItem* item)
{
    m_topItem.addItem(item);
}

void MeshWidgetChart::removeItem(MeshChartItem* item)
{
    m_topItem.removeItem(item);
}

void MeshWidgetChart::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QPainter painter;

    painter.begin(this);

    setFont(font());
    painter.setRenderHints(QPainter::TextAntialiasing);
    painter.drawText(m_titleRect, Qt::AlignCenter, m_title);

    ChartItemList items = m_topItem.items();
    double maxVal = 0;
    foreach (MeshChartItem* item, items) { maxVal = std::max(maxVal, item->value()); }

    m_topItem.paint(&painter, m_itemsRect, maxVal);
    paintLegend(&painter);

    painter.end();
}

void MeshWidgetChart::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    updateItems();
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
    return QRect(QPoint(0, 0), m_topItem.minimumSize());
}

QRect MeshWidgetChart::calculateLegendRect() const
{
    QFontMetrics metrics(legendFont());

    int width = metrics.width("Legend");
    int height = metrics.height() + 10 + m_spacing;
    ChartItemList items = m_topItem.items();

    foreach(MeshChartItem* item, items)
    {
        const QFont& font = item->titleFont();
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
    return metrics.boundingRect(title());
}

void MeshWidgetChart::updateItems()
{
    updateBounds();
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
    update();
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

    ChartItemList items = m_topItem.items();
    foreach(MeshChartItem* item, items)
    {

        painter->setFont(item->titleFont());
        int fontHeight = painter->fontMetrics().height();
        painter->fillRect(point.x(), point.y(), fontHeight, fontHeight, item->color());
        QRect textbox(QPoint(point.x() + fontHeight + 4, point.y()), QSize(m_legendRect.width() - fontHeight - 4, fontHeight));
        painter->drawText(textbox, item->title());
        point.setY(point.y() + fontHeight + 4);
    }
}

void MeshWidgetChart::setTitle(const QString& title)
{
    m_title = title;
}
