// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Mesh.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_SIMULATION_SETTINGS, &CMainFrame::OnSimulationSettings)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SIMULATIONTOOLBAR, &CMainFrame::OnUpdateViewSimulationToolbar)
	ON_COMMAND(ID_VIEW_SIMULATIONTOOLBAR, &CMainFrame::OnViewSimulationToolbar)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_simulationBar.Create(this, IDD_SIMULATION_BAR, CBRS_ALIGN_ANY, AFX_IDW_DIALOGBAR);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_TRANSPARENT | TBSTYLE_FLAT, CBRS_SIZE_DYNAMIC | CBRS_ALIGN_ANY) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar) ||
		!m_wndReBar.AddBar(&m_simulationBar))
	{
		TRACE0("Failed to create simulation bar");
		return -1; // fail to create
	}

	m_wndReBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	// EnableDocking(CBRS_ALIGN_ANY);

	return 0;
}

void CMainFrame::OnSimulationSettings()
{
	RecalcLayout();
}

void CMainFrame::OnUpdateViewSimulationToolbar(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_simulationBar.IsWindowVisible());
	pCmdUI->Enable();
}

void CMainFrame::OnViewSimulationToolbar()
{
	m_simulationBar.ShowWindow(m_simulationBar.IsVisible() ? SW_HIDE : SW_SHOW);
	this->RecalcLayout();
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG
