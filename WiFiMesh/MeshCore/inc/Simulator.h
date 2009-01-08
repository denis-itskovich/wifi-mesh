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

#include <Status.h>
#include <Station.h>
#include <Settings.h>

typedef struct _Simulator Simulator; ///< Simulator forward declaration

/** Allocates and initializes new instance of Simulator
 * \param ppThis [out] pointer to new instance will be stored at *ppThis
 * \param pSettings [in] pointer to settings instance
 */
EStatus SimulatorNew(Simulator** ppThis, Settings* pSettings);

/** Destroys and deallocates an instance of Simulator
 * \param ppThis [in, out] *ppThis must point to valid instance
 */
EStatus SimulatorDelete(Simulator** ppThis);

/** Initializes an instance
 * \param pThis [in] pointer to instance
 * \param pSettings [in] pointer to settings instance
 */
EStatus SimulatorInit(Simulator* pThis, Settings* pSettings);

/** Destroys an instance
 * \param pThis [in] pointer to valid instance
 */
EStatus SimulatorDestroy(Simulator* pThis);

/** Adds a station to simulator
 * \param pThis [in] pointer to instance
 * \param id [in] station id
 * \param location [in] station location
 * \param velocity [in] station velocity vector
 * \param ppStation [out] pointer to new station will be stored at *ppStation
 */
EStatus SimulatorAddStation(Simulator* pThis, Station** ppStation, StationId id, Location location, Velocity velocity);

/** Removes a station from simulator
 * \param pThis [in] pointer to instance
 * \param pStation [in] pointer to station to be removed
 */
EStatus SimulatorRemoveStation(Simulator* pThis, Station* pStation);

/** Performs single simulation iteration
 * \param pThis [in] pointer to instance
 */
EStatus SimulatorProcess(Simulator* pThis);

#endif /* WIFI_MESH_SIMULATOR_H_ */
