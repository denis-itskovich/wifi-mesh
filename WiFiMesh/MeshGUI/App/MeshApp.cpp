#include "MeshApp.h"
#include "../Dialogs/MeshAboutDlg.h"

#include "../DockWidgets/DockRandomizer.h"
#include "../DockWidgets/DockStationProperties.h"

MeshApp::MeshApp(QWidget *parent)
    : QMainWindow(parent)
{
	init();
}

MeshApp::~MeshApp()
{

}

void MeshApp::init()
{
	createActions();
	createMenus();
	createWidgets();
	createToolBars();
	createStatusBar();
	createDocks();
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

	m_actFileClose = new QAction(QIcon(":/close.png"), tr("&Close"), this);
	m_actFileClose->setStatusTip(tr("Close simulation"));

	m_actFileExit =	new QAction(QIcon(":/exit.png"), tr("E&xit"), this);
	m_actFileExit->setStatusTip(tr("Quit WiFi Mesh simulator"));
	connect(m_actFileExit, SIGNAL(triggered()), this, SLOT(close()));

	m_actViewShowBackground = new QAction(tr("Show &background"), this);
	m_actViewShowBackground->setShortcut(tr("Ctrl+1"));
	m_actViewShowBackground->setStatusTip(tr("Show background"));

	m_actViewShowGrid =	new QAction(tr("Show &grid"), this);
	m_actViewShowGrid->setShortcut(tr("Ctrl+2"));
	m_actViewShowGrid->setStatusTip(tr("Show grid"));

	m_actViewShowRules = new QAction(tr("Show &rules"), this);
	m_actViewShowRules->setShortcut(tr("Ctrl+3"));
	m_actViewShowRules->setStatusTip(tr("Show rules"));

	m_actViewShowStations =	new QAction(tr("Show &stations"), this);
	m_actViewShowStations->setShortcut(tr("Ctrl+4"));
	m_actViewShowStations->setStatusTip(tr("Show stations"));

	m_actViewShowDataFlow = new QAction(tr("Show &data flow"), this);
	m_actViewShowDataFlow->setShortcut(tr("Ctrl+5"));
	m_actViewShowDataFlow->setStatusTip(tr("Show data flow"));

	m_actViewShowRouting = new QAction(tr("Show r&outing"), this);
	m_actViewShowRouting->setShortcut(tr("Ctrl+6"));
	m_actViewShowRouting->setStatusTip(tr("Show routing"));

	m_actViewShowCoverage = new QAction(tr("Show &coverage"), this);
	m_actViewShowCoverage->setShortcut(tr("Ctrl+7"));
	m_actViewShowCoverage->setStatusTip(tr("Show coverage"));

	m_actSimulationRun = new QAction(QIcon(":/run.png"), tr("&Run"), this);
	m_actSimulationRun->setShortcut(tr("F5"));
	m_actSimulationRun->setStatusTip(tr("Run simulation"));

	m_actSimulationBreak = new QAction(QIcon(":/break.png"), tr("&Break"), this);
	m_actSimulationBreak->setShortcut(tr("Shift+F5"));
	m_actSimulationBreak->setStatusTip(tr("Break simulation"));

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

	m_menuView->addAction(m_actViewShowBackground);
	m_menuView->addAction(m_actViewShowGrid);
	m_menuView->addAction(m_actViewShowRules);
	m_menuView->addAction(m_actViewShowStations);
	m_menuView->addAction(m_actViewShowDataFlow);
	m_menuView->addAction(m_actViewShowRouting);
	m_menuView->addAction(m_actViewShowCoverage);

	m_menuSimulation->addAction(m_actSimulationRun);
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
	m_toolbarSimulation = addToolBar(tr("Simulation"));

	m_toolbarFile->addAction(m_actFileNew);
	m_toolbarFile->addAction(m_actFileOpen);
	m_toolbarFile->addAction(m_actFileSave);
	m_toolbarFile->addAction(m_actFileClose);

	m_toolbarSimulation->addAction(m_actSimulationRun);
	m_toolbarSimulation->addAction(m_actSimulationBreak);
}

void MeshApp::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

void MeshApp::createDocks()
{
	addFrame(tr("Stations randomizer"), new DockRandomizer(this), Qt::RightDockWidgetArea);
	addFrame(tr("Station properties"), new DockStationProperties(this), Qt::RightDockWidgetArea);
	setCentralWidget(new QMdiArea(this));
}

void MeshApp::addFrame(const QString& title, QFrame* frame, Qt::DockWidgetArea area)
{
	QDockWidget* dock = new QDockWidget(title, this);
	dock->setWidget(frame);
	addDockWidget(area, dock);
}

void MeshApp::about()
{
	MeshAboutDlg* aboutDlg = new MeshAboutDlg();
	aboutDlg->show();
}

void MeshApp::aboutQt()
{
	QMessageBox::aboutQt(this, tr("About Qt"));
}
