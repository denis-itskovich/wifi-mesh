/**
 * \file Globals.h
 *
 * Project: MeshCore
 * (C) Copyright 2008
 * \date 20/12/2008
 * \author denis
 */

#ifndef WIFI_MESH_GLOBALS_H_
#define WIFI_MESH_GLOBALS_H_

/** Sets coverage range
 * \param coverage [in] desired coverage range, in distance units
 */
void GlobalsSetCoverage(double coverage);

/** Sets data rate
 * \param bpu [in] desired data rate in bits per time unit
 */
void GlobalsSetDataRate(unsigned bpu);

/** Sets routing table item time to live
 * \param ttl [in] time to live, in time units
 */
void GlobalsSetRouteTimeToLive(double ttl);

/** Sets current time
 * \param time [in] current time, in time units
 */
void GlobalsSetTime(double time);

/** Gets current time
 * \return current time, in time units
 */
double GlobalsGetTime();

/** Retrieves routing table item time to live
 * \return expiration time, in time units
 */
double GlobalsRoutingEntryExpirationTime();

#endif /* WIFI_MESH_GLOBALS_H_ */
