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
 * @file MeshTheme.h
 * @date 17/02/2009
 * @author Denis Itskovich
 */


#ifndef MESHTHEME_H_
#define MESHTHEME_H_

#include "../../Core/MeshCore.h"
#include <QtGui/QtGui>

class MeshTheme
{
public:

    struct ItemDescriptor
    {
        const char* title;
        QRgb        color;
    };

    static const ItemDescriptor& packetTypeDescriptor(EPacketType packetType);
    static const ItemDescriptor& packetStatusDescriptor(EPacketStatus packetStatus);
    static const ItemDescriptor& trafficTypeDescriptor(ETraffic trafficType);
    static const ItemDescriptor& statisticTypeDescriptor(EStatisticsType type);

private:
    static const ItemDescriptor s_packetType[ePKT_TYPE_LAST];
    static const ItemDescriptor s_packetStatus[ePKT_STATUS_LAST];
    static const ItemDescriptor s_trafficType[eTRAFFIC_LAST];
    static const ItemDescriptor s_statisticType[eSTAT_LAST];
};

#endif /* MESHTHEME_H_ */
