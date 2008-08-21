#ifndef MESHAPP_H
#define MESHAPP_H

#include <QtGui>

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

private slots:
	void showAboutDlg();

private:

	QToolBar*	m_toolbarFile;
	QToolBar*	m_toolbarSimulation;
	QToolBar*	m_toolbarSettings;

	QMenu*		m_menuFile;
	QMenu*		m_menuView;
	QMenu*		m_menuSimulation;
	QMenu*		m_menuHelp;

	QAction*	m_actFileNew;
	QAction*	m_actFileOpen;
	QAction*	m_actFileSave;
	QAction*	m_actFileClose;
	QAction*	m_actFileExit;

	QAction*	m_actViewShowBackground;
	QAction*	m_actViewShowGrid;
	QAction*	m_actViewShowRules;
	QAction*	m_actViewShowStations;
	QAction*	m_actViewShowDataFlow;
	QAction*	m_actViewShowRouting;
	QAction*	m_actViewShowCoverage;

	QAction*	m_actSimulationRun;
	QAction*	m_actSimulationBreak;

	QAction*	m_actHelpAbout;

	QSlider*	m_sliderSpeed;
	QSlider*	m_sliderCoverage;
	QSlider*	m_sliderDuration;
};

#endif // MESHAPP_H
