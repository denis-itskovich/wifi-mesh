#include "Grid.h"
#include "List.h"
#include "Macros.h"

#include <malloc.h>
#include <string.h>

struct _GridItem
{
	ListHeader			list;
	Position			position;
	Velocity			velocity;
	Station				station;
};

struct _Grid
{
	Size			size;
	Size			gridStep;
	unsigned long	items;
	ListHeader*		pArray;
};

#define VALIDATE_THIS						VALIDATE(pThis, eSTATUS_GRID_INVALID_PTR)
#define VALIDATE_GRID(pThis)				SAFE_OPERATION({VALIDATE(pThis, eSTATUS_GRID_INVALID_PTR); VALIDATE(pThis->pArray, eSTATUS_GRID_INVALID_ARRAY);})

#define ARRAY_INDEX(size, position)			((size.x) * ROUND(position.y) + ROUND(position.x))

#define PREV_ITEM(item)						(GridItem*)((item)->list.pPrev)
#define NEXT_ITEM(item)						(GridItem*)((item)->list.pNext)
#define FIRST_ITEM(list)					(GridItem*)(list.pNext)

#define IF_OUT_OF_RANGE(size, position)		if (((int)(size.x) <= ROUND(position.x)) || ((int)(size.y) <= ROUND(position.y)) || (ROUND(position.x) < 0) || (ROUND(position.y) < 0))
#define VALIDATE_BOUNDS(size, position)		SAFE_OPERATION(IF_OUT_OF_RANGE(size, position) return eSTATUS_GRID_OUT_OF_RANGE)

static EStatus GridRemoveItem(Grid* pThis, GridItem* pItem);

EStatus	GridCreate(Grid** ppThis, Size size)
{
	Grid* pThis;
	VALIDATE(size.x * size.y != 0, eSTATUS_GRID_OUT_OF_RANGE);

	*ppThis = pThis = NEW(Grid);
	VALIDATE(pThis, eSTATUS_COMMON_NO_MEMORY);

	CLEAR(pThis);

	pThis->size = size;
	pThis->items = 0;

	pThis->pArray = NEW_ARRAY(ListHeader, size);
	VALIDATE(pThis->pArray, eSTATUS_COMMON_NO_MEMORY);

	CLEAR_ARRAY(pThis->pArray, size);

	return eSTATUS_COMMON_OK;
}

EStatus GridDestroy(Grid** ppThis)
{
	Grid* pThis;
	GridItem *pItem;
	VALIDATE(ppThis, eSTATUS_COMMON_INVALID_ARGUMENT);

	pThis = *ppThis;
	VALIDATE_GRID(pThis);

	pItem = NULL;
	while (GridFirstItem(pThis, &pItem) == eSTATUS_COMMON_OK)
	{
		GridRemoveItem(pThis, pItem);
		DELETE(pItem);
	}

	free(pThis->pArray);
	free(pThis);
	*ppThis = NULL;

	return eSTATUS_COMMON_OK;
}

EStatus GridAddStation(Grid* pThis, Position position, Velocity velocity, Station station)
{
	GridItem* pItem;
	VALIDATE_GRID(pThis);

	pItem = NEW(GridItem);
	CLEAR(pItem);

	pItem->position = position;
	pItem->velocity = velocity;
	pItem->station = station;

	++pThis->items;

	return GridAddItem(pThis, pItem);
}

EStatus GridEraseItem(Grid* pThis, GridItem* pItem)
{
    CHECK_STATUS_RETURN(GridRemoveItem(pThis, pItem));
    DELETE(pItem);
    return eSTATUS_COMMON_OK;
}

EStatus GridRemoveItem(Grid* pThis, GridItem* pItem)
{
	VALIDATE_GRID(pThis);
	VALIDATE(pItem != NULL, eSTATUS_GRID_INVALID_PTR);

	CHECK_STATUS_RETURN(ListRemove(&pItem->list));
    return eSTATUS_COMMON_OK;
}

EStatus GridAddItem(Grid* pThis, GridItem* pItem)
{
	unsigned i;
	VALIDATE_GRID(pThis);
	VALIDATE(pItem != NULL, eSTATUS_GRID_INVALID_PTR);
	VALIDATE_BOUNDS(pThis->size, pItem->position);

	i = ARRAY_INDEX(pThis->size, pItem->position);

	return ListInsert(&pThis->pArray[i], &pItem->list);
}

EStatus GridMoveItem(Grid* pThis, GridItem* pItem)
{
	pItem->position.x += pItem->velocity.x;
	pItem->position.y += pItem->velocity.y;
	VALIDATE_BOUNDS(pThis->size, pItem->position);
	return eSTATUS_COMMON_OK;
}

EStatus GridMoveItemTo(Grid* pThis, GridItem* pItem, Position newPosition)
{
    unsigned oldIndex, newIndex;

    VALIDATE_GRID(pThis);
    VALIDATE(pItem, eSTATUS_GRID_INVALID_PTR);
    VALIDATE_BOUNDS(pThis->size, pItem->position);
    VALIDATE_BOUNDS(pThis->size, newPosition);

    oldIndex = ARRAY_INDEX(pThis->size, pItem->position);
    newIndex = ARRAY_INDEX(pThis->size, newPosition);

    if (oldIndex != newIndex)
    {
        GridRemoveItem(pThis, pItem);
        pItem->position = newPosition;
        GridAddItem(pThis, pItem);
    }
    else pItem->position = newPosition;

    return eSTATUS_COMMON_OK;
}

