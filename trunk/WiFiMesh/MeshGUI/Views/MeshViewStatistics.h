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
 * @file MeshViewStatistics.h
 * @date 17/02/2009
 * @author Denis Itskovich
 */

#ifndef MESHVIEWSTATISTICS_H_
#define MESHVIEWSTATISTICS_H_

#include "MeshView.h"
#include "Widgets/MeshWidgetChart.h"
#include "Theme/MeshTheme.h"
#include "../Core/MeshCore.h"

class MeshViewStatistics : public MeshView
{
    Q_OBJECT
public:
    MeshViewStatistics(QWidget* parent = NULL);

    virtual void setDocument(MeshDocument* doc);

protected slots:
    void updateStatistics(const Statistics* pStatistics);

private:
    MeshChartItem* createItem(const MeshTheme::ItemDescriptor& desc);
    void contextMenuEvent(QContextMenuEvent* event);

    void init();

    MeshWidgetChart*    m_chartPacketsByStatus;
    MeshWidgetChart*    m_chartPacketsByType;
    MeshWidgetChart*    m_chartSizeByType;
    MeshWidgetChart*    m_chartPacketsByTraffic;
    MeshWidgetChart*    m_chartSizeByTraffic;
    MeshWidgetChart*    m_chartDelays;
    MeshWidgetChart*    m_chartPathLen;
    MeshWidgetChart*    m_chartThroughput;

    MeshChartItem*      m_itemPacketsByStatus[ePKT_STATUS_PENDING];
    MeshChartItem*      m_itemPacketsByType[ePKT_TYPE_LAST];
    MeshChartItem*      m_itemSizeByType[ePKT_TYPE_LAST];
    MeshChartItem*      m_itemPacketsByTraffic[eTRAFFIC_LAST];
    MeshChartItem*      m_itemSizeByTraffic[eTRAFFIC_LAST];
    MeshChartItem*      m_itemDelayType[eSTAT_LAST];
    MeshChartItem*      m_itemPathLen[eSTAT_LAST];
    MeshChartItem*      m_itemThroughput[eSTAT_LAST];

    QActionGroup*       m_groupByTraffic;
    QActionGroup*       m_groupByType;
    QActionGroup*       m_groupThroughput;

    QMenu*              m_menu;
};

#endif /* MESHVIEWSTATISTICS_H_ */
