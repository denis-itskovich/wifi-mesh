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
 * @file MeshTheme.cpp
 * @date 17/02/2009
 * @author Denis Itskovich
 */

#include "MeshTheme.h"
#include "../../../MeshCore/inc/Descriptors.h"

const MeshTheme::ItemDescriptor MeshTheme::s_packetType[ePKT_TYPE_LAST] =
{
     { DESC_PACKET_TYPE[ePKT_TYPE_SEARCH_REQUEST],          0x000000bf },
     { DESC_PACKET_TYPE[ePKT_TYPE_SEARCH_RESPONSE],         0x0000bf00 },
     { DESC_PACKET_TYPE[ePKT_TYPE_DATA],                    0x00000000 },
     { DESC_PACKET_TYPE[ePKT_TYPE_ACK],                     0x007f7f7f }
};

const MeshTheme::ItemDescriptor MeshTheme::s_packetStatus[ePKT_STATUS_LAST] =
{
     { DESC_PACKET_STATUS[ePKT_STATUS_DELIVERED],           0x00007f00 },
     { DESC_PACKET_STATUS[ePKT_STATUS_COLLISION],           0x00df0000 },
     { DESC_PACKET_STATUS[ePKT_STATUS_OUT_OF_RANGE],        0x00df7f00 },
     { DESC_PACKET_STATUS[ePKT_STATUS_HOPS_LIMIT_REACHED],  0x00dfbf00 },
     { DESC_PACKET_STATUS[ePKT_STATUS_PENDING],             0x007f7f7f },
};

const MeshTheme::ItemDescriptor MeshTheme::s_trafficType[eTRAFFIC_LAST] =
{
     { DESC_TRAFFIC_TYPE[eTRAFFIC_SCHEDULED],               0x00bf00bf },
     { DESC_TRAFFIC_TYPE[eTRAFFIC_ISSUED],                  0x00bfbf00 },
     { DESC_TRAFFIC_TYPE[eTRAFFIC_DELIVERED],               0x0000bfbf }
};

const MeshTheme::ItemDescriptor& MeshTheme::packetTypeDescriptor(EPacketType packetType)
{
    assert(packetType < ePKT_TYPE_LAST);
    return s_packetType[packetType];
}

const MeshTheme::ItemDescriptor& MeshTheme::packetStatusDescriptor(EPacketStatus packetStatus)
{
    assert(packetStatus < ePKT_STATUS_LAST);
    return s_packetStatus[packetStatus];
}

const MeshTheme::ItemDescriptor& MeshTheme::trafficTypeDescriptor(ETraffic trafficType)
{
    assert(trafficType < eTRAFFIC_LAST);
    return s_trafficType[trafficType];
}
