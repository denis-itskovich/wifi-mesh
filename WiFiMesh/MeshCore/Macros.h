/**
 * \file Macros.h
 *
 * Functional macros
 *
 * \author Denis Itskovich
 * \date 01/12/2008
 */

#ifndef _WIFI_MESH_MACROS_H
#define _WIFI_MESH_MACROS_H

#include "Status.h"

/** Used in other macros in order to allow usage of macros inside if/else block
 * \param operation Code block
 */
#define SAFE_OPERATION(operation)				do {operation;} while (0)

/** Converts double value into rounded long
 * \param val double value
 */
#define ROUND(val)								(long)((val) + 0.5)

/** Checks a condition and returns an error if a condition not satisfied
 * \param condition boolean expression
 * \param error error code to be returned
 */
#define VALIDATE(condition, error)				SAFE_OPERATION(if (!(condition)) return (error))

/** Checks a condition and returns eSTATUS_COMMON_INVALID_ARGUMENT if a condition not satisfied
 * \param condition boolean expression
 */
#define VALIDATE_ARGUMENTS(condition)			VALIDATE(condition, eSTATUS_COMMON_INVALID_ARGUMENT)

/** Zeroes a struct according to its size
 * \param var variable to be cleared
 * \sa CLEAR_ARRAY, NEW, DELETE
 */
#define CLEAR(var)								SAFE_OPERATION(memset(var, 0, sizeof(*var)))

/** Zeroes an array
 * \param var pointer to array to be cleared
 * \param size items count
 * \sa CLEAR, NEW_ARRAY, DELETE
 */
#define CLEAR_ARRAY(var, size)					SAFE_OPERATION(memset(var, 0, sizeof(var[0]) * ARRAY_SIZE(size)))

/** Allocates a memory for a type according to its size
 * \param type data type to be allocated
 * \sa NEW_ARRAY, DELETE, CLEAR
 */
#define NEW(type)								(type*)(malloc(sizeof(type)))

/** Allocates an array
 * \param type item type
 * \param size items count
 * \sa NEW, DELETE, CLEAR_ARRAY
 */
#define NEW_ARRAY(type, size)					(type*)(malloc(ARRAY_BYTE_SIZE(type, size)))

/** Frees previousely allocated variable
 * \param ptr pointer to allocated variable
 * \sa NEW, NEW_ARRAY
 */
#define DELETE(ptr)								SAFE_OPERATION(free(ptr); ptr = NULL)

/** Checks return code and exits function if not successfull
 * \param rc return code to be checked
 */
#define CHECK_STATUS(rc)						SAFE_OPERATION(if ((rc) != eSTATUS_COMMON_OK) return (rc))

/** Checks return code and breaks a loop if not successfull
 * \param rc return code to be checked
 */
#define CHECK_STATUS_BREAK(rc)					{if ((rc) != eSTATUS_COMMON_OK) break;}

/** Retrieves structure member value, previously validating the pointers
 * \param destination pointer to destination
 * \param ptr pointer to structure
 * \param member member name to be retrieved
 * \sa SET_MEMBER
 */
#define GET_MEMBER(destination, ptr, member)	SAFE_OPERATION(VALIDATE_ARGUMENTS(ptr && destination); *destination = ptr->member; return eSTATUS_COMMON_OK;)

/** Sets structure member, previously validating a pointer
 * \param source source value
 * \param ptr pointer to structure
 * \param member member name to be set
 * \sa GET_MEMBER
 */
#define SET_MEMBER(source, ptr, member)			SAFE_OPERATION(VALIDATE_ARGUMENTS(ptr); ptr->member = source; return eSTATUS_COMMON_OK;)

/** Allocates a structure, validating a pointer
 * \param pptr *pptr will be initialized with allocated structure
 * \param type data type to be allocated
 * \param cond additional condition to be checked during arguments validation
 * \sa NEW
 */
#define CONSTRUCT(pptr, type, cond)				SAFE_OPERATION ( \
													VALIDATE(pptr && cond, eSTATUS_COMMON_INVALID_ARGUMENT); \
													*pptr = NEW(type); \
													VALIDATE(*pptr, eSTATUS_COMMON_NO_MEMORY); \
												)

#endif // _WIFI_MESH_MACROS_H
