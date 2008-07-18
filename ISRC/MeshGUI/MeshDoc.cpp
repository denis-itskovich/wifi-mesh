// MeshGUIDoc.cpp : implementation of the CMeshDoc class
//

#include "stdafx.h"
#include "Mesh.h"

#include "MeshDoc.h"
#include "NewMeshDlg.h"
#include "NewStationDlg.h"
#include "MainFrm.h"
#include "MeshView.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMeshDoc

IMPLEMENT_DYNCREATE(CMeshDoc, CDocument)

BEGIN_MESSAGE_MAP(CMeshDoc, CDocument)
	ON_UPDATE_COMMAND_UI(ID_SIMULATION_RUN, OnUpdateSimulationRun)
	ON_UPDATE_COMMAND_UI(ID_SIMULATION_BREAK, OnUpdateSimulationBreak)
	ON_UPDATE_COMMAND_UI(ID_SIMULATION_SETTINGS, OnUpdateSimulationSettings)

	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWBACKGROUND, OnUpdateViewSettings)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWGRID, OnUpdateViewSettings)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWRULES, OnUpdateViewSettings)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWSTATIONS, OnUpdateViewSettings)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWDATAFLOW, OnUpdateViewSettings)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWROUTING, OnUpdateViewSettings)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWCOVERAGE, OnUpdateViewSettings)

	ON_COMMAND(ID_SIMULATION_RUN, OnSimulationRun)
END_MESSAGE_MAP()


// CMeshDoc construction/destruction

CMeshDoc::CMeshDoc() :
m_bStarted(FALSE),
m_bInitialized(FALSE)
{
	// TODO: add one-time construction code here
	memset(&m_grid, 0, sizeof(m_grid));
	m_bStarted = FALSE;
}

CMeshDoc::~CMeshDoc()
{
	GridDestroy(&m_grid);
}

BOOL CMeshDoc::IsGridValid() const
{
	return m_grid.size.x * m_grid.size.y != 0;
}

BOOL CMeshDoc::IsStarted() const
{
	return m_bStarted;
}

BOOL CMeshDoc::OnNewDocument()
{
	GridDestroy(&m_grid);

	if (!CDocument::OnNewDocument())
		return FALSE;

	if (!m_bInitialized)
	{
		m_bInitialized = TRUE;
		return TRUE;
	}

	CNewMeshDlg dlg;
	INT_PTR res = dlg.DoModal();
	if (res == IDCANCEL) return TRUE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	CSize size = dlg.GetSize();

	Size _size;
	Size _gridStep;
	_size.x = size.cx;
	_size.y = size.cy;
	_gridStep.x = 1;
	_gridStep.y = 1;

	GridCreate(&m_grid, _size, _gridStep);
	if ((_size.x) * (_size.y) == 0) return TRUE;

	m_currentId = 0;

	GetMainView()->SetGrid(&m_grid);

	return TRUE;
}

CMeshView* CMeshDoc::GetMainView()
{
	POSITION pos = GetFirstViewPosition();
	return (CMeshView*)GetNextView(pos);
}

void CMeshDoc::AddStation(int x, int y)
{
	CNewStationDlg dlg;
	dlg.m_positionX = x;
	dlg.m_positionY = y;
	if (dlg.DoModal() == IDCANCEL) return;

	Position pos;
	pos.x = dlg.m_positionX;
	pos.y = dlg.m_positionY;
	Velocity speed;
	double angle = dlg.m_angle / 180 * 3.1415269;
	speed.x = dlg.m_speed * cos(angle);
	speed.y = dlg.m_speed * sin(angle);
	
	Station station;
	station.id = m_currentId++;

	GridAddStation(&m_grid, pos, speed, station);
	GetMainView()->Refresh();
	GetMainView()->Invalidate();
}

void CMeshDoc::Iterate()
{
	GridIterate(&m_grid);
	GetMainView()->Refresh();
	GetMainView()->Invalidate();
}

void CMeshDoc::OnUpdateSimulationRun(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsGridValid() && !IsStarted());
}

void CMeshDoc::OnUpdateSimulationBreak(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsGridValid() && IsStarted());
}

void CMeshDoc::OnUpdateSimulationSettings(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsGridValid());
	CSettingsBar* pSettings = GetSettings();
	pCmdUI->SetCheck(pSettings->IsWindowVisible());
}

void CMeshDoc::OnUpdateViewSettings(CCmdUI* pCmdUI)
{
	CSettingsBar* pSettings = GetSettings();
	switch (pCmdUI->m_nID)
	{
	case ID_VIEW_SHOWBACKGROUND: pCmdUI->SetCheck(pSettings->m_showBackground); break;
	case ID_VIEW_SHOWGRID: pCmdUI->SetCheck(pSettings->m_showGrid); break;
	case ID_VIEW_SHOWRULES: pCmdUI->SetCheck(pSettings->m_showRules); break;
	case ID_VIEW_SHOWSTATIONS: pCmdUI->SetCheck(pSettings->m_showStations); break;
	case ID_VIEW_SHOWDATAFLOW: pCmdUI->SetCheck(pSettings->m_showDataFlow); break;
	case ID_VIEW_SHOWROUTING: pCmdUI->SetCheck(pSettings->m_showRouting); break;
	case ID_VIEW_SHOWCOVERAGE: pCmdUI->SetCheck(pSettings->m_showCoverage); break;
	default: break;
	}
	pCmdUI->Enable(IsGridValid());
}

// CMeshDoc serialization

void CMeshDoc::OnSimulationRun()
{
	Iterate();
}

void CMeshDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar << m_grid.size.x << m_grid.size.y;
		ar << m_grid.gridStep.x << m_grid.gridStep.y;
		ar << m_grid.items;
		GridItem* pItem = NULL;
		GridFirstItem(&m_grid, &pItem);
		while (pItem)
		{
			ar << pItem->station.id;
			ar << pItem->position.x << pItem->position.y;
			ar << pItem->velocity.x << pItem->velocity.y;

			GridNextItem(&m_grid, &pItem);
		}
	}
	else
	{
		m_currentId = 0;
		// TODO: add loading code here
		Size size;
		Size gridStep;
		unsigned long stationsCount = 0;
		ar >> size.x >> size.y;
		ar >> gridStep.x >> gridStep.y;
		ar >> stationsCount;

		GridCreate(&m_grid, size, gridStep);
		for (unsigned i = 0; i < stationsCount; ++i)
		{
			Station station;
			Position position;
			Velocity velocity;

			ar >> station.id;
			ar >> position.x >> position.y;
			ar >> velocity.x >> velocity.y;

			if (station.id > m_currentId) m_currentId = station.id + 1;

			GridAddStation(&m_grid, position, velocity, station);
		}

		GetMainView()->SetGrid(&m_grid);
	}
}


// CMeshDoc diagnostics

#ifdef _DEBUG
void CMeshDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMeshDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

Grid* CMeshDoc::GetGrid()
{
	return &m_grid;
}

CSettingsBar* CMeshDoc::GetSettings()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
	return pMainFrame->GetSettings();
}

// CMeshDoc commands