EStatus	GridMoveItems(Grid* pThis)
{
	GridItem *pItem, *pNext;
	unsigned i;
	EStatus rc;
	ListHeader waitingList = {0, 0};

	VALIDATE_GRID(pThis);

	rc = ListCreate(&waitingList);
	CHECK_STATUS_RETURN(rc);

	rc = GridFirstItem(pThis, &pItem);
	CHECK_STATUS_RETURN(rc);

	while (pItem)
	{
		pNext = pItem;
		rc = GridNextItem(pThis, &pNext);

		if (rc && rc != eSTATUS_GRID_LAST_ITEM) return rc;

		i = ARRAY_INDEX(pThis->size, pItem->position);
        GridMoveItem(pThis, pItem);
		if (i != ARRAY_INDEX(pThis->size, pItem->position))
		{
			rc = GridRemoveItem(pThis, pItem);
			CHECK_STATUS_RETURN(rc);

			rc = ListInsert(&waitingList, &pItem->list);
			CHECK_STATUS_RETURN(rc);
		}

		pItem = pNext;
	}

	while ((pItem = (GridItem*)(waitingList.pNext)))
	{
		rc = ListRemove(&pItem->list);
		CHECK_STATUS_BREAK(rc);

		rc = GridAddItem(pThis, pItem);
		if (rc == eSTATUS_GRID_OUT_OF_RANGE)
		{
			DELETE(pItem);
			rc = eSTATUS_COMMON_OK;
		}
		else CHECK_STATUS_BREAK(rc);
	}

	return rc;
}

EStatus GridGetItemAt(Grid* pThis, Position position, GridItem** ppItem)
{
    int index;
    VALIDATE_GRID(pThis);
    VALIDATE(ppItem, eSTATUS_COMMON_INVALID_ARGUMENT);
    VALIDATE_BOUNDS(pThis->size, position);
    index = ARRAY_INDEX(pThis->size, position);
    *ppItem = (GridItem*)pThis->pArray[index].pNext;
    return eSTATUS_COMMON_OK;
}

EStatus GridFirstItem(Grid* pThis, GridItem** ppItem)
{
	VALIDATE_GRID(pThis);
	VALIDATE(ppItem, eSTATUS_COMMON_INVALID_ARGUMENT);

	*ppItem = NULL;
	VALIDATE(pThis->items, eSTATUS_GRID_EMPTY);

	return GridNextItem(pThis, ppItem);
}

EStatus GridNextItem(Grid* pThis, GridItem** ppItem)
{
	unsigned i;
	GridItem* pItem = *ppItem;
	VALIDATE_GRID(pThis);
	VALIDATE(ppItem, eSTATUS_COMMON_INVALID_ARGUMENT);

	if (pItem && NEXT_ITEM(pItem)) *ppItem = NEXT_ITEM(pItem);
	else
	{
		i = (*ppItem) ? ARRAY_INDEX(pThis->size, pItem->position) + 1 : 0;
		*ppItem = NULL;
		while (i < ARRAY_SIZE(pThis->size))
		{
			*ppItem = FIRST_ITEM(pThis->pArray[i]);
			if (*ppItem) return eSTATUS_COMMON_OK;
			++i;
		}
	}
	return eSTATUS_GRID_LAST_ITEM;
}

EStatus GridGetSize(Grid* pThis, Size* pSize)
{
	VALIDATE_GRID(pThis);
	VALIDATE(pSize != NULL, eSTATUS_COMMON_INVALID_ARGUMENT);

	*pSize = pThis->size;
	return eSTATUS_COMMON_OK;
}

EStatus GridGetItemsCount(Grid* pThis, int* pItems)
{
	VALIDATE_GRID(pThis);
	VALIDATE(pItems != NULL, eSTATUS_COMMON_INVALID_ARGUMENT);

	*pItems = pThis->items;
	return eSTATUS_COMMON_OK;
}

EStatus GridIsValid(Grid* pThis)
{
	VALIDATE_GRID(pThis);
	return eSTATUS_COMMON_OK;
}

EStatus GridItemGetPosition(GridItem* pItem, Position* pPosition)
{
	GET_MEMBER(pPosition, pItem, position);
}

EStatus GridItemGetVelocity(GridItem* pItem, Velocity* pVelocity)
{
	GET_MEMBER(pVelocity, pItem, velocity);
}

EStatus GridItemGetStation(GridItem* pItem, Station* pStation)
{
	GET_MEMBER(pStation, pItem, station);
}

EStatus GridItemSetVelocity(GridItem* pItem, Velocity velocity)
{
    SET_MEMBER(velocity, pItem, velocity);
}

EStatus GridItemSetStation(GridItem* pItem, Station station)
{
    SET_MEMBER(station, pItem, station);
}
