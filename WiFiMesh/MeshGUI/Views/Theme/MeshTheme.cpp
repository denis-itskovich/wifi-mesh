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

const MeshTheme::ItemDescriptor MeshTheme::s_packetType[ePKT_TYPE_LAST] =
{
     { "Search request",    0x000000bf },
     { "Search response",   0x0000bf00 },
     { "Data",              0x00000000 },
     { "Ack",               0x007f7f7f }
};

const MeshTheme::ItemDescriptor MeshTheme::s_packetStatus[ePKT_TYPE_LAST] =
{
     { "Delivered",     0x00007f00 },
     { "Collision",     0x00bf0000 },
     { "Out of range",  0x00bf7f00 },
     { "Pending",       0x007f7f7f },
};

const MeshTheme::ItemDescriptor& MeshTheme::packetTypeDescriptor(EPacketType packetType)
{
    return s_packetType[packetType];
}

const MeshTheme::ItemDescriptor& MeshTheme::packetStatusDescriptor(EPacketStatus packetStatus)
{
    return s_packetStatus[packetStatus];
}
