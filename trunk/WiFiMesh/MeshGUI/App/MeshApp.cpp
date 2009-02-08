#include "MeshApp.h"
#include "../Dialogs/MeshDlgAbout.h"

#include "../MeshViews/MeshViewRandomizer.h"
#include "../MeshViews/MeshViewStationProperties.h"
#include "../MeshViews/MeshViewSettings.h"
#include "../MeshViews/MeshViewStationsList.h"
#include "../MeshViews/MeshViewStationsGraph.h"
#include "../MeshViews/MeshViewSniffer.h"
#include "../MeshViews/MeshLog.h"

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
	setWindowTitle(tr("WiFi Mesh Simulator - Professional Edition"));
	setWindowIcon(QIcon(":/station.png"));

	createActions();
	setDocument(new MeshDocument());

	createMenus();
	createToolBars();
	createTabs();
	createDocks();
	createWidgets();
	createStatusBar();
}

void MeshApp::setDocument(MeshDocument* doc)
{
	m_document = doc;
	connect(m_actSimulationRun, SIGNAL(triggered()), doc, SLOT(start()));
	connect(m_actSimulationPause, SIGNAL(toggled(bool)), doc, SLOT(togglePause(bool)));
	connect(m_actSimulationBreak, SIGNAL(triggered()), doc, SLOT(stop()));
	connect(doc, SIGNAL(simulationStarted()), this, SLOT(simulationStarted()));
	connect(doc, SIGNAL(simulationStopped()), this, SLOT(simulationStopped()));
}

