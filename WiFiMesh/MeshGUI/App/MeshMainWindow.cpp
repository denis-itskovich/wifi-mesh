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

#include "MeshMainWindow.h"
#include "../Dialogs/MeshDlgAbout.h"

#include "../Views/MeshViewRandomizer.h"
#include "../Views/MeshViewSettings.h"
#include "../Views/MeshViewStationsList.h"
#include "../Views/MeshViewStationsGraph.h"
#include "../Views/MeshViewSniffer.h"
#include "../Views/MeshViewStatistics.h"
#include "../Views/Widgets/MeshWidgetLog.h"

MeshMainWindow::MeshMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    init();
}

MeshMainWindow::~MeshMainWindow()
{
    if (m_document) delete m_document;
}

void MeshMainWindow::init()
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

void MeshMainWindow::setDocument(MeshDocument* doc)
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
    connect(doc, SIGNAL(simulationCleared()), this, SLOT(simulationCleared()));
    connect(m_sliderSpeed, SIGNAL(valueChanged(int)), m_document, SLOT(setSpeed(int)));
    connect(m_document, SIGNAL(statusChanged(const QString&)), statusBar(), SLOT(showMessage(const QString&)));
    connect(m_document, SIGNAL(timeChanged(double)), this, SLOT(simulationTime(double)));

    foreach (MeshView* view, m_views)
    {
        view->setDocument(m_document);
    }

    m_document->viewsAttached();
}

