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

#include "MeshLog.h"

static const char* TXT_SEVERITIES[eSEVERITY_LAST] =
{
		"ERROR",
		"WARN ",
		"INFO ",
		"TRACE",
		"DUMP "
};

static const QRgb COLOR_SEVERITIES[eSEVERITY_LAST] =
{
		0x00ff7f7f,
		0x00ffbf7f,
		0x007fff7f,
		0x007fbfff,
		0x007fffff
};

MeshLog::MeshLog(QWidget *parent)
    : QListWidget(parent)
{
	LogSetLogger(&MeshLog::LogOutput, this);
	LogSetFilter(&MeshLog::LogFilter, this);
	this->setFont(QFont("Courier"));
}

MeshLog::~MeshLog()
{
	LogSetLogger(0, 0);
	LogSetFilter(0, 0);
}

Boolean MeshLog::LogFilter(ELogSeverity severity, const char* file, const char* function, int line, void* pUserArg)
{
	return ((MeshLog*)pUserArg)->filter(severity, file, function, line) ? TRUE : FALSE;
}

void MeshLog::LogOutput(ELogSeverity severity, const char* function, const char* msg, void* pUserArg)
{
	((MeshLog*)pUserArg)->output(severity, function, msg);
}

bool MeshLog::filter(ELogSeverity severity, const char* file, const char* function, int line)
{
	return true;
}

void MeshLog::output(ELogSeverity severity, const char* function, const char* msg)
{
	QString str(msg);
	str = tr("[%1] [%2] : %3").arg(TXT_SEVERITIES[severity]).arg(function).arg(str);
	QListWidgetItem* pItem = new QListWidgetItem(str);
	pItem->setBackgroundColor(COLOR_SEVERITIES[severity]);
	addItem(pItem);
}
