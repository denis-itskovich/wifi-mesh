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
    m_chartPacketsByStatus = new MeshWidgetChart("Packets count by delivery status");
    m_chartPacketsByType = new MeshWidgetChart("Packets count by packet type");
    m_chartTrafficByType = new MeshWidgetChart("Traffic by packet type");

    for (int i = 0; i < ePKT_TYPE_LAST; ++i)
    {
        m_itemPacketsByType[i] = createItem(MeshTheme::packetTypeDescriptor((EPacketType)i));
        m_chartPacketsByType->addItem(m_itemPacketsByType[i]);
        m_itemTrafficByType[i] = createItem(MeshTheme::packetTypeDescriptor((EPacketType)i));
        m_chartTrafficByType->addItem(m_itemTrafficByType[i]);
    }

    for (int i = 0; i < ePKT_STATUS_PENDING; ++i)
    {
        m_itemPacketsByStatus[i] = createItem(MeshTheme::packetStatusDescriptor((EPacketStatus)i));
        m_chartPacketsByStatus->addItem(m_itemPacketsByStatus[i]);
    }

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(m_chartTrafficByType, 0, 0);
    gridLayout->addWidget(m_chartPacketsByType, 0, 1);
    gridLayout->addWidget(m_chartPacketsByStatus, 1, 0);

    setLayout(gridLayout);
}

void MeshViewStatistics::updateStatistics(const Statistics* pStatistics)
{
    for (int i = 0; i < ePKT_TYPE_LAST; ++i)
    {
        m_itemPacketsByType[i]->setValue(pStatistics->packetsByType[i]);
        m_itemTrafficByType[i]->setValue(pStatistics->trafficByType[i]);
    }

    for (int i = 0; i < ePKT_STATUS_PENDING; ++i)
    {
        m_itemPacketsByStatus[i]->setValue(pStatistics->packetsByStatus[i]);
    }
}