void MeshMainWindow::createActions()
{
    m_actFileNew = new QAction(QIcon(":/new.png"), tr("&New"), this);
    m_actFileNew->setShortcut(tr("Ctrl+N"));
    m_actFileNew->setStatusTip(tr("Create new simulation"));
    connect(m_actFileNew, SIGNAL(triggered()), this, SLOT(newFile()));

    m_actFileOpen = new QAction(QIcon(":/open.png"), tr("&Open..."), this);
    m_actFileOpen->setShortcut(tr("Ctrl+O"));
    m_actFileOpen->setStatusTip(tr("Open simulation"));
    connect(m_actFileOpen, SIGNAL(triggered()), this, SLOT(openFile()));

    m_actFileSave = new QAction(QIcon(":/save.png"), tr("&Save"), this);
    m_actFileSave->setShortcut(tr("Ctrl+S"));
    m_actFileSave->setStatusTip(tr("Save simulation"));
    connect(m_actFileSave, SIGNAL(triggered()), this, SLOT(saveFile()));

    m_actFileSaveAs = new QAction(QIcon(":/save.png"), tr("Save &As..."), this);
    m_actFileSaveAs->setStatusTip(tr("Save simulation as..."));
    connect(m_actFileSaveAs, SIGNAL(triggered()), this, SLOT(saveFileAs()));

    m_actFileExit = new QAction(QIcon(":/exit.png"), tr("E&xit"), this);
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

void MeshMainWindow::createMenus()
{
    m_menuFile = menuBar()->addMenu(tr("&File"));
    m_menuView = menuBar()->addMenu(tr("&View"));
    m_menuSimulation = menuBar()->addMenu(tr("&Simulation"));
    m_menuHelp = menuBar()->addMenu(tr("&Help"));

    m_menuFile->addAction(m_actFileNew);
    m_menuFile->addAction(m_actFileOpen);
    m_menuFile->addAction(m_actFileSave);
    m_menuFile->addAction(m_actFileSaveAs);
    m_menuFile->addSeparator();
    m_menuFile->addAction(m_actFileExit);

    m_menuSimulation->addAction(m_actSimulationReset);
    m_menuSimulation->addAction(m_actSimulationRun);
    m_menuSimulation->addAction(m_actSimulationPause);
    m_menuSimulation->addAction(m_actSimulationBreak);

    m_menuHelp->addAction(m_actHelpAbout);
    m_menuHelp->addAction(m_actHelpAboutQt);
}

void MeshMainWindow::createToolBars()
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

void MeshMainWindow::createStatusBar()
{
    m_simulationTime = new QLabel;
    m_simulationProgress = new QProgressBar;
    m_simulationProgress->setMaximumWidth(128);
    m_simulationProgress->setRange(0, 1000);

    statusBar()->setSizeGripEnabled(true);
    statusBar()->addPermanentWidget(m_simulationTime, 0);
    statusBar()->addPermanentWidget(m_simulationProgress, 0);
    statusBar()->showMessage(tr("Ready"));
}

void MeshMainWindow::simulationTime(double time)
{
    m_simulationTime->setText(QString("Time: %1 [msec]").arg(time * 1000, 0, 'f', 5));
    m_simulationProgress->setValue((int)(1000.0 * time / m_document->simulationDuration()));
}

void MeshMainWindow::createDocks()
{
    setDockOptions(dockOptions() | AllowNestedDocks);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    addDock(Qt::LeftDockWidgetArea, tr("Stations browser"), new MeshViewStationsList, tr("Alt+1"));
    addDock(Qt::BottomDockWidgetArea, tr("Packet sniffer"), new MeshViewSniffer, tr("Alt+2"));
    addDock(Qt::RightDockWidgetArea, tr("Simulator settings"), new MeshViewSettings, tr("Alt+3"));
    addDock(Qt::RightDockWidgetArea, tr("World generator"), new MeshViewRandomizer, tr("Alt+4"));

    m_tabs->setCurrentIndex(0);
}

void MeshMainWindow::addTab(const QIcon& icon, const QString& str, MeshView* view)
{
    m_tabs->addTab(view, icon, str);
    addView(view);
}

void MeshMainWindow::createTabs()
{
    m_tabs = new QTabWidget(this);
    setCentralWidget(m_tabs);

    addTab(QIcon(":/map.png"), tr("World map"), new MeshViewStationsGraph);
    addTab(QIcon(":/statistics.png"), tr("Statistics"), new MeshViewStatistics);
}

void MeshMainWindow::addView(MeshView* view)
{
    m_views << view;
}

void MeshMainWindow::addDock(Qt::DockWidgetArea area, const QString& title, MeshView* view, const QKeySequence shortcut)
{
    QDockWidget* dock = new QDockWidget(title, this);
    addView(view);
    dock->setWidget(view);
    QAction* actToggleView = dock->toggleViewAction();
    actToggleView->setShortcut(shortcut);
    m_menuView->addAction(actToggleView);
    addDockWidget(area, dock);
}

bool MeshMainWindow::getFilename(bool isOpenning)
{
    if (isOpenning) m_filename = QFileDialog::getOpenFileName(this, tr("Open simulation file"), m_filename, tr("*.txt"));
    else m_filename = QFileDialog::getSaveFileName(this, tr("Save simulation to file"), m_filename, tr("*.txt"));

    if (m_filename.isEmpty()) return false;
    m_recentDir = QFileInfo(m_filename).dir().dirName();
    return true;
}

void MeshMainWindow::newFile()
{
    m_filename.clear();
    m_document->clear();
}

void MeshMainWindow::openFile()
{
    if (!getFilename(true)) return;
    m_document->importFromFile(m_filename);
}

void MeshMainWindow::saveFile()
{
    if (m_filename.isEmpty()) saveFileAs();
    else m_document->exportToFile(m_filename);
}

void MeshMainWindow::saveFileAs()
{
    if (!getFilename(false)) return;
    m_document->exportToFile(m_filename);
}

void MeshMainWindow::about()
{
    MeshDlgAbout().exec();
}

void MeshMainWindow::aboutQt()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

void MeshMainWindow::simulationCleared()
{
    simulationTime(0);
}

void MeshMainWindow::simulationStarted()
{
     m_actSimulationRun->setEnabled(m_actSimulationPause->isChecked());
     m_actSimulationReset->setEnabled(false);
     m_actSimulationBreak->setEnabled(true);
}

void MeshMainWindow::simulationStopped()
{
     m_actSimulationRun->setEnabled(true);
     m_actSimulationReset->setEnabled(true);
     m_actSimulationBreak->setEnabled(false);
}

void MeshMainWindow::simulationPaused(bool isPaused)
{
    m_actSimulationRun->setEnabled(m_actSimulationPause->isChecked() || !m_actSimulationBreak->isEnabled());
}

void MeshMainWindow::simulationEmpty(bool isEmpty)
{
     m_actSimulationRun->setEnabled(!isEmpty);
}
