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

#include "MeshApp.h"
#include "../Dialogs/MeshDlgAbout.h"

#include "../Views/MeshViewRandomizer.h"
#include "../Views/MeshViewStationProperties.h"
#include "../Views/MeshViewSettings.h"
#include "../Views/MeshViewStationsList.h"
#include "../Views/MeshViewStationsGraph.h"
#include "../Views/MeshViewSniffer.h"
#include "../Views/MeshViewStatistics.h"
#include "../Views/Widgets/MeshWidgetLog.h"

MeshApp::MeshApp(QWidget *parent)
    : QMainWindow(parent)
{
	init();
}

MeshApp::~MeshApp()
{
	if (m_document) delete m_document;
}

void MeshApp::init()
{
	setWindowTitle(qApp->applicationName());
	setWindowIcon(QIcon(":/station.png"));

	createActions();
	createMenus();
	createToolBars();
	createTabs();
	createDocks();
	createStatusBar();

	setDocument(new MeshDocument());
}

void MeshApp::setDocument(MeshDocument* doc)
{
	m_document = doc;

	m_sliderSpeed->setRange(1, m_document->maximumSpeed());
    m_sliderSpeed->setValue(m_document->speed());

    connect(m_actSimulationReset, SIGNAL(triggered()), doc, SLOT(reset()));
	connect(m_actSimulationRun, SIGNAL(triggered()), doc, SLOT(start()));
	connect(m_actSimulationPause, SIGNAL(toggled(bool)), doc, SLOT(togglePause(bool)));
	connect(m_actSimulationBreak, SIGNAL(triggered()), doc, SLOT(stop()));
	connect(doc, SIGNAL(simulationStarted()), this, SLOT(simulationStarted()));
	connect(doc, SIGNAL(simulationStopped()), this, SLOT(simulationStopped()));
	connect(m_actFileNew, SIGNAL(triggered()), doc, SLOT(clear()));
    connect(m_sliderSpeed, SIGNAL(valueChanged(int)), m_document, SLOT(setSpeed(int)));
    connect(m_document, SIGNAL(timeChanged(const QString&)), m_simulationTime, SLOT(setText(const QString&)));
    connect(m_document, SIGNAL(statusChanged(const QString&)), statusBar(), SLOT(showMessage(const QString&)));

    foreach (MeshView* view, m_views)
    {
        view->setDocument(m_document);
    }
}

void MeshApp::createActions()
{
	m_actFileNew = new QAction(QIcon(":/new.png"), tr("&New"), this);
	m_actFileNew->setShortcut(tr("Ctrl+N"));
	m_actFileNew->setStatusTip(tr("Create new simulation"));

	m_actFileOpen = new QAction(QIcon(":/open.png"), tr("&Open..."), this);
	m_actFileOpen->setShortcut(tr("Ctrl+O"));
	m_actFileOpen->setStatusTip(tr("Open simulation"));

	m_actFileSave = new QAction(QIcon(":/save.png"), tr("&Save..."), this);
	m_actFileSave->setShortcut(tr("Ctrl+S"));
	m_actFileSave->setStatusTip(tr("Save simulation"));

	m_actFileExit =	new QAction(QIcon(":/exit.png"), tr("E&xit"), this);
	m_actFileExit->setStatusTip(tr("Quit WiFi Mesh simulator"));
	connect(m_actFileExit, SIGNAL(triggered()), this, SLOT(close()));

    m_actSimulationReset = new QAction(QIcon(":/reset.png"), tr("&Reset"), this);
    m_actSimulationReset->setShortcut(tr("Shift+F4"));
    m_actSimulationReset->setStatusTip(tr("Reset simulation"));

    m_actSimulationRun = new QAction(QIcon(":/play.png"), tr("&Run"), this);
	m_actSimulationRun->setShortcut(tr("F5"));
	m_actSimulationRun->setStatusTip(tr("Run simulation"));

	m_actSimulationPause = new QAction(QIcon(":/pause.png"), tr("&Pause"), this);
	m_actSimulationPause->setShortcut(tr("F4"));
	m_actSimulationPause->setStatusTip(tr("Pause simulation"));
	m_actSimulationPause->setCheckable(true);
	connect(m_actSimulationPause, SIGNAL(toggled(bool)), this, SLOT(simulationPaused(bool)));

	m_actSimulationBreak = new QAction(QIcon(":/stop.png"), tr("&Break"), this);
	m_actSimulationBreak->setShortcut(tr("Shift+F5"));
	m_actSimulationBreak->setStatusTip(tr("Break simulation"));
	m_actSimulationBreak->setEnabled(false);

	m_actHelpAbout = new QAction(QIcon(":/about.png"), tr("&About..."), this);
	m_actHelpAbout->setStatusTip(tr("About WiFi Mesh simulator"));
	connect(m_actHelpAbout, SIGNAL(triggered()), this, SLOT(about()));

	m_actHelpAboutQt = new QAction(QIcon(":/qt_logo.png"), tr("About &Qt..."), this);
	m_actHelpAboutQt->setStatusTip(tr("About Qt"));
	connect(m_actHelpAboutQt, SIGNAL(triggered()), this, SLOT(aboutQt()));
}

