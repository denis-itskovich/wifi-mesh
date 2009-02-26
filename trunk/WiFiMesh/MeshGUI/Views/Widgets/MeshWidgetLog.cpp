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

#include "MeshWidgetLog.h"

static const char* TXT_SEVERITIES[eSEVERITY_LAST] =
{
		"ERROR",
		"WARN ",
		"INFO ",
		"TRACE",
		"DUMP "
};

static const char* NAME_SEVERITIES[eSEVERITY_LAST] =
{
        "errors",
        "warnings",
        "info",
        "trace",
        "dump"
};

static const QRgb COLOR_SEVERITIES[eSEVERITY_LAST] =
{
		0x00ff7f7f,
		0x00ffbf7f,
		0x007fff7f,
		0x007fbfff,
		0x007fffff
};

MeshWidgetLog::MeshWidgetLog(QWidget *parent)
    : QListWidget(parent)
{
	LogSetLogger(&MeshWidgetLog::LogOutput, this);
	LogSetFilter(&MeshWidgetLog::LogFilter, this);
	init();
}

MeshWidgetLog::~MeshWidgetLog()
{
	LogSetLogger(0, 0);
	LogSetFilter(0, 0);
	delete m_menu;
}

void MeshWidgetLog::init()
{
    this->setFont(QFont("Courier"));
    m_actClear = new QAction(QIcon(":/clear.png"), tr("&Clear"), this);
    connect(m_actClear, SIGNAL(triggered()), this, SLOT(clear()));

    m_actAutoScroll = new QAction(tr("&Auto scroll"), this);
    m_actAutoScroll->setCheckable(true);
    m_actAutoScroll->setChecked(true);

    m_menu = new QMenu;
    m_menuSeverities = new QMenu(tr("Severity filter"));
    m_menuModules = new QMenu(tr("Module filter"));

    m_menu->addAction(m_actClear);
    m_menu->addAction(m_actAutoScroll);

    for (int i = 0; i < eSEVERITY_LAST; ++i)
    {
        QAction* action = new QAction(tr("Show %1").arg(NAME_SEVERITIES[i]), this);
        action->setCheckable(true);
        action->setChecked(false);
        m_severityFilters[i] = action;
        m_menuSeverities->addAction(action);
    }

    m_menu->addMenu(m_menuSeverities);
}

QString MeshWidgetLog::moduleName(const QString& filename) const
{
    QString module = filename;
    module.replace('\\', '/');
    int end = module.lastIndexOf('.');
    int begin = module.lastIndexOf('/') + 1;

    return module.mid(begin, end - begin);
}

Boolean MeshWidgetLog::LogFilter(ELogSeverity severity, const char* file, const char* function, int line, void* pUserArg)
{
	return ((MeshWidgetLog*)pUserArg)->filter(severity, file, function, line) ? TRUE : FALSE;
}

void MeshWidgetLog::LogOutput(ELogSeverity severity, const char* function, const char* msg, void* pUserArg)
{
	((MeshWidgetLog*)pUserArg)->output(severity, function, msg);
}

bool MeshWidgetLog::filter(ELogSeverity severity, const char* file, const char* function, int line)
{
	QString module = moduleName(file);
	if (!m_moduleFilters.count(module))
	{
	    QAction* action = new QAction(tr("Show %1 module").arg(module), this);
	    action->setCheckable(true);
	    action->setChecked(false);
	    m_menuModules->addAction(action);
        if (m_moduleFilters.isEmpty()) m_menu->addMenu(m_menuModules);
	    m_moduleFilters[module] = action;
	}
	return m_severityFilters[severity]->isChecked() && m_moduleFilters[module]->isChecked();
}

void MeshWidgetLog::output(ELogSeverity severity, const char* function, const char* msg)
{
	QString str(msg);
	str = tr("[%1] [%2] : %3").arg(TXT_SEVERITIES[severity]).arg(function).arg(str);
	QListWidgetItem* item = new QListWidgetItem(str);
	item->setBackgroundColor(COLOR_SEVERITIES[severity]);
	addItem(item);
	if (m_actAutoScroll->isChecked()) scrollToItem(item);
}

void MeshWidgetLog::contextMenuEvent(QContextMenuEvent* event)
{
    m_menu->exec(event->globalPos());
}
