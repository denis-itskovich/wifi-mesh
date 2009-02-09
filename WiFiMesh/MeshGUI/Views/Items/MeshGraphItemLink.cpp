/**
 * \file MeshGraphItemLink.cpp
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 08/02/2009
 * \author Denis Itskovich
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
    MeshGraphItemArrow(PKT_TYPES_COLOR[pPacket->type], 5, 1, 10.0, 10.0),
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
