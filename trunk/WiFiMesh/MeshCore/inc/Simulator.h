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
 * \file Simulator.h
 *
 * Represents WiFi Mesh Simulator
 *
 * \author Denis Itskovich
 * \date   06/12/2008
 */

#ifndef WIFI_MESH_SIMULATOR_H_
#define WIFI_MESH_SIMULATOR_H_

#include "Status.h"
#include "Station.h"
#include "Settings.h"

typedef struct _Simulator Simulator; ///< Simulator forward declaration

/** Event type, handled by station tracker
 * \see StationTracker
 */
typedef enum
{
	eSTATION_ADDED,			///< Station was added to simulator
	eSTATION_REMOVED,		///< Station was removed from simulator
	eSTATION_UPDATED		///< Station was updated
} EStationEvent;

/** Packet status, handled by sniffer
 * \see Sniffer
 */
typedef enum
{
    ePKT_STATUS_PENDING,        ///< Packet transmitting has been recently started
    ePKT_STATUS_DELIVERED,      ///< Transmit was successfully finished
    ePKT_STATUS_COLLISION,      ///< Transmit was aborted due to collision
    ePKT_STATUS_OUT_OF_RANGE,   ///< Transmit was not started because destination was out of range

    ePKT_STATUS_LAST            ///< Terminator
} EPacketStatus;

/** Station tracker
 * \param pStation [in] pointer to station
 * \param eventType [in] station event type
 * \param pUserArg [in] user defined argument
 */
typedef void (*StationTracker)(Station* pStation, EStationEvent eventType, void* pUserArg);

/** Packet transfer callback
 * \param pPacket [in] pointer to packet
 * \param pSrc [in] pointer to source station
 * \param pDest [in] pointer to destination station
 * \param status [in] current packet status
 * \param pUserArg [in] user defined argument
 */
typedef void (*PacketSniffer)(  const Packet* pPacket,
                                const Station* pSrc,
                                const Station* pDest,
                                EPacketStatus status,
                                void* pUserArg);

/** Stations enumerator
 * \param pStation [in] pointer to station
 * \param pUserArg [in] user defined argument
 */
typedef void (*StationsEnumerator)(Station* pStation, void* pUserArg);

/** Allocates and initializes new instance of Simulator
 * \param ppThis [out] pointer to new instance will be stored at *ppThis
 * \param pSettings [in] pointer to settings instance
 * \param pTimeLine [in] pointer to time line instance
 */
EStatus SimulatorNew(Simulator** ppThis, Settings* pSettings, TimeLine* pTimeLine);

/** Destroys and deallocates an instance of Simulator
 * \param ppThis [in, out] *ppThis must point to valid instance
 */
EStatus SimulatorDelete(Simulator** ppThis);

/** Initializes an instance
 * \param pThis [in] pointer to instance
 * \param pSettings [in] pointer to settings instance
 * \param pTimeLine [in] pointer to time line instance
 */
EStatus SimulatorInit(Simulator* pThis, Settings* pSettings, TimeLine* pTimeLine);

/** Destroys an instance
 * \param pThis [in] pointer to valid instance
 */
EStatus SimulatorDestroy(Simulator* pThis);

/** Adds a station to simulator
 * \param pThis [in] pointer to instance
 * \param pStation [in] pointer to station
 */
EStatus SimulatorAddStation(Simulator* pThis, Station* pStation);

/** Removes a station from simulator
 * \param pThis [in] pointer to instance
 * \param pStation [in] pointer to station to be removed
 */
EStatus SimulatorRemoveStation(Simulator* pThis, Station* pStation);

/** Performs single simulation iteration
 * \param pThis [in] pointer to instance
 */
EStatus SimulatorProcess(Simulator* pThis);

/** Sets sniffer callback
 * \param pThis [in] pointer to instance
 * \param sniffer [in] sniffer callback
 * \param pUserArg [in] user defined argument of the sniffer
 */
EStatus SimulatorSetPacketSniffer(Simulator* pThis, PacketSniffer sniffer, void* pUserArg);

/** Sets station tracker callback
 * \param pThis [in] pointer to instance
 * \param tracker [in] tracker callback
 * \param pUserArg [in] user defined argument of the tracker
 */
EStatus SimulatorSetStationTracker(Simulator* pThis, StationTracker tracker, void* pUserArg);

/** Resets simulator
 * \param pThis [in] pointer to instance
 */
EStatus SimulatorReset(Simulator* pThis);

/** Clears simulator
 * \param pThis [in] pointer to instance
 */
EStatus SimulatorClear(Simulator* pThis);

/** Enumerates all stations
 * \param pThis [in] pointer to instance
 * \param enumerator [in] enumerator callback
 * \param pUserArg [in] user defined argument
 */
EStatus SimulatorEnumerateStations(Simulator* pThis, StationsEnumerator enumerator, void* pUserArg);

/** Sets broadcast sniffing mode
 * \param pThis [in] pointer to instance
 * \param bSingle [in] determines whether sniffer will receive broadcast packets once,
 * or multiple times (for each recipient)
 */
EStatus SimulatorSetSniffingMode(Simulator* pThis, Boolean bSingle);

/** Retrieves broadcast sniffing mode
 * \param pThis [in] pointer to instance
 * \param pSingle [out] current mode will be stored at *pSingle
 */
EStatus SimulatorGetSniffingMode(Simulator* pThis, Boolean* pSingle);

#endif /* WIFI_MESH_SIMULATOR_H_ */
