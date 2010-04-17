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
 * @file PathLoss.h
 * @date 25/02/2009
 * @author Denis Itskovich
 */

#ifndef PATHLOSS_H_
#define PATHLOSS_H_

#include "CommonTypes.h"
#include "Status.h"

typedef struct _PathLoss PathLoss; ///< forward declaration

/** Allocates and initializes an instance of path loss structure
 * @param ppThis [out] pointer to new instance will be stored at *ppThis
 * @param count [in] stations count
 * @param maxAttenuation [in] maximum attenuation
 * @param filename [in] path loss filename
 */
EStatus PathLossNew(PathLoss** ppThis, int count, double maxAttenuation, const char* filename);

/** Destroys and deallocates an instance
 * @param ppThis [in, out] *ppThis should point to valid instance
 */
EStatus PathLossDelete(PathLoss** ppThis);

/** initializes an instance of path loss
 * @param pThis [in] pointer to instance
 * @param count [in] stations count
 * @param maxAttenuation [in] maximum attenuation
 * @param filename [in] path loss filename
 */
EStatus PathLossInit(PathLoss* pThis, int count, double maxAttenuation, const char* filename);

/** Destroys an instance
 * @param pThis [in] pointer to instance
 */
EStatus PathLossDestroy(PathLoss* pThis);

/** Synchronizes path loss with a timer
 * @param pThis [in] pointer to instance
 * @param time [in] current time
 */
EStatus PathLossSynchronize(PathLoss* pThis, double time);

/** Checks whether stations are adjacent
 * @param pThis [in] pointer to instance
 * @param srcId [in] source station id
 * @param dstId [in] destination station id
 * @param pIsAdjacent [out] result will be stored at *pIsAdjacent
 */
EStatus PathLossIsAdjacent(PathLoss* pThis, StationId srcId, StationId dstId, Boolean* pIsAdjacent);

/** Resets path loss
 * @param pThis [in] pointer to instance
 */
EStatus PathLossReset(PathLoss* pThis);

#endif /* PATHLOSS_H_ */
