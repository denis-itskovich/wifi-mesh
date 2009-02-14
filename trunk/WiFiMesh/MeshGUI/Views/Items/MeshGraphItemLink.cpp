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
 * @file MeshGraphItemLink.cpp
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * @date 08/02/2009
 * @author Denis Itskovich
 */

#include "MeshGraphItemLink.h"

static const QRgb PKT_TYPES_COLOR[ePKT_TYPE_LAST] =
{
        0x000000bf,
        0x0000bf00,
        0x00000000,
        0x007f7f7f
};

MeshGraphItemLink::MeshGraphItemLink(MeshGraphItemStation* src, MeshGraphItemStation* dst, const Packet* pPacket) :
    MeshGraphItemArrow(PKT_TYPES_COLOR[pPacket->header.type], 5, 1, 10.0, 10.0),
    m_src(src),
    m_dst(dst)
{
    setAcceptedMouseButtons(0);
    setZValue(-99);
    updateLink();
}

void MeshGraphItemLink::updateLink()
{
    setEdges(m_src->location(), m_dst->location());
}