void MeshApp::createActions()
{
	m_actFileNew = new QAction(QIcon(":/new.png"), tr("&New"), this);
	m_actFileNew->setShortcut(tr("Ctrl+N"));
	m_actFileNew->setStatusTip(tr("Create new simulation"));
	m_actFileNew->setEnabled(false);

	m_actFileOpen = new QAction(QIcon(":/open.png"), tr("&Open..."), this);
	m_actFileOpen->setShortcut(tr("Ctrl+O"));
	m_actFileOpen->setStatusTip(tr("Open simulation"));
	m_actFileOpen->setEnabled(false);

	m_actFileSave = new QAction(QIcon(":/save.png"), tr("&Save..."), this);
	m_actFileSave->setShortcut(tr("Ctrl+S"));
	m_actFileSave->setStatusTip(tr("Save simulation"));
	m_actFileSave->setEnabled(false);

	m_actFileClose = new QAction(QIcon(":/close.png"), tr("&Close"), this);
	m_actFileClose->setStatusTip(tr("Close simulation"));
	m_actFileClose->setEnabled(false);

	m_actFileExit =	new QAction(QIcon(":/exit.png"), tr("E&xit"), this);
	m_actFileExit->setStatusTip(tr("Quit WiFi Mesh simulator"));
	connect(m_actFileExit, SIGNAL(triggered()), this, SLOT(close()));

//	m_actViewShowBackground = new QAction(tr("Show &background"), this);
//	m_actViewShowBackground->setShortcut(tr("Ctrl+1"));
//	m_actViewShowBackground->setStatusTip(tr("Show background"));
//
//	m_actViewShowGrid =	new QAction(tr("Show &grid"), this);
//	m_actViewShowGrid->setShortcut(tr("Ctrl+2"));
//	m_actViewShowGrid->setStatusTip(tr("Show grid"));
//
//	m_actViewShowRules = new QAction(tr("Show &rules"), this);
//	m_actViewShowRules->setShortcut(tr("Ctrl+3"));
//	m_actViewShowRules->setStatusTip(tr("Show rules"));
//
//	m_actViewShowStations =	new QAction(tr("Show &stations"), this);
//	m_actViewShowStations->setShortcut(tr("Ctrl+4"));
//	m_actViewShowStations->setStatusTip(tr("Show stations"));
//
//	m_actViewShowDataFlow = new QAction(tr("Show &data flow"), this);
//	m_actViewShowDataFlow->setShortcut(tr("Ctrl+5"));
//	m_actViewShowDataFlow->setStatusTip(tr("Show data flow"));
//
//	m_actViewShowRouting = new QAction(tr("Show r&outing"), this);
//	m_actViewShowRouting->setShortcut(tr("Ctrl+6"));
//	m_actViewShowRouting->setStatusTip(tr("Show routing"));
//
//	m_actViewShowCoverage = new QAction(tr("Show &coverage"), this);
//	m_actViewShowCoverage->setShortcut(tr("Ctrl+7"));
//	m_actViewShowCoverage->setStatusTip(tr("Show coverage"));

//	m_actViewShowSettings = new QAction(QIcon(":/configure.png"), tr("Show &simulator settings"), this);
//	m_actViewShowSettings->setStatusTip(tr("Show/hide simulator settings window"));
//	m_actViewShowSettings->setCheckable(true);
//	m_actViewShowSettings->setChecked(true);
//
//	m_actViewShowSniffer = new QAction(QIcon(":/packet.png"), tr("Show &packets sniffer"), this);
//	m_actViewShowSniffer->setStatusTip(tr("Show/hide packets sniffer window"));
//	m_actViewShowSniffer->setCheckable(true);
//	m_actViewShowSniffer->setChecked(true);
//
//	m_actViewShowStations = new QAction(QIcon(":/station.png"), tr("Show s&tations view"), this);
//	m_actViewShowStations->setStatusTip(tr("Show/hide stations view window"));
//	m_actViewShowStations->setCheckable(true);
//	m_actViewShowStations->setChecked(true);
//
//	m_actViewShowGenerator = new QAction(QIcon(":/generate.png"), tr("Show world &generator"), this);
//	m_actViewShowGenerator->setStatusTip(tr("Show/hide world generator window"));
//	m_actViewShowGenerator->setCheckable(true);
//	m_actViewShowGenerator->setChecked(true);
//
//	m_actViewShowLog = new QAction(QIcon(":/log.png"), tr("Show world &generator"), this);
//	m_actViewShowLog->setStatusTip(tr("Show/hide world generator window"));
//	m_actViewShowLog->setCheckable(true);
//	m_actViewShowLog->setChecked(true);

	m_actSimulationRun = new QAction(QIcon(":/play.png"), tr("&Run"), this);
	m_actSimulationRun->setShortcut(tr("F5"));
	m_actSimulationRun->setStatusTip(tr("Run simulation"));
//	m_actSimulationRun->setEnabled(false);

	m_actSimulationPause = new QAction(QIcon(":/pause.png"), tr("&Pause"), this);
	m_actSimulationPause->setShortcut(tr("F5"));
	m_actSimulationPause->setStatusTip(tr("Pause simulation"));
	m_actSimulationPause->setEnabled(false);
	m_actSimulationPause->setCheckable(true);

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
	m_menuFile->addAction(m_actFileClose);
	m_menuFile->addSeparator();
	m_menuFile->addAction(m_actFileExit);

//	m_menuView->addAction(m_actViewShowBackground);
//	m_menuView->addAction(m_actViewShowGrid);
//	m_menuView->addAction(m_actViewShowRules);
//	m_menuView->addAction(m_actViewShowStations);
//	m_menuView->addAction(m_actViewShowDataFlow);
//	m_menuView->addAction(m_actViewShowRouting);
//	m_menuView->addAction(m_actViewShowCoverage);

//	m_menuView->addAction(m_actViewShowSettings);
//	m_menuView->addAction(m_actViewShowSniffer);
//	m_menuView->addAction(m_actViewShowStations);
//	m_menuView->addAction(m_actViewShowGenerator);

	m_menuSimulation->addAction(m_actSimulationRun);
	m_menuSimulation->addAction(m_actSimulationPause);
	m_menuSimulation->addAction(m_actSimulationBreak);

	m_menuHelp->addAction(m_actHelpAbout);
	m_menuHelp->addAction(m_actHelpAboutQt);
}

