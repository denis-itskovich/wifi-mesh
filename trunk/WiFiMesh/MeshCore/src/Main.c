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

#include <stdlib.h>
#include <string.h>

const char* EXE_NAME = NULL;

void Welcome()
{
    printf("Welcome to %s, version %s\n\n", APP_NAME, APP_VERSION);
}

void Usage()
{
    printf("Usage: %s <input_file> [<path_loss_file>]\n\n", EXE_NAME);
    printf("Where  <input_file> is simulation file\n");
    printf("       <path_loss_file> is path loss file, as described in user guide\n");
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

int Simulate(const char* inputfile, const char* pathloss)
{
    Simulator* pSimulator;
    TimeLine* pTimeLine;
    Settings* pSettings;
    double maxDuration, time;

    Check(SettingsNew(&pSettings));
    Check(TimeLineNew(&pTimeLine));
    Check(SimulatorNew(&pSimulator, pSettings, pTimeLine));

    Check(SimulatorImport(pSimulator, inputfile));
    Check(SettingsGetMaxDuration(pSettings, &maxDuration));
    Check(SimulatorReset(pSimulator));

    printf("Running simulation, please wait...");
    fflush(stdout);

    while (SimulatorProcess(pSimulator) == eSTATUS_COMMON_OK)
    {
        Check(TimeLineGetTime(pTimeLine, &time));
        if (time > maxDuration) break;
    }

    printf("\nSimulation finished, time: %lf\n", time);
    PrintStatistics(pSimulator);

    Check(SimulatorDelete(&pSimulator));
    Check(TimeLineDelete(&pTimeLine));
    Check(SettingsDelete(&pSettings));

    return 0;
}

int main(int argc, char** argv)
{
    const char* inputfile = NULL;
    const char* pathloss = NULL;

    EXE_NAME = argv[0];
    Welcome();
    if ((argc < 2) || (argc > 3) || strcmp(argv[1], "/?") == 0) Usage();

    inputfile = argv[1];
    if (argc > 2) pathloss = argv[2];

    return Simulate(inputfile, pathloss);
}
