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
 * @file CommonTypes.h
 *
 * Common data type declarations
 *
 * @author Denis Itskovich
 * @date 13/12/2008
 */

#ifndef WIFI_MESH_COMMONTYPES_H_
#define WIFI_MESH_COMMONTYPES_H_

#undef FALSE
#undef TRUE

typedef enum { FALSE = 0, TRUE } Boolean;					///< Boolean type
typedef enum { LESS = -1, EQUAL, GREAT } Comparison;		///< Comparison type, used as return type of comparison operations

/// Location and Velocity vector declarations
typedef struct _Vector
{
	double x;					///< \a x component
	double y; 					///< \a y component
} Vector;


typedef Vector Location;			///< Location vector declaration
typedef Vector Velocity;			///< Velocity vector declaration
typedef Vector Size;				///< Size declaration
typedef unsigned long StationId; 	///< Station id

/// Rect declaration - defines top-left point and size
typedef struct _Rect
{
	Location	topLeft;	///< Top left point of the rectangle location
	Size	 	size;		///< Rectangle size
} Rect;

#define INVALID_STATION_ID			((StationId)-2) ///< Non-existing station id
#define BROADCAST_STATION_ID		((StationId)-1) ///< Broadcast station id
#define IS_BROADCAST(id)			((id) == BROADCAST_STATION_ID)

#endif /* WIFI_MESH_COMMONTYPES_H_ */