void MeshApp::createMenus()
{
	m_menuFile = menuBar()->addMenu(tr("&File"));
	m_menuView = menuBar()->addMenu(tr("&View"));
	m_menuSimulation = menuBar()->addMenu(tr("&Simulation"));
	m_menuHelp = menuBar()->addMenu(tr("&Help"));

	m_menuFile->addAction(m_actFileNew);
	m_menuFile->addAction(m_actFileOpen);
	m_menuFile->addAction(m_actFileSave);
	m_menuFile->addSeparator();
	m_menuFile->addAction(m_actFileExit);

	m_menuSimulation->addAction(m_actSimulationReset);
	m_menuSimulation->addAction(m_actSimulationRun);
	m_menuSimulation->addAction(m_actSimulationPause);
	m_menuSimulation->addAction(m_actSimulationBreak);

	m_menuHelp->addAction(m_actHelpAbout);
	m_menuHelp->addAction(m_actHelpAboutQt);
}

void MeshApp::createToolBars()
{
	m_toolbarFile = addToolBar(tr("File"));
	m_toolbarFile->setIconSize(QSize(24, 24));
	m_toolbarSimulation = addToolBar(tr("Simulation"));
	m_toolbarSimulation->setIconSize(QSize(24, 24));

	m_toolbarFile->addAction(m_actFileNew);
	m_toolbarFile->addAction(m_actFileOpen);
	m_toolbarFile->addAction(m_actFileSave);

	m_toolbarSimulation->addAction(m_actSimulationReset);
	m_toolbarSimulation->addAction(m_actSimulationRun);
	m_toolbarSimulation->addAction(m_actSimulationPause);
	m_toolbarSimulation->addAction(m_actSimulationBreak);

	m_sliderSpeed = new QSlider(Qt::Horizontal);
	m_sliderSpeed->setTickPosition(QSlider::TicksBelow);
	m_sliderSpeed->setMaximumWidth(200);

	m_toolbarSimulation->addSeparator();
	m_toolbarSimulation->addWidget(new QLabel(tr("Speed: min")));
	m_toolbarSimulation->addWidget(m_sliderSpeed);
	m_toolbarSimulation->addWidget(new QLabel(tr("max")));
}

void MeshApp::createStatusBar()
{
    m_simulationTime = new QLabel;
    statusBar()->addPermanentWidget(m_simulationTime, 0);
	statusBar()->showMessage(tr("Ready"));
}

void MeshApp::createDocks()
{
    setDockOptions(dockOptions() | AllowNestedDocks);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

	addDock(Qt::LeftDockWidgetArea, tr("Stations browser"), new MeshViewStationsList);
	addDock(Qt::BottomDockWidgetArea, tr("Packet sniffer"), new MeshViewSniffer);
    addDock(Qt::RightDockWidgetArea, tr("Simulator settings"), new MeshViewSettings);
    addDock(Qt::RightDockWidgetArea, tr("World generator"), new MeshViewRandomizer);

	m_tabs->setCurrentIndex(0);
}

void MeshApp::addTab(const QIcon& icon, const QString& str, MeshView* view)
{
    m_tabs->addTab(view, icon, str);
    addView(view);
}

void MeshApp::createTabs()
{
	m_tabs = new QTabWidget(this);
	setCentralWidget(m_tabs);

	addTab(QIcon(":/map.png"), tr("World map"), new MeshViewStationsGraph);
	addTab(QIcon(":/statistics.png"), tr("Statistics"), new MeshViewStatistics);
}

void MeshApp::addView(MeshView* view)
{
    m_views << view;
}

void MeshApp::addDock(Qt::DockWidgetArea area, const QString& title, MeshView* view)
{
	QDockWidget* dock = new QDockWidget(title, this);
	addView(view);
	dock->setWidget(view);
	m_menuView->addAction(dock->toggleViewAction());
	addDockWidget(area, dock);
}


void MeshApp::about()
{
	MeshDlgAbout().exec();
}

void MeshApp::aboutQt()
{
	QMessageBox::aboutQt(this, tr("About Qt"));
}

void MeshApp::simulationStarted()
{
	 m_actSimulationRun->setEnabled(m_actSimulationPause->isChecked());
     m_actSimulationReset->setEnabled(false);
	 m_actSimulationBreak->setEnabled(true);
}

void MeshApp::simulationStopped()
{
	 m_actSimulationRun->setEnabled(true);
     m_actSimulationReset->setEnabled(true);
	 m_actSimulationBreak->setEnabled(false);
}

void MeshApp::simulationPaused(bool isPaused)
{
    m_actSimulationRun->setEnabled(m_actSimulationPause->isChecked() || !m_actSimulationBreak->isEnabled());
}

void MeshApp::simulationEmpty(bool isEmpty)
{
	 m_actSimulationRun->setEnabled(!isEmpty);
}
