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
	void createWidgets();
	void createToolBars();
	void createStatusBar();
	void createDocks();
	void createTabs();
	void setDocument(MeshDocument* doc);

	QDockWidget* createDock(const QString& title, MeshView* view);

private slots:
	void about();
	void aboutQt();

	void simulationStarted();
	void simulationStopped();
	void simulationEmpty(bool isEmpty);

private:
	QToolBar*	m_toolbarFile;
	QToolBar*	m_toolbarSimulation;
//	QToolBar*	m_toolbarView;

	QMenu*		m_menuFile;
	QMenu*		m_menuView;
	QMenu*		m_menuSimulation;
	QMenu*		m_menuHelp;

	QAction*	m_actFileNew;
	QAction*	m_actFileOpen;
	QAction*	m_actFileSave;
	QAction*	m_actFileClose;
	QAction*	m_actFileExit;

	QAction*	m_actViewShowSettings;
	QAction*	m_actViewShowSniffer;
	QAction*	m_actViewShowStations;
	QAction*	m_actViewShowGenerator;
	QAction*	m_actViewShowLog;

//	QAction*	m_actViewShowBackground;
//	QAction*	m_actViewShowGrid;
//	QAction*	m_actViewShowRules;
//	QAction*	m_actViewShowStations;
//	QAction*	m_actViewShowDataFlow;
//	QAction*	m_actViewShowRouting;
//	QAction*	m_actViewShowCoverage;

	QAction*	m_actSimulationRun;
	QAction*	m_actSimulationPause;
	QAction*	m_actSimulationBreak;

	QAction*	m_actHelpAbout;
	QAction*	m_actHelpAboutQt;

	QTabWidget*	m_tabs;
	QSlider*    m_sliderSpeed;

	MeshDocument*	m_document;
};

#endif // MESHAPP_H
