/*
 * Simulator.h
 *
 *  Created on: 06/12/2008
 *      Author: denis
 */

#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include "Status.h"
#include "Protocol.h"

typedef struct _Simulator Simulator;

EStatus SimulatorCreate(Simulator** ppThis);
EStatus SimulatorDestroy(Simulator** ppThis);
EStatus SimulatorAddStation(Simulator* pThis, StationId id);
EStatus SimulatorProcess(Simulator* pThis);

#endif /* SIMULATOR_H_ */
