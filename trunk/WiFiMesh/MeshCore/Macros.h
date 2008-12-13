#ifndef _WIFI_MESH_MACROS_H
#define _WIFI_MESH_MACROS_H

#include "Status.h"

#define SAFE_OPERATION(operation)				do {operation;} while (0)
#define ROUND(x)								(long)((x) + 0.5)

#define VALIDATE(condition, error)				SAFE_OPERATION(if (!(condition)) return (error))
#define VALIDATE_ARGUMENTS(condition)			VALIDATE(condition, eSTATUS_COMMON_INVALID_ARGUMENT)

#define ARRAY_SIZE(size)						((size.x) * (size.y))
#define ARRAY_BYTE_SIZE(type, size)				(sizeof(type) * ARRAY_SIZE(size))

#define CLEAR(var)								SAFE_OPERATION(memset(var, 0, sizeof(*var)))
#define CLEAR_ARRAY(var, size)					SAFE_OPERATION(memset(var, 0, sizeof(var[0]) * ARRAY_SIZE(size)))

#define NEW(type)								(type*)(malloc(sizeof(type)))
#define NEW_ARRAY(type, size)					(type*)(malloc(ARRAY_BYTE_SIZE(type, size)))

#define DELETE(ptr)								SAFE_OPERATION(free(ptr); ptr = NULL)

#define CHECK_STATUS(rc)						SAFE_OPERATION(if ((rc) != eSTATUS_COMMON_OK) return (rc))
#define CHECK_STATUS_BREAK(rc)					{if ((rc) != eSTATUS_COMMON_OK) break;}

#define GET_MEMBER(destination, ptr, member)	SAFE_OPERATION(VALIDATE_ARGUMENTS(ptr && destination); *destination = ptr->member; return eSTATUS_COMMON_OK;)
#define SET_MEMBER(source, ptr, member)			SAFE_OPERATION(VALIDATE_ARGUMENTS(ptr); ptr->member = source; return eSTATUS_COMMON_OK;)

#define CONSTRUCT(pptr, type, cond)				SAFE_OPERATION \
												( \
													VALIDATE(pptr && cond, eSTATUS_COMMON_INVALID_ARGUMENT); \
													*pptr = NEW(type); \
													VALIDATE(*pptr, eSTATUS_COMMON_NO_MEMORY); \
												)

typedef enum { FALSE, TRUE } Boolean;

#endif // _WIFI_MESH_MACROS_H
