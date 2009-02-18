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

#ifndef MESHAPP_H
#define MESHAPP_H

#include <QtGui>
#include "../Document/MeshDocument.h"
#include "../Views/MeshView.h"

class MeshApp : public QMainWindow
{
    Q_OBJECT

public:
    MeshApp(QWidget *parent = 0);
    ~MeshApp();

private:
    void init();
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDocks();
    void createTabs();
    void setDocument(MeshDocument* doc);

    void addDock(Qt::DockWidgetArea area, const QString& title, MeshView* view);
    void addTab(const QIcon& icon, const QString& title, MeshView* view);

private slots:
    void about();
    void aboutQt();

    void simulationStarted();
    void simulationStopped();
    void simulationPaused(bool isPaused);
    void simulationEmpty(bool isEmpty);
    void simulationTime(double time);

private:
    void addView(MeshView* view);
    void removeView(MeshView* view);

    typedef QList<MeshView*>    MeshViews;

    QToolBar*       m_toolbarFile;
    QToolBar*       m_toolbarSimulation;

    QMenu*          m_menuFile;
    QMenu*          m_menuView;
    QMenu*          m_menuSimulation;
    QMenu*          m_menuHelp;

    QAction*        m_actFileNew;
    QAction*        m_actFileOpen;
    QAction*        m_actFileSave;
    QAction*        m_actFileExit;

    QAction*        m_actViewShowSettings;
    QAction*        m_actViewShowSniffer;
    QAction*        m_actViewShowStations;
    QAction*        m_actViewShowGenerator;
    QAction*        m_actViewShowLog;

    QAction*        m_actSimulationReset;
    QAction*        m_actSimulationRun;
    QAction*        m_actSimulationPause;
    QAction*        m_actSimulationBreak;

    QAction*        m_actHelpAbout;
    QAction*        m_actHelpAboutQt;

    QTabWidget*     m_tabs;
    QSlider*        m_sliderSpeed;
    QLabel*         m_simulationTime;
    QProgressBar*   m_simulationProgress;

    MeshViews   m_views;

    MeshDocument*	m_document;
};

#endif // MESHAPP_H
