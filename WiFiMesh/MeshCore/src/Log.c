/*********************************************************************************
WiFi Mesh Simulator PRO
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
 * @file Log.c
 *
 * Project: MeshCore
 * (C) Copyright 2009 Denis Itskovich
 *
 * @date 09/01/2009
 * @author Denis Itskovich
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
