/**
 * \file Log.c
 *
 * Project: MeshCore
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 09/01/2009
 * \author Denis Itskovich
 */

#include "Log.h"
#include "Macros.h"
#include <stdarg.h>
#include <stdio.h>

typedef struct _LogDescriptor
{
	struct
	{
		Logger	callback;	///< logger callback
		void*	pArg;		///< logger user defined argument
	} logger;

	struct
	{
		Filter	callback;	///< filter callback
		void*	pArg;		///< filter user defined argument
	} filter;
} LogDescriptor;			///< Log descriptor structure

static LogDescriptor s_logDesc = {{0, 0}, {0, 0}};	///< Static log descriptor instance

EStatus LogSetLogger(Logger logger, void* pUserArg)
{
	s_logDesc.logger.callback = logger;
	s_logDesc.logger.pArg = pUserArg;
	return eSTATUS_COMMON_OK;
}

EStatus LogSetFilter(Filter filter, void* pUserArg)
{
	s_logDesc.filter.callback = filter;
	s_logDesc.filter.pArg = pUserArg;

	return eSTATUS_COMMON_OK;
}

void LogOutput(ELogSeverity severity, const char* file, const char* function, int line, const char* fmt, ...)
{
	char buff[1024];
	va_list args;

	if (!fmt) return;

	if (s_logDesc.logger.callback)
	{
		if (s_logDesc.filter.callback)
		{
			if (!s_logDesc.filter.callback(severity, file, function, line, s_logDesc.filter.pArg)) return;
		}
		va_start(args, fmt);
		vsprintf(buff, fmt, args);
		va_end(args);
		s_logDesc.logger.callback(severity, function, buff, s_logDesc.logger.pArg);
	}
}
