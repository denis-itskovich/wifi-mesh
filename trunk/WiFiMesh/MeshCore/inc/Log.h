/**
 * \file Log.h
 *
 * Project: MeshCore
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 09/01/2009
 * \author Denis Itskovich
 */


#ifndef MESH_CORE_LOG_H_
#define MESH_CORE_LOG_H_

#include "Status.h"
#include "CommonTypes.h"

/**
 * Log entry severity
 */
typedef enum
{
	eSEVERITY_ERROR,	///< Error packet
	eSEVERITY_WARNING,	///< Warning packet
	eSEVERITY_INFO,		///< Info packet
	eSEVERITY_TRACE,	///< Trace packet
	eSEVERITY_DUMP,		///< Dump packet

	eSEVERITY_LAST		///< terminator
} ELogSeverity;

#if 0
	#define LOG_PRINT(severity, fmt, ...)	LogOutput(severity, __FILE__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#else
	#define LOG_PRINT(severity, fmt, ...) {}
#endif

#define ERROR_PRINT(fmt, ...) 			LOG_PRINT(eSEVERITY_ERROR, fmt, ##__VA_ARGS__)
#define WARNING_PRINT(fmt, ...) 		LOG_PRINT(eSEVERITY_WARNING, fmt, ##__VA_ARGS__)
#define INFO_PRINT(fmt, ...) 			LOG_PRINT(eSEVERITY_INFO, fmt, ##__VA_ARGS__)
#define TRACE_PRINT(fmt, ...) 			LOG_PRINT(eSEVERITY_TRACE, fmt, ##__VA_ARGS__)
#define DUMP_PRINT(fmt, ...) 			LOG_PRINT(eSEVERITY_DUMP, fmt, ##__VA_ARGS__)

#define BEGIN_FUNCTION					TRACE_PRINT(">> %s", __FUNCTION__)
#define END_FUNCTION					TRACE_PRINT("<< %s", __FUNCTION__)

/** Log output callback
 * \param severity [in] log entry severity
 * \param function [in] function name
 * \param msg [in] log entry text
 * \param pUserArg [in] user defined argument
 */
typedef void (*Logger)(ELogSeverity severity, const char* function, const char* msg, void* pUserArg);

/** Log filter callback
 * this callback should decide whether the packet
 * should be printed or not
 * \param severity [in] packet severity
 * \param file [in] source file name
 * \param function [in] function name
 * \param line [in] line in a file name
 * \param pUserArg [in] user defined argument
 * \return TRUE if the packet should be printed
 * \return FALSE if the packet should not be printed
 */
typedef Boolean (*Filter)(ELogSeverity severity, const char* file, const char* function, int line, void* pUserArg);

/** Set log output callback
 * \param logger [in] output callback
 * \param pUserArg [in] user defined argument
 */
EStatus LogSetLogger(Logger logger, void* pUserArg);

/** Set filter callback
 * \param filter [in] filter callback
 * \param pUserArg [in] user defined argument
 */
EStatus LogSetFilter(Filter filter, void* pUserArg);

/** Output log packet
 * \param severity [in] log entry severity
 * \param file [in] source file name, initiated a log packet
 * \param function [in] function name, initiated a log packet
 * \param line [in] line number in source file
 * \param fmt [in] format
 * \param ... [in] additional arguments
 */
void LogOutput(ELogSeverity severity, const char* file, const char* function, int line, const char* fmt, ...);

#endif /* MESH_CORE_LOG_H_ */
