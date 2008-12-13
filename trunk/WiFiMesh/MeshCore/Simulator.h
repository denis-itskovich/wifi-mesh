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
#include "Protocol.h"

typedef struct _Simulator Simulator; ///< Simulator forward declaration

/** Allocates and initializes new instance of Simulator
 * \param ppThis [out] pointer to new instance will be stored at *ppThis
 */
EStatus SimulatorNew(Simulator** ppThis);

/** Destroys and deallocates an instance of Simulator
 * \param ppThis [in, out] *ppThis must point to valid instance
 */
EStatus SimulatorDelete(Simulator** ppThis);

/** Initializes an instance
 * \param pThis [in] pointer to instance
 */
EStatus SimulatorInit(Simulator* pThis);

/** Destroys an instance
 * \param pThis [in] pointer to valid instance
 */
EStatus SimulatorDestroy(Simulator* pThis);

/** Adds a station to simulator
 * \param pThis [in] pointer to instance
 * \param pStation [in] pointer to station
 */
EStatus SimulatorAddStation(Simulator* pThis, Station* pStation);

/** Performs single simulation iteration
 * \param pThis [in] pointer to instance
 */
EStatus SimulatorProcess(Simulator* pThis);

#endif /* WIFI_MESH_SIMULATOR_H_ */
