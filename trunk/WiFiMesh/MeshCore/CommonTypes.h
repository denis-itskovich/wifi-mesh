/**
 * \file CommonTypes.h
 *
 * Common data type declarations
 *
 * \author Denis Itskovich
 * \date 13/12/2008
 */

#ifndef WIFI_MESH_COMMONTYPES_H_
#define WIFI_MESH_COMMONTYPES_H_

typedef enum { FALSE, TRUE } Boolean;					///< Boolean type
typedef enum { LESS = -1, EQUAL, GREAT } Comparison;	///< Comparison type, used as return type of comparison operations

/// Location and Velocity vector declarations
typedef struct _Vector
{
	double x;					///< \a x component
	double y; 					///< \a y component
} Vector;

typedef Vector Location;			///< Location vector declaration
typedef Vector Velocity;			///< Velocity vector declaration
typedef unsigned long StationId; 	///< Station id

#define INVALID_STATION_ID			((StationId)-2) ///< Non-existing station id
#define BROADCAST_STATION_ID		((StationId)-1) ///< Broadcast station id
#define IS_BROADCAST(id)			((id) == BROADCAST_STATION_ID)

#endif /* WIFI_MESH_COMMONTYPES_H_ */
