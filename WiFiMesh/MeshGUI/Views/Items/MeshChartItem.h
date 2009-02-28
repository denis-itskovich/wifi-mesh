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
 * Represents chart item
 * @file MeshChartItem.h
 * @date 15/02/2009
 * @author Denis Itskovich
 */


#ifndef MESHCHARTITEM_H_
#define MESHCHARTITEM_H_

#include <QtGui>

class MeshChartItem;

typedef QList<MeshChartItem*> ChartItemList;

class MeshChartItem : public QObject
{
    Q_OBJECT
public:
    MeshChartItem(const QString& title, QColor color, QObject* parent = NULL);
    MeshChartItem(QObject* parent = NULL);

    double value() const                    { return m_value; }
    QColor color() const                    { return m_color; }
    const QString& title() const            { return m_title; }
    const QFont& titleFont() const          { return m_font; }
    QFont valueFont() const;
    int itemCount() const                   { return m_children.count(); }
    MeshChartItem* itemAt(int index)        { return m_children[index]; }
    QSize minimumSize() const;
    int minimumWidth() const                { return minimumSize().width(); }
    int minimumHeight() const               { return minimumSize().height(); }

    ChartItemList items() const;
    virtual void paint(QPainter* painter, const QRect& rect, double referenceVal);

public slots:
    void setValue(double val)               { m_value = val; emit updateRequired(); }
    void setColor(QColor col)               { m_color = col; emit updateRequired(); }
    void setTitle(const QString& title)     { m_title = title; emit updateRequired(); }
    void setFont(const QFont& font)         { m_font = font; emit updateRequired(); }
    void addItem(MeshChartItem* item);
    void removeItem(MeshChartItem* item);

protected slots:
    void updateItem();

signals:
    void updateRequired();

private:
    QString text() const;

    QList<MeshChartItem*>   m_children;
    QString                 m_title;
    QColor                  m_color;
    QFont                   m_font;
    double                  m_value;
    int                     m_shadowSize;
    int                     m_spacing;
};

#endif /* MESHCHARTITEM_H_ */
