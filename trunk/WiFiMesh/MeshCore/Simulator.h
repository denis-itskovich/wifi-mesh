/*
 * Simulator.h
 *
 * Represents Simulator
 *
 * \author Denis Itskovich
 * \date   06/12/2008
 */

#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include "Status.h"
#include "Protocol.h"

typedef struct _Simulator Simulator;

EStatus SimulatorCreate(Simulator** ppThis);
EStatus SimulatorDispose(Simulator** ppThis);
EStatus SimulatorInit(Simulator** pThis);
EStatus SimulatorDestroy(Simulator* pThis);
EStatus SimulatorAddStation(Simulator* pThis, StationId id);
EStatus SimulatorProcess(Simulator* pThis);

#endif /* SIMULATOR_H_ */
