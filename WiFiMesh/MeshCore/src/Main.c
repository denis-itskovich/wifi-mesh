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
 * Command line version of WiFi Mesh Simulator
 * @file Main.c
 * @date 21/02/2009
 * @author Denis Itskovich
 */

#include "../inc/Simulator.h"
#include "../inc/TimeLine.h"
#include "../inc/Settings.h"
#include "../inc/Version.h"
#include "../inc/Descriptors.h"

#include <stdlib.h>
#include <string.h>

const char* executableName = NULL;
TimeLine* pTimeLine = NULL;

extern int __counter_Packet;
extern int __counter_Station;
extern int __counter_PacketEntry;
extern int __counter_ListEntry;
extern int __counter_Routing;
extern int __counter_RoutingEntry;
extern int __counter_Event;
extern int __counter_Scheduler;
extern int __counter_SchedulerEntry;
extern int __counter_PacketTypes[ePKT_TYPE_LAST];

void Welcome()
{
    printf("Welcome to %s, version %s\n\n", APP_NAME, APP_VERSION);
}

void Usage()
{
    printf("Usage: %s -i <input_file> [-p <path_loss_file>] [-l <log_file>]\n\n", executableName);
    printf("Where  <input_file>     is a simulation file        [input, mandatory]\n");
    printf("       <path_loss_file> is a path loss file         [input, optional]\n");
    printf("       <log_file>       is a log file (.csv format) [output, optional]\n");
    exit(0);
}

void Error(const char* msg)
{
    printf("Error occurred: %s\n", msg);
    exit(1);
}

void Check(EStatus status)
{
    if (status != eSTATUS_COMMON_OK) Error(StatusGetMessage(status));
}

void PrintStatistics(Simulator* pSimulator)
{
    char buff[2048];
    int len = sizeof(buff);

    const Statistics* pStatistics;
    Check(SimulatorGetStatistics(pSimulator, &pStatistics));
    Check(StatisticsPrint(pStatistics, buff, len));

    printf("%s", buff);
}

void Sniffer(const Packet* pPacket,
             const Station* pSrc,
             const Station* pDest,
             EPacketStatus status,
             FILE* file)
{
    StationId id;
    double time;
    unsigned size;
    if (!file || status == ePKT_STATUS_PENDING) return;

    Check(StationGetId(pDest, &id));
    Check(TimeLineGetTime(pTimeLine, &time));
    Check(PacketGetSize(pPacket, &size));

    fprintf(file, "%-12.9lf,%-16s,%-3d,%-3d,%-3d,%-3d,%-5d,%-3d,%-10s\n",
            time,
            DESC_PACKET_TYPE[pPacket->header.type],
            (int)pPacket->header.originalSrcId,
            (int)pPacket->header.originalDstId,
            (int)pPacket->header.transitSrcId,
            (int)id,
            (int)size,
            (int)pPacket->header.hopsCount,
            DESC_PACKET_STATUS[status]);
}

int Simulate(const char* inputfile, const char* pathloss, const char* logname)
{
    Simulator* pSimulator;
    Settings* pSettings;
    double maxDuration, time;
    FILE* logFile = NULL;

#ifdef _DEBUG
	int i;
#endif // _DEBUG
	if (logname) logFile = fopen(logname, "w");
    if (logFile) fprintf(logFile, "Time, Message type, From, To, Source, Destination, Size, Hops count, Status\n");

    Check(SettingsNew(&pSettings));
    Check(TimeLineNew(&pTimeLine));
    Check(SimulatorNew(&pSimulator, pSettings, pTimeLine));
    Check(SimulatorSetPacketSniffer(pSimulator, (PacketSniffer)&Sniffer, logFile));
    Check(TimeLineSetSmoothenMode(pTimeLine, FALSE));
    Check(SimulatorImport(pSimulator, inputfile));
    Check(SimulatorSetPathLoss(pSimulator, pathloss));

    Check(SettingsGetMaxDuration(pSettings, &maxDuration));
    Check(SimulatorReset(pSimulator));

    printf("Running simulation, please wait...");
    fflush(stdout);

    while (SimulatorProcess(pSimulator) == eSTATUS_COMMON_OK)
    {
        Check(TimeLineGetTime(pTimeLine, &time));
        if (time > maxDuration) break;
    }

    printf("\nSimulation finished, time: %lf\n\n", time);
    PrintStatistics(pSimulator);

    if (logFile) fclose(logFile);

    Check(SimulatorDelete(&pSimulator));
    Check(TimeLineDelete(&pTimeLine));
    Check(SettingsDelete(&pSettings));

#ifdef _DEBUG
    printf("Packets: %d\n", __counter_Packet);
    for (i = 0; i < ePKT_TYPE_LAST; ++i)
    {
        printf("Packets (%s): %d\n", DESC_PACKET_TYPE[i], __counter_PacketTypes[i]);
    }

    printf("Stations: %d\n", __counter_Station);
    printf("PacketsEntries: %d\n", __counter_PacketEntry);
    printf("ListEntries: %d\n", __counter_ListEntry);
    printf("Routing: %d\n", __counter_Routing);
    printf("RoutingEntries: %d\n", __counter_RoutingEntry);
    printf("Events: %d\n", __counter_Event);
    printf("Schedulers: %d\n", __counter_Scheduler);
    printf("SchedulerEntries: %d\n", __counter_SchedulerEntry);
#endif // _DEBUG

    return 0;
}

void ParseArgs(int argc, char** argv, const char** pInputFile, const char** pPathlossFile, const char** pLogFile)
{
    executableName = *argv++;
    --argc;
    *pInputFile = *pPathlossFile = *pLogFile = NULL;

    if (argc < 2) Usage();

    while (argc > 1)
    {
        if (strcmp(*argv, "-i") == 0) *pInputFile = argv[1];
        else if (strcmp(*argv, "-p") == 0) *pPathlossFile = argv[1];
        else if (strcmp(*argv, "-l") == 0) *pLogFile = argv[1];
        else Usage();
        argc -= 2;
        argv += 2;
    }
    if (!*pInputFile)
    {
        printf("Error: Input file not specified!\n\n");
        Usage();
    }
}

int main(int argc, char** argv)
{
    const char* inputFile = NULL;
    const char* pathlossFile = NULL;
    const char* logFile = NULL;

    Welcome();
    ParseArgs(argc, argv, &inputFile, &pathlossFile, &logFile);
    return Simulate(inputFile, pathlossFile, logFile);
}
