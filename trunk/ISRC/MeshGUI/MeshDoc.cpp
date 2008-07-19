// MeshGUIDoc.cpp : implementation of the CMeshDoc class
//

#include "stdafx.h"
#include "Mesh.h"

#include "MeshDoc.h"
#include "NewMeshDlg.h"
#include "NewStationDlg.h"
#include "MainFrm.h"
#include "MeshView.h"
#include "MeshException.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMeshDoc

IMPLEMENT_DYNCREATE(CMeshDoc, CDocument)

BEGIN_MESSAGE_MAP(CMeshDoc, CDocument)
END_MESSAGE_MAP()


// CMeshDoc construction/destruction

CMeshDoc::CMeshDoc() : 
m_pGrid(NULL),
m_bInitialized(false),
m_bChanged(true)
{
	// TODO: add one-time construction code here
}

CMeshDoc::~CMeshDoc()
{
	GridDestroy(&m_pGrid);
}

bool CMeshDoc::IsValid() const
{
	return GridIsValid(m_pGrid) == eSTATUS_COMMON_OK;
}

BOOL CMeshDoc::OnNewDocument()
{
	if (IsValid()) GridDestroy(&m_pGrid);

	if (!CDocument::OnNewDocument())
		return FALSE;

	if (!m_bInitialized)
	{
		m_bInitialized = true;
		return TRUE;
	}

	CNewMeshDlg dlg;
	INT_PTR res = dlg.DoModal();
	if (res == IDCANCEL) return TRUE;

	CSize size = dlg.GetSize();

	Size _size;
	Size _gridStep;
	_size.x = size.cx;
	_size.y = size.cy;
	_gridStep.x = 1;
	_gridStep.y = 1;

	m_currentId = 0;

	MESH_CHECK_STATUS(GridCreate(&m_pGrid, _size));

	return TRUE;
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

	GridAddStation(m_pGrid, pos, speed, station);
	Refresh();
}

void CMeshDoc::Iterate()
{
	GridIterate(m_pGrid);
	Refresh();
}

void CMeshDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		Size size;
		int items;
		MESH_CHECK_STATUS(GridGetSize(m_pGrid, &size));
		MESH_CHECK_STATUS(GridGetItemsCount(m_pGrid, &items));

		ar << size.x << size.y;
		ar << items;

		GridItem* pItem = NULL;
		GridFirstItem(m_pGrid, &pItem);
		while (pItem)
		{
			Station station;
			Position position;
			Velocity velocity;

			MESH_CHECK_STATUS(GridItemGetStation(pItem, &station));
			MESH_CHECK_STATUS(GridItemGetPosition(pItem, &position));
			MESH_CHECK_STATUS(GridItemGetVelocity(pItem, &velocity));

			ar << station.id;
			ar << position.x << position.y;
			ar << velocity.x << velocity.y;

			GridNextItem(m_pGrid, &pItem);
		}
	}
	else
	{
		Refresh();
		m_currentId = 0;
		// TODO: add loading code here
		Size size;
		Size gridStep;
		unsigned long stationsCount = 0;
		ar >> size.x >> size.y;
		ar >> gridStep.x >> gridStep.y;
		ar >> stationsCount;

		MESH_CHECK_STATUS(GridCreate(&m_pGrid, size));

		for (unsigned i = 0; i < stationsCount; ++i)
		{
			Station station;
			Position position;
			Velocity velocity;

			ar >> station.id;
			ar >> position.x >> position.y;
			ar >> velocity.x >> velocity.y;

			if (station.id > m_currentId) m_currentId = station.id + 1;

			MESH_CHECK_STATUS(GridAddStation(m_pGrid, position, velocity, station));
		}
	}
}

Grid* CMeshDoc::GetGrid()
{
	MESH_CHECK_STATUS(GridIsValid(m_pGrid));
	return m_pGrid;
}

bool CMeshDoc::IsChanged()
{
	bool tmp = m_bChanged;
	m_bChanged = false;
	return tmp;
}

void CMeshDoc::Refresh()
{
	m_bChanged = true;
	POSITION pos = GetFirstViewPosition();
	CView* pView;
	while (pView = GetNextView(pos))
	{
		pView->Invalidate();
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


// CMeshDoc commands
