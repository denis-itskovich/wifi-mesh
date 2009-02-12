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
 * \file MeshItemLink.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 08/02/2009
 * \author Denis Itskovich
 */


#ifndef MESHITEMLINK_H_
#define MESHITEMLINK_H_

#include <QtGui>
#include "MeshGraphItemStation.h"
#include "MeshGraphItemArrow.h"

class MeshGraphItemLink : public MeshGraphItemArrow
{
public:
    MeshGraphItemLink(MeshGraphItemStation* src, MeshGraphItemStation* dst, const Packet* pPacket);
    void updateLink();

private:
    MeshGraphItemStation*   m_src;
    MeshGraphItemStation*   m_dst;
};

#endif /* MESHITEMLINK_H_ */