void MeshApp::createWidgets()
{
}

void MeshApp::createToolBars()
{
	m_toolbarFile = addToolBar(tr("File"));
	m_toolbarFile->setIconSize(QSize(24, 24));
	m_toolbarSimulation = addToolBar(tr("Simulation"));
	m_toolbarSimulation->setIconSize(QSize(24, 24));

//	m_toolbarView = addToolBar(tr("View"));
//	m_toolbarView->setIconSize(QSize(24, 24));

	m_toolbarFile->addAction(m_actFileNew);
	m_toolbarFile->addAction(m_actFileOpen);
	m_toolbarFile->addAction(m_actFileSave);
//	m_toolbarFile->addAction(m_actFileClose);

	m_toolbarSimulation->addAction(m_actSimulationRun);
	m_toolbarSimulation->addAction(m_actSimulationPause);
	m_toolbarSimulation->addAction(m_actSimulationBreak);

//	m_toolbarView->addAction(m_actViewShowSettings);
//	m_toolbarView->addAction(m_actViewShowSniffer);
//	m_toolbarView->addAction(m_actViewShowStations);
//	m_toolbarView->addAction(m_actViewShowGenerator);
}

void MeshApp::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

void MeshApp::createDocks()
{
	QDockWidget* dockStations = createDock(tr("Stations browser"), new MeshViewStationsList(this));
	QDockWidget* dockRandomizer = createDock(tr("World generator"), new MeshViewRandomizer(this));
	QDockWidget* dockSettings = createDock(tr("Simulator settings"), new MeshViewSettings(this));
	QDockWidget* dockPackets = createDock(tr("Packet sniffer"), new MeshViewSniffer(this));

//	connect(m_actViewShowStations, SIGNAL(toggled(bool)), dockStations, SLOT(setVisible(bool)));
//	connect(m_actViewShowSettings, SIGNAL(toggled(bool)), dockSettings, SLOT(setVisible(bool)));
//	connect(m_actViewShowSniffer, SIGNAL(toggled(bool)), dockPackets, SLOT(setVisible(bool)));
//	connect(m_actViewShowGenerator, SIGNAL(toggled(bool)), dockRandomizer, SLOT(setVisible(bool)));

	addDockWidget(Qt::RightDockWidgetArea, dockSettings);
	addDockWidget(Qt::RightDockWidgetArea, dockRandomizer);
	addDockWidget(Qt::BottomDockWidgetArea, dockPackets);
	addDockWidget(Qt::LeftDockWidgetArea, dockStations);

	m_tabs->setCurrentIndex(0);
}

void MeshApp::createTabs()
{
	m_tabs = new QTabWidget(this);
	setCentralWidget(m_tabs);
	MeshView* viewMap = new MeshViewStationsGraph(this);
	viewMap->setDocument(m_document);
	m_tabs->addTab(viewMap, QIcon(":/map.png"), tr("World map"));
	m_tabs->addTab(new MeshLog(m_tabs), QIcon(":/log.png"), tr("Activity log"));
}

QDockWidget* MeshApp::createDock(const QString& title, MeshView* view)
{
	view->setDocument(m_document);
	QDockWidget* dock = new QDockWidget(title, this);
	dock->setWidget(view);
	m_menuView->addAction(dock->toggleViewAction());
//	m_toolbarView->addAction(dock->toggleViewAction());
	return dock;
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
	 m_actSimulationRun->setEnabled(false);
	 m_actSimulationBreak->setEnabled(true);
	 m_actSimulationPause->setEnabled(true);
}

void MeshApp::simulationStopped()
{
	 m_actSimulationRun->setEnabled(true);
	 m_actSimulationBreak->setEnabled(false);
	 m_actSimulationPause->setEnabled(false);
	 m_actSimulationPause->setChecked(false);
}

void MeshApp::simulationEmpty(bool isEmpty)
{
	 m_actSimulationRun->setEnabled(!isEmpty);
}
