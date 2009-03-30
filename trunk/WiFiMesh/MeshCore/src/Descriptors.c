/*********************************************************************************
MeshCore
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
 * @file Descriptions.c
 * @date 21/02/2009
 * @author Denis Itskovich
 */

#include "../inc/Descriptors.h"

const char* DESC_PACKET_TYPE[ePKT_TYPE_LAST] =
{
     "Search request",
     "Search response",
     "Data",
     "Ack"
};

const char* DESC_PACKET_STATUS[ePKT_STATUS_LAST] =
{
     "Delivered",
     "Collision",
     "Out of range",
     "Hops limit reached",
     "Pending"
};

const char* DESC_TRAFFIC_TYPE[eTRAFFIC_LAST] =
{
     "Scheduled",
     "Issued",
     "Delivered"
};

const char* DESC_STAT_TYPE[eSTAT_LAST] =
{
     "Minimum",
     "Average",
     "Maximum"
};
