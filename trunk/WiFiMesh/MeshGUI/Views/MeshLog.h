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

#ifndef MESHLOG_H
#define MESHLOG_H

#include <QtGui>
#include "../Core/MeshCore.h"

class MeshLog : public QListWidget
{
    Q_OBJECT

public:
    MeshLog(QWidget *parent = 0);
    ~MeshLog();

private:
    void init();
    QString moduleName(const QString& filename) const;
	bool filter(ELogSeverity severity, const char* file, const char* function, int line);
	void output(ELogSeverity severity, const char* function, const char* msg);
    void contextMenuEvent(QContextMenuEvent* event);

	static Boolean LogFilter(ELogSeverity severity, const char* file, const char* function, int line, void* pUserArg);
	static void LogOutput(ELogSeverity severity, const char* function, const char* msg, void* pUserArg);

	typedef QMap<QString, QAction*>    ModuleFilters;

	QAction*       m_severityFilters[eSEVERITY_LAST];
	ModuleFilters  m_moduleFilters;
	QAction*       m_actClear;
	QAction*       m_actAutoScroll;
	QMenu*         m_menu;
	QMenu*         m_menuModules;
	QMenu*         m_menuSeverities;
};

#endif // MESHLOG_H
