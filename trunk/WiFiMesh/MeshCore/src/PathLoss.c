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
 * @file PathLoss.c
 * @date 01/03/2009
 * @author Denis Itskovich
 */

#include "../inc/PathLoss.h"
#include "../inc/Macros.h"
#include "../inc/SortedList.h"
#include <stdio.h>

int __counter_Boolean = 0;
int __counter_BooleanPtr = 0;
int __counter_PathLoss = 0;
int __counter_PathLossEntry = 0;

typedef struct _PathLossEntry
{
    double      time;
    Boolean**   ppAdjacency;
} PathLossEntry;

struct _PathLoss
{
    int             stations;
    SortedList*     pEntries;
    ListEntry*      pNextEntry;
    PathLossEntry*  pCurrent;
    PathLossEntry*  pNext;
};

EStatus PathLossNew(PathLoss** ppThis, int count, double maxAttenuation, const char* filename)
{
    CONSTRUCT(ppThis, PathLoss, count, maxAttenuation, filename);
}

EStatus PathLossDelete(PathLoss** ppThis)
{
    DESTRUCT(ppThis, PathLoss);
}

EStatus PathLossNewEntry(PathLoss* pThis, double time, PathLossEntry** ppEntry)
{
    PathLossEntry* pEntry;
    int i;

    VALIDATE_ARGUMENTS(pThis);

    pEntry = NEW(PathLossEntry);
    pEntry->time = time;
    pEntry->ppAdjacency = NEW_ARRAY(Boolean*, pThis->stations);

    for (i = 0; i < pThis->stations; ++i)
    {
        pEntry->ppAdjacency[i] = NEW_ARRAY(Boolean, pThis->stations);
        CLEAR_ARRAY(pEntry->ppAdjacency[i], pThis->stations);
    }

    *ppEntry = pEntry;
    return SortedListAdd(pThis->pEntries, pEntry, TRUE);
}

EStatus PathLossDeleteEntry(PathLoss* pThis, PathLossEntry* pEntry)
{
    int i;
    VALIDATE_ARGUMENTS(pThis && pEntry);

    for (i = 0; i < pThis->stations; ++i)
    {
        DELETE(Boolean, pEntry->ppAdjacency[i]);
    }

    DELETE(BooleanPtr, pEntry->ppAdjacency);
    DELETE(PathLossEntry, pEntry);
    return eSTATUS_COMMON_OK;
}

Comparison PathLossComparator(const PathLossEntry* pLeft, const PathLossEntry* pRight, PathLoss* pThis)
{
    return (pLeft->time == pRight->time) ? EQUAL : (pLeft->time < pRight->time) ? LESS : GREAT;
}

Comparison PathLossFinder(const PathLossEntry* pEntry, const double* pTime, PathLoss* pThis)
{
    return (pEntry->time == *pTime) ? EQUAL : (pEntry->time < *pTime) ? LESS : GREAT;
}

EStatus PathLossGetEntry(PathLoss* pThis, double time, PathLossEntry** ppEntry)
{
    ListEntry* pListEntry;
    VALIDATE_ARGUMENTS(pThis && ppEntry);
    SortedListFind(pThis->pEntries, &pListEntry, (ItemComparator)&PathLossFinder, &time, pThis);
    if (pListEntry) return SortedListGetValue(pListEntry, ppEntry);
    else return PathLossNewEntry(pThis, time, ppEntry);
}

EStatus PathLossAddStation(PathLoss* pThis, double time, int index, double maxAttenuation, FILE* file)
{
    PathLossEntry* pEntry;
    int i = 0;
    double attenuation;
    VALIDATE_ARGUMENTS(pThis && file);
    CHECK(PathLossGetEntry(pThis, time, &pEntry));
    for (i = 0; i < pThis->stations; ++i)
    {
        VALIDATE(fscanf(file, "%lf", &attenuation) == 1, eSTATUS_SIMULATOR_FILE_CORRUPTED);
        pEntry->ppAdjacency[index][i] = (attenuation <= maxAttenuation) ? TRUE : FALSE;
    }
    return eSTATUS_COMMON_OK;
}

EStatus PathLossInit(PathLoss* pThis, int count, double maxAttenuation, const char* filename)
{
    int index;
    FILE* file;
    double time;
    VALIDATE_ARGUMENTS(pThis && filename);
    CLEAR(pThis);

    if (count < 0) return eSTATUS_COMMON_INVALID_ARGUMENT;

    file = fopen(filename, "r");
    if (!file) return eSTATUS_SIMULATOR_FILE_OPEN_FAILURE;

    CHECK(SortedListNew(&pThis->pEntries, (ItemComparator)&PathLossComparator, pThis));
    pThis->stations = count;

    while (!feof(file))
    {
        if (fscanf(file, "%lf %d", &time, &index) != 2) break;
        CHECK(PathLossAddStation(pThis, time, --index, maxAttenuation, file));
    }

    fclose(file);
    return eSTATUS_COMMON_OK;
}

EStatus PathLossDestroy(PathLoss* pThis)
{
    PathLossEntry* pEntry;
    VALIDATE_ARGUMENTS(pThis);

    if (!pThis->pEntries) return eSTATUS_COMMON_OK;

    do
    {
        SortedListPopFront(pThis->pEntries, &pEntry);
        if (pEntry) CHECK(PathLossDeleteEntry(pThis, pEntry));
    } while (pEntry);

    CHECK(SortedListDelete(&pThis->pEntries));

    return eSTATUS_COMMON_OK;
}

EStatus PathLossSynchronize(PathLoss* pThis, double time)
{
    VALIDATE_ARGUMENTS(pThis);

    if (!pThis->pNext || (time < pThis->pNext->time)) return eSTATUS_COMMON_OK;

    pThis->pCurrent = pThis->pNext;
    CHECK(SortedListGetNext(&pThis->pNextEntry));
    if (pThis->pNextEntry) CHECK(SortedListGetValue(pThis->pNextEntry, &pThis->pNext));
    else pThis->pNext = NULL;

    return eSTATUS_COMMON_OK;
}

EStatus PathLossIsAdjacent(PathLoss* pThis, StationId srcId, StationId dstId, Boolean* pIsAdjacent)
{
    VALIDATE_ARGUMENTS(pThis && pIsAdjacent && (srcId <= pThis->stations) && (dstId <= pThis->stations));
    *pIsAdjacent = FALSE;
    if (!pThis->pCurrent) return eSTATUS_COMMON_OK;

    *pIsAdjacent = pThis->pCurrent->ppAdjacency[--srcId][--dstId];
    return eSTATUS_COMMON_OK;
}

EStatus PathLossReset(PathLoss* pThis)
{
    VALIDATE_ARGUMENTS(pThis);
    CHECK(SortedListGetHead(pThis->pEntries, &pThis->pNextEntry));
    if (pThis->pNextEntry) CHECK(SortedListGetValue(pThis->pNextEntry, &pThis->pNext));
    pThis->pCurrent = NULL;
    return eSTATUS_COMMON_OK;
}
