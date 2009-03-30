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
 * @file MeshViewStatistics.cpp
 * @date 17/02/2009
 * @author Denis Itskovich
 */

#include "MeshViewStatistics.h"

MeshViewStatistics::MeshViewStatistics(QWidget* parent)
{
    init();
}

void MeshViewStatistics::setDocument(MeshDocument* doc)
{
    disconnect(document());
    connect(doc, SIGNAL(statisticsUpdated(const Statistics*)), this, SLOT(updateStatistics(const Statistics*)));
    MeshView::setDocument(doc);
}

MeshChartItem* MeshViewStatistics::createItem(const MeshTheme::ItemDescriptor& desc)
{
    return new MeshChartItem(desc.title, desc.color);
}

void MeshViewStatistics::init()
{
    m_chartPacketsByStatus = new MeshWidgetChart("Packets by delivery status");
    m_chartPacketsByType = new MeshWidgetChart("Packets by packet type");
    m_chartSizeByType = new MeshWidgetChart("Traffic by packet type");
    m_chartPacketsByTraffic = new MeshWidgetChart("Packets by scheduling status");
    m_chartSizeByTraffic = new MeshWidgetChart("Traffic by scheduling status");
    m_chartDelays = new MeshWidgetChart("Packet delivery delay");
    m_chartPathLen = new MeshWidgetChart("Delivery path length");
    m_chartThroughput = new MeshWidgetChart("Upper layer throughput");

//    m_groupByTraffic = new QActionGroup(this);
//    m_groupByTraffic->addAction(m_chartPacketsByTraffic->toggleViewAction());
//    m_groupByTraffic->addAction(m_chartSizeByTraffic->toggleViewAction());
//
//    m_groupByType = new QActionGroup(this);
//    m_groupByType->addAction(m_chartPacketsByType->toggleViewAction());
//    m_groupByType->addAction(m_chartSizeByType->toggleViewAction());
//
//    m_groupThroughput = new QActionGroup(this);
//    m_groupThroughput->addAction(m_chartThroughput->toggleViewAction());
//    m_groupThroughput->addAction(m_chartDelays->toggleViewAction());
//    m_groupThroughput->addAction(m_chartPathLen->toggleViewAction());

    m_menu = new QMenu(this);
    m_menu->addAction(m_chartPacketsByStatus->toggleViewAction());
    m_menu->addAction(m_chartPacketsByType->toggleViewAction());
    m_menu->addAction(m_chartSizeByType->toggleViewAction());
    m_menu->addAction(m_chartPacketsByTraffic->toggleViewAction());
    m_menu->addAction(m_chartSizeByTraffic->toggleViewAction());
    m_menu->addAction(m_chartThroughput->toggleViewAction());
    m_menu->addAction(m_chartDelays->toggleViewAction());
    m_menu->addAction(m_chartPathLen->toggleViewAction());

    m_chartSizeByType->hide();
    m_chartSizeByTraffic->hide();
    m_chartPathLen->hide();

    for (int i = 0; i < ePKT_TYPE_LAST; ++i)
    {
        m_itemPacketsByType[i] = createItem(MeshTheme::packetTypeDescriptor((EPacketType)i));
        m_itemSizeByType[i] = createItem(MeshTheme::packetTypeDescriptor((EPacketType)i));
        m_chartPacketsByType->addItem(m_itemPacketsByType[i]);
        m_chartSizeByType->addItem(m_itemSizeByType[i]);
    }

    for (int i = 0; i < ePKT_STATUS_PENDING; ++i)
    {
        m_itemPacketsByStatus[i] = createItem(MeshTheme::packetStatusDescriptor((EPacketStatus)i));
        m_chartPacketsByStatus->addItem(m_itemPacketsByStatus[i]);
    }

    for (int i = 0; i < eTRAFFIC_LAST; ++i)
    {
        m_itemPacketsByTraffic[i] = createItem(MeshTheme::trafficTypeDescriptor((ETraffic)i));
        m_itemSizeByTraffic[i] = createItem(MeshTheme::trafficTypeDescriptor((ETraffic)i));
        m_chartPacketsByTraffic->addItem(m_itemPacketsByTraffic[i]);
        m_chartSizeByTraffic->addItem(m_itemSizeByTraffic[i]);
    }

    for (int i = 0; i < eSTAT_LAST; ++i)
    {
        m_itemDelayType[i] = createItem(MeshTheme::statisticTypeDescriptor((EStatisticsType)i));
        m_itemPathLen[i] = createItem(MeshTheme::statisticTypeDescriptor((EStatisticsType)i));
        m_itemThroughput[i] = createItem(MeshTheme::statisticTypeDescriptor((EStatisticsType)i));
        m_chartDelays->addItem(m_itemDelayType[i]);
        m_chartPathLen->addItem(m_itemPathLen[i]);
        m_chartThroughput->addItem(m_itemThroughput[i]);
    }

    QVBoxLayout* vlayout = new QVBoxLayout;
    QHBoxLayout* topLayout = new QHBoxLayout;
    QHBoxLayout* bottomLayout = new QHBoxLayout;

    topLayout->addWidget(m_chartSizeByType);
    topLayout->addWidget(m_chartPacketsByType);
    topLayout->addWidget(m_chartSizeByTraffic);
    topLayout->addWidget(m_chartPacketsByTraffic);
    bottomLayout->addWidget(m_chartPacketsByStatus);
    bottomLayout->addWidget(m_chartThroughput);
    bottomLayout->addWidget(m_chartDelays);
    bottomLayout->addWidget(m_chartPathLen);

    vlayout->addItem(topLayout);
    vlayout->addItem(bottomLayout);
    setLayout(vlayout);
}

void MeshViewStatistics::updateStatistics(const Statistics* pStatistics)
{
    for (int i = 0; i < ePKT_TYPE_LAST; ++i)
    {
        m_itemPacketsByType[i]->setValue(pStatistics->packetsByType[i]);
        m_itemSizeByType[i]->setValue(pStatistics->sizeByType[i]);
    }

    for (int i = 0; i < ePKT_STATUS_PENDING; ++i)
    {
        m_itemPacketsByStatus[i]->setValue(pStatistics->packetsByStatus[i]);
    }

    for (int i = 0; i < eTRAFFIC_LAST; ++i)
    {
        m_itemPacketsByTraffic[i]->setValue(pStatistics->packetsByTraffic[i]);
        m_itemSizeByTraffic[i]->setValue(pStatistics->sizeByTraffic[i]);
    }

    for (int i = 0; i < eSTAT_LAST; ++i)
    {
        m_itemDelayType[i]->setValue(pStatistics->deliveryDelay[i]);
        m_itemPathLen[i]->setValue(pStatistics->routeLength[i]);
        m_itemThroughput[i]->setValue(pStatistics->throughput[i]);
    }
}

void MeshViewStatistics::contextMenuEvent(QContextMenuEvent* event)
{
    m_menu->exec(event->globalPos());
}
