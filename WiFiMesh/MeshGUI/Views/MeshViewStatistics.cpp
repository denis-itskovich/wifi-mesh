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

    m_menu = new QMenu(this);
    m_menu->addAction(m_chartPacketsByStatus->toggleViewAction());
    m_menu->addAction(m_chartPacketsByType->toggleViewAction());
    m_menu->addAction(m_chartSizeByType->toggleViewAction());
    m_menu->addAction(m_chartPacketsByTraffic->toggleViewAction());
    m_menu->addAction(m_chartSizeByTraffic->toggleViewAction());

//    QActionGroup* typeGroup = new QActionGroup(this);
//    m_chartPacketsByType->toggleViewAction()->setActionGroup(typeGroup);
//    m_chartSizeByType->toggleViewAction()->setActionGroup(typeGroup);
//
//    QActionGroup* trafficGroup = new QActionGroup(this);
//    m_chartPacketsByTraffic->toggleViewAction()->setActionGroup(trafficGroup);
//    m_chartSizeByTraffic->toggleViewAction()->setActionGroup(trafficGroup);

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

    QVBoxLayout* vlayout = new QVBoxLayout;
    QHBoxLayout* topLayout = new QHBoxLayout;
    QHBoxLayout* bottomLayout = new QHBoxLayout;

    topLayout->addWidget(m_chartSizeByType);
    topLayout->addWidget(m_chartPacketsByType);
    bottomLayout->addWidget(m_chartSizeByTraffic);
    bottomLayout->addWidget(m_chartPacketsByTraffic);
    bottomLayout->addWidget(m_chartPacketsByStatus);

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

}

void MeshViewStatistics::contextMenuEvent(QContextMenuEvent* event)
{
    m_menu->exec(event->globalPos());
}
