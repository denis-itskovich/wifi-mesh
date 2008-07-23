// MeshGUIView.cpp : implementation of the CMeshGridView class
//
#include "stdafx.h"
#include "Mesh.h"
#include "Palette.h"

#include "MeshDoc.h"
#include "MeshGridView.h"
#include "MeshException.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Inner class CMeshGridView::Cell
// represents single cell in the grid.

class CMeshGridView::Cell
{
	typedef CList<GridItem*>	StationList;

public:
	Cell() {}

	void		Clear()								{ m_stations.RemoveAll(); m_title.Empty(); }
	bool		IsEmpty() const						{ return m_stations.IsEmpty() != FALSE; }
	CString		GetTitle()							{ return m_title; }
	void		Add(GridItem* pItem);

	GridItem*	GetHead() const						{ return m_stations.IsEmpty() ? NULL : m_stations.GetHead(); }
	int			GetRow() const						{ return m_row; }
	int			GetColumn() const					{ return m_column; }
	void		SetLocation(int row, int column)	{ m_row = row; m_column = column; }

private:
	StationList		m_stations;
	CString			m_title;
	int				m_row;
	int				m_column;
};

void CMeshGridView::Cell::Add(GridItem* pItem)
{
	Station station;
	MESH_CHECK_STATUS(GridItemGetStation(pItem, &station));
	m_title.AppendFormat((IsEmpty() ? _T("%d") : _T(", %d")), station.id);
	m_stations.AddHead(pItem);
}

IMPLEMENT_DYNCREATE(CMeshGridView, CView)

// CMeshGridView message map



BEGIN_MESSAGE_MAP(CMeshGridView, CView)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()

	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWBACKGROUND, &CMeshGridView::OnUpdateViewSubMenus)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWCOVERAGE, &CMeshGridView::OnUpdateViewSubMenus)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWGRID, &CMeshGridView::OnUpdateViewSubMenus)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWRULES, &CMeshGridView::OnUpdateViewSubMenus)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWSTATIONS, &CMeshGridView::OnUpdateViewSubMenus)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWDATAFLOW, &CMeshGridView::OnUpdateViewSubMenus)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWROUTING, &CMeshGridView::OnUpdateViewSubMenus)

	ON_COMMAND(ID_VIEW_SHOWBACKGROUND, &CMeshGridView::OnViewShowBackground)
	ON_COMMAND(ID_VIEW_SHOWCOVERAGE, &CMeshGridView::OnViewShowCoverage)
	ON_COMMAND(ID_VIEW_SHOWGRID, &CMeshGridView::OnViewShowGrid)
	ON_COMMAND(ID_VIEW_SHOWRULES, &CMeshGridView::OnViewShowRules)
	ON_COMMAND(ID_VIEW_SHOWSTATIONS, &CMeshGridView::OnViewShowStations)
	ON_COMMAND(ID_VIEW_SHOWDATAFLOW, &CMeshGridView::OnViewShowDataFlow)
	ON_COMMAND(ID_VIEW_SHOWROUTING, &CMeshGridView::OnViewShowRouting)

    ON_COMMAND(ID_MESHVIEWPOPUP_STATION_ADD, &CMeshGridView::OnPopupStationAdd)
    ON_COMMAND(ID_MESHVIEWPOPUP_STATION_EDIT, &CMeshGridView::OnPopupStationEdit)
    ON_COMMAND(ID_MESHVIEWPOPUP_STATION_REMOVE, &CMeshGridView::OnPopupStationRemove)

    ON_UPDATE_COMMAND_UI(ID_MESHVIEWPOPUP_STATION_ADD, &CMeshGridView::OnUpdatePopupStationAdd)
    ON_UPDATE_COMMAND_UI(ID_MESHVIEWPOPUP_STATION_EDIT, &CMeshGridView::OnUpdatePopupStationEdit)
    ON_UPDATE_COMMAND_UI(ID_MESHVIEWPOPUP_STATION_REMOVE, &CMeshGridView::OnUpdatePopupStationEdit)
    ON_WM_SIZE()
    ON_WM_TIMER()
    ON_UPDATE_COMMAND_UI(ID_SIMULATION_RUN, &CMeshGridView::OnUpdateSimulationRun)
    ON_UPDATE_COMMAND_UI(ID_SIMULATION_BREAK, &CMeshGridView::OnUpdateSimulationBreak)
    ON_COMMAND(ID_SIMULATION_RUN, &CMeshGridView::OnSimulationRun)
    ON_COMMAND(ID_SIMULATION_BREAK, &CMeshGridView::OnSimulationBreak)
END_MESSAGE_MAP()

// CMeshGridView construction/destruction

CMeshGridView::CMeshGridView() :
m_pCells(NULL),
m_pCurrentCell(NULL),
m_rows(0),
m_columns(0),
m_range(0),
m_nStep(0),
m_bIsRunning(0)
{
	m_wifiBitmap.LoadBitmap(IDB_WIFI_STATION);
}

CMeshGridView::~CMeshGridView()
{
	if (m_pCells) delete[] m_pCells;
	if (m_primaryCoverage.GetSafeHandle()) m_primaryCoverage.DeleteObject();
	if (m_secondaryCoverage.GetSafeHandle()) m_secondaryCoverage.DeleteObject();
}

// CMeshGridView drawing

void CMeshGridView::OnDraw(CDC* pDC)
{
	CMeshDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc || !pDoc->IsValid()) return;

	if (pDoc->IsChanged()) Refresh();

	CMeshSettings& settings = GetSettings();
	if (settings.IsVisible(eVI_BACKGROUND)) DrawBackground(pDC);

	Grid* pGrid = GetGrid();
	if (!pGrid) return;

	if (settings.IsVisible(eVI_COVERAGE)) DrawCoverage(pDC);
	if (settings.IsVisible(eVI_RULES)) DrawRules(pDC);
	if (settings.IsVisible(eVI_GRID)) DrawGrid(pDC);
	if (settings.IsVisible(eVI_STATIONS)) DrawStations(pDC);
}

void CMeshGridView::DrawGrid(CDC* pDC)
{
	CRect rect = GetGridRect();
	CSize size = rect.Size();
	double stepX = GetHorizontalStep();
	double stepY = GetVerticalStep();

    LOGBRUSH logBrush;
    logBrush.lbStyle = BS_SOLID;
    logBrush.lbColor = COLOR_GRID;

	CPen pen;
    pen.CreatePen(PS_DOT | PS_GEOMETRIC, 1, &logBrush);
	CPen* pOldPen = pDC->SelectObject(&pen);

	CRect rcClip;
	pDC->GetClipBox(rcClip);

	for (double i = rect.left; ((int)i < size.cx) && ((int)i <= rcClip.right); i+=stepX)
	{
		if ((int)i < rcClip.left) continue;
		pDC->MoveTo((int)i, (max(rect.top, rcClip.top) + 1) & ~1);
		pDC->LineTo((int)i, min(rect.top + size.cy, rcClip.bottom));
	}

	for (double i = rect.top; ((int)i < size.cy) && ((int)i <= rcClip.bottom); i+=stepY)
	{
		if ((int)i < rcClip.top) continue;
		pDC->MoveTo((max(rect.left, rcClip.left) + 1) & ~1, (int)i);
		pDC->LineTo(min(rect.left + size.cx, rcClip.right), (int)i);
	}

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}

void CMeshGridView::RefreshCoverage()
{
    CRgn oldPrimary;
    CRgn oldSecondary;

    oldPrimary.CreateRectRgn(0,0,0,0);
    oldSecondary.CreateRectRgn(0,0,0,0);

    if (m_primaryCoverage.GetSafeHandle()) oldPrimary.CopyRgn(&m_primaryCoverage);
    if (m_secondaryCoverage.GetSafeHandle()) oldSecondary.CopyRgn(&m_secondaryCoverage);

    double radius = GetSettings().GetCoverageRange();

    CalcCoverage(m_primaryCoverage, radius / 2);
    CalcCoverage(m_secondaryCoverage, radius);

    oldPrimary.CombineRgn(&oldPrimary, &m_primaryCoverage, RGN_XOR);
    oldSecondary.CombineRgn(&oldSecondary, &m_secondaryCoverage, RGN_XOR);
    oldPrimary.CombineRgn(&oldPrimary, &oldSecondary, RGN_OR);

    InvalidateRgn(&oldPrimary);

    oldPrimary.DeleteObject();
    oldSecondary.DeleteObject();
}

void CMeshGridView::DrawCoverage(CDC* pDC)
{
	int items;
	MESH_CHECK_STATUS(GridGetItemsCount(GetGrid(), &items));


	CRect rcClip;
	pDC->GetClipBox(rcClip);

	CRect gridRect = GetGridRect();

	rcClip.InflateRect(1,1);

	CBrush primaryBrush(COLOR_COVERAGE_PRIMARY_FILL);
	CBrush secondaryBrush(COLOR_COVERAGE_SECONDARY_FILL);
	CBrush strokeBrush(COLOR_COVERAGE_STROKE);

	CRgn rgnClip;
    rgnClip.CreateRectRgn(rcClip.left, rcClip.top, rcClip.right, rcClip.bottom);

	CRgn rgnPrimary;
	rgnPrimary.CreateRectRgn(0,0,0,0);
	rgnPrimary.CombineRgn(&rgnClip, &m_primaryCoverage, RGN_AND);

	CRgn rgnSecondary;
	rgnSecondary.CreateRectRgn(0,0,0,0);
	rgnSecondary.CombineRgn(&rgnClip, &m_secondaryCoverage, RGN_AND);

	CDC dcMem;
	CBitmap bmp;

	dcMem.CreateCompatibleDC(pDC);
	bmp.CreateBitmap(rcClip.Width(), rcClip.Height(), 1, 32, NULL);

	dcMem.SelectObject(&bmp);
	dcMem.BitBlt(0, 0, rcClip.Width(), rcClip.Height(), pDC, rcClip.left, rcClip.top, SRCCOPY);
	dcMem.SetViewportOrg(-rcClip.left, -rcClip.top);

	rgnSecondary.CombineRgn(&rgnSecondary, &rgnPrimary, RGN_DIFF);

	dcMem.FillRgn(&rgnSecondary, &secondaryBrush);
	dcMem.FillRgn(&rgnPrimary, &primaryBrush);

    BLENDFUNCTION blend = { AC_SRC_OVER, 0, OPACITY_COVERAGE, 0 };
	pDC->AlphaBlend(rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &dcMem, rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), blend);

	dcMem.DeleteDC();
	bmp.DeleteObject();

	rgnPrimary.DeleteObject();
	rgnSecondary.DeleteObject();
	rgnClip.DeleteObject();
	secondaryBrush.DeleteObject();
	primaryBrush.DeleteObject();
}

void CMeshGridView::DrawStations(CDC* pDC)
{
	CMeshSettings& settings = GetSettings();

	CFont font; font.CreatePointFont(100, FONT_STATION);
	CFont* pOldFont = pDC->SelectObject(&font);

	for (int i = 0; i < GetCellsCount(); ++i)
	{
		Cell& cell = m_pCells[i];

		if (settings.IsVisible(eVI_STATIONS) && !cell.IsEmpty()) DrawCell(pDC, cell);
	}

	if (m_pCurrentCell) DrawCell(pDC, *m_pCurrentCell, TRUE);

	pDC->SelectObject(pOldFont);
	font.DeleteObject();
}

void CMeshGridView::DrawCell(CDC* pDC, Cell& cell, BOOL bSelected)
{
	CRect rect = GetCellRect(cell);
	CRect rcClip;
	pDC->GetClipBox(rcClip);

	if (!rcClip.IntersectRect(rcClip, rect)) return;

	if (bSelected)
	{
		CDC dcSel;
		dcSel.CreateCompatibleDC(pDC);
		CBitmap bmp;
		bmp.CreateBitmap(rcClip.Width(), rcClip.Height(), 1, 32, NULL);
		dcSel.SelectObject(&bmp);
		dcSel.SetViewportOrg(-rcClip.left, -rcClip.top);

		CBrush brush(COLOR_CELL_CELLECTION);
		dcSel.FillRect(rcClip, &brush);
		BLENDFUNCTION blend = {AC_SRC_OVER, 0, 128, 0};
		pDC->AlphaBlend(rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &dcSel, rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), blend);

		bmp.DeleteObject();
		brush.DeleteObject();
		dcSel.DeleteDC();
	}

	if (cell.IsEmpty()) return;

	CDC bmpDC;
	bmpDC.CreateCompatibleDC(pDC);
	bmpDC.SelectObject(&m_wifiBitmap);
	COLORREF background = bmpDC.GetPixel(0,0);

    BITMAP bmpWiFi;
    m_wifiBitmap.GetBitmap(&bmpWiFi);
	pDC->TransparentBlt(rect.left + 2, rect.top + 2, rect.Width() - 4, rect.Height() - 4, &bmpDC, 0, 0, bmpWiFi.bmWidth, bmpWiFi.bmHeight, background);
	
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(cell.GetTitle(), CRect(rect), DT_CENTER | DT_BOTTOM | DT_SINGLELINE | DT_WORD_ELLIPSIS);

	bmpDC.DeleteDC();
}

void CMeshGridView::CalcCoverage(CRgn& rgn, double radius) const
{
	if (rgn.GetSafeHandle()) rgn.DeleteObject();
	rgn.CreateRectRgn(0,0,0,0);

	for (int i = 0; i < GetCellsCount(); ++i)
	{
		Cell& cell = m_pCells[i];
		if (!cell.IsEmpty()) CalcCoverage(rgn, cell, radius);
	}
    
    CRgn rgnClip;
    CRect rcGrid = GetGridRect();
    rcGrid.InflateRect(1,1,1,1);

    rgnClip.CreateRectRgn(rcGrid.left, rcGrid.top, rcGrid.right, rcGrid.bottom);
    rgn.CombineRgn(&rgn, &rgnClip, RGN_AND);

    rgnClip.DeleteObject();
}

void CMeshGridView::CalcCoverage(CRgn& rgn, const Cell& cell, double radius) const
{
	int dx = (int)(GetHorizontalStep() * radius);
	int dy = (int)(GetVerticalStep() * radius);

	CRect dstRect = GetCellRect(cell);
	dstRect = CRect(dstRect.CenterPoint(), dstRect.CenterPoint());
	dstRect.InflateRect(CRect(dx + 1, dy + 1, dx + 2, dy + 2));

	CRgn newRgn; newRgn.CreateEllipticRgn(dstRect.left, dstRect.top, dstRect.right, dstRect.bottom);
	rgn.CombineRgn(&rgn, &newRgn, RGN_OR);
	newRgn.DeleteObject();
}

void CMeshGridView::DrawBackground(CDC* pDC)
{
	CRect rect;
	pDC->GetClipBox(rect);

	CBrush bkBrush(COLOR_BACKGROUND_FILL);
	pDC->FillRect(rect, &bkBrush);
	bkBrush.DeleteObject();

	rect = GetGridRect();
	double angle = atan((double)rect.Height() / (double)rect.Width());
	int nEscapement = (int)(angle / 3.1415269 * 1800);

	CFont font;
	int fontWidth = 16, fontHeight = 20;

	CString text = TEXT_BACKGROUND;
	
	font.CreateFont(fontHeight, fontWidth, nEscapement, 0,
					FW_BOLD, FALSE, FALSE, FALSE,
					ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
					FF_DONTCARE, FONT_BACKGROUND);

	int dcOld = pDC->SaveDC();
	pDC->SetTextColor(COLOR_BACKGROUND_TEXT);
	pDC->SelectObject(&font);

	CRect newRect = rect;
	pDC->DrawText(text, newRect, DT_CALCRECT | DT_SINGLELINE);
	font.DeleteObject();

	fontWidth = fontWidth * rect.Width() / newRect.Width();
	fontHeight = fontHeight * rect.Height() / newRect.Height();
	font.CreateFont(fontHeight, fontWidth, nEscapement, 0,
					FW_BOLD, FALSE, FALSE, FALSE,
					ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
					FF_DONTCARE, FONT_BACKGROUND);

	pDC->SelectObject(&font);
	pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);

	CPoint textPos = rect.CenterPoint();
	textPos.x += (int)(sin(angle) * (double)fontHeight / 2);
	textPos.y += (int)(cos(angle) * (double)fontHeight / 2);

	pDC->SetBkMode(TRANSPARENT);
	pDC->BeginPath();
	pDC->TextOut(textPos.x, textPos.y, text);
	pDC->EndPath();
	// pDC->SelectClipPath(RGN_AND);

	CBrush brush;
	CPen pen(PS_SOLID, 1, COLOR_BACKGROUND_STROKE);
	brush.CreateSolidBrush(COLOR_BACKGROUND_TEXT);
	pDC->SelectObject(&brush);
	pDC->SelectObject(&pen);
	pDC->StrokeAndFillPath();
	// pDC->FillPath();

	pDC->RestoreDC(dcOld);

	font.DeleteObject();
	brush.DeleteObject();
	pen.DeleteObject();
}

void CMeshGridView::DrawRules(CDC* pDC)
{
	if (!m_pCells) return;

	CRect rcHoriz = GetHorizontalRuleRect();
	CRect rcVert = GetVerticalRuleRect();

	CRect rcClip;
	pDC->GetClipBox(rcClip);

	CBrush brush(COLOR_RULES);
	CFont font; font.CreatePointFont(80, FONT_RULES);
	CFont* pOldFont = pDC->SelectObject(&font);

	pDC->FillRect(rcVert & rcClip, &brush);
	pDC->FillRect(rcHoriz & rcClip, &brush);

	brush.DeleteObject();

	pDC->SetBkMode(TRANSPARENT);

	for (int i = 0; i < m_columns; ++i)
	{
		CRect rect = GetCellRect(GetCell(0, i));
		rect.top = rcHoriz.top;
		rect.bottom = rcHoriz.bottom - 1;

		CRect tmp;
		tmp.IntersectRect(rect, rcClip);
		if (tmp.IsRectEmpty()) continue;

		CString num; num.Format(_T("%d"), i);

		pDC->MoveTo(rect.left - 1, rcHoriz.Height() / 2);
		pDC->LineTo(rect.left - 1, rcHoriz.Height());

		pDC->DrawText(num, rect, DT_CENTER | DT_BOTTOM | DT_SINGLELINE);
	}

	for (int i = 0; i < m_rows; ++i)
	{
		CRect rect = GetCellRect(GetCell(i, 0));
		rect.left = rcVert.left + 1;
		rect.right = rcVert.right - 1;

		CRect tmp;
		tmp.IntersectRect(rect, rcClip);
		if (tmp.IsRectEmpty()) continue;

		CString num; num.Format(_T("%d"), i);

		pDC->MoveTo(rcVert.Width() / 2, rect.top - 1);
		pDC->LineTo(rcVert.Width(), rect.top - 1);

		pDC->DrawText(num, rect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	}

	pDC->SelectObject(pOldFont);
	font.DeleteObject();
}

CRect CMeshGridView::GetGridRect() const
{
	CRect rect;
	GetClientRect(&rect);

	CMeshDoc* pDoc = (CMeshDoc*)GetDocument();
	if (!GetSettings().IsVisible(eVI_RULES)) return rect;

	rect.SubtractRect(rect, GetHorizontalRuleRect());
	rect.SubtractRect(rect, GetVerticalRuleRect());
	return rect;
}

CRect CMeshGridView::GetHorizontalRuleRect() const
{
	CRect rect;
	GetClientRect(rect);
	rect.bottom = rect.top + 16;
	return rect;
}

CRect CMeshGridView::GetVerticalRuleRect() const
{
	CRect rect;
	GetClientRect(rect);
	rect.right = rect.left + 16;
	return rect;
}

double CMeshGridView::GetHorizontalStep() const
{
	CRect rect = GetGridRect();
	CSize size = rect.Size();
	return (double)size.cx / (double)m_columns;
}

double CMeshGridView::GetVerticalStep() const
{
	CRect rect = GetGridRect();
	CSize size = rect.Size();
	return (double)size.cy / (double)m_rows;
}

void CMeshGridView::GetCellRgn(const Cell &cell, CRgn &rgn, bool bEmpty) const
{
    CMeshSettings& settings = GetSettings();
    rgn.CreateRectRgnIndirect(GetCellRect(cell));

    if (!bEmpty && settings.IsVisible(eVI_COVERAGE)) CalcCoverage(rgn, cell, settings.GetCoverageRange());
}

CRect CMeshGridView::GetCellRect(const Cell& cell) const
{
	CRect rect = GetGridRect();
	double dx = GetHorizontalStep();
	double dy = GetVerticalStep();
	int row = cell.GetRow();
	int column = cell.GetColumn();

	rect.right = rect.left + (int)(dx * (double)(column + 1));
	rect.left += (int)(dx * (double)column) + 1;
	rect.bottom = rect.top + (int)(dy * (double)(row + 1));
	rect.top += (int)(dy * (double)row) + 1;

	return rect;
}

CMeshGridView::Cell& CMeshGridView::GetCell(int row, int column)
{
	return m_pCells[row * m_columns + column];
}

void CMeshGridView::Refresh()
{
    if (!GetDocument()->IsValid()) return;

	Size size = {0, 0};
	Grid* pGrid = GetGrid();
	if (pGrid) MESH_CHECK_STATUS(GridGetSize(pGrid, &size));

	if (m_columns != size.x || m_rows != size.y)
	{
		m_rows = size.y;
		m_columns = size.x;
		if (m_pCells)
		{
			delete[] m_pCells;
			m_pCells = NULL;
		}

		int cellsCount = GetCellsCount();
		if (cellsCount) m_pCells = new Cell[cellsCount];
		for (int i = 0; i < cellsCount; ++i) m_pCells[i].SetLocation(i / m_columns, i % m_columns);
        Invalidate();
        RefreshCoverage();
        return;
	}

	if (!m_pCells) return;
    bool bRecalcCoverage = GetSettings().GetCoverageRange() != m_range;

	for (int i = 0; i < GetCellsCount(); ++i)
	{
        Cell& curCell = m_pCells[i];
        Position pos;
        pos.x = curCell.GetColumn();
        pos.y = curCell.GetRow();
        GridItem* pItem = NULL;
        MESH_CHECK_STATUS(GridGetItemAt(pGrid, pos, &pItem));
        
        if ((curCell.IsEmpty() && (pItem != NULL)) || 
            (!curCell.IsEmpty() && (pItem == NULL)))
        {
            bRecalcCoverage = true;
        }
        curCell.Clear();
    }

	GridItem* pItem = NULL;
	GridFirstItem(pGrid, &pItem);

	while (pItem)
	{
		Position pos;
		MESH_CHECK_STATUS(GridItemGetPosition(pItem, &pos));

		int x = (int)(pos.x + 0.5);
		int y = (int)(pos.y + 0.5);

		Cell& curCell = GetCell(y, x);
		curCell.Add(pItem);
		GridNextItem(pGrid, &pItem);
	}

    if (bRecalcCoverage && GetSettings().IsVisible(eVI_COVERAGE)) RefreshCoverage();
}

CMeshGridView::Cell* CMeshGridView::GetCellByLocation(CPoint point)
{
	if (!m_pCells) return NULL;
	CRect gridRect = GetGridRect();
	if (!gridRect.PtInRect(point)) return NULL;

	int column = (int)((double)(point.x - gridRect.left) / GetHorizontalStep());
	int row = (int)((double)(point.y - gridRect.top) / GetVerticalStep());
	return &GetCell(row, column);
}

// CMeshGridView message handlers

void CMeshGridView::OnMouseMove(UINT nFlags, CPoint point)
{
	Cell* pNewCurrCell = GetCellByLocation(point);
	if (pNewCurrCell == m_pCurrentCell) return;

    if ((nFlags & MK_LBUTTON) && m_pCurrentCell && pNewCurrCell && !m_pCurrentCell->IsEmpty())
    {
        GetDocument()->MoveStation(m_pCurrentCell->GetColumn(), m_pCurrentCell->GetRow(),
                                   pNewCurrCell->GetColumn(), pNewCurrCell->GetRow());
    }

    if (m_pCurrentCell) InvalidateRect(GetCellRect(*m_pCurrentCell));
    if (pNewCurrCell) InvalidateRect(GetCellRect(*pNewCurrCell));

	m_pCurrentCell = pNewCurrCell;
}

void CMeshGridView::OnMouseLeave()
{
	if (!m_pCurrentCell) return;
	InvalidateRect(GetCellRect(*m_pCurrentCell));
	m_pCurrentCell = NULL;
}

void CMeshGridView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if (!m_pCurrentCell) return;
	GetDocument()->AddStation(m_pCurrentCell->GetColumn(), m_pCurrentCell->GetRow());
}

void CMeshGridView::OnRButtonUp(UINT nFlags, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_MENU1);

	ClientToScreen(&point);
	CMenu* pPopup = menu.GetSubMenu(0);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());

	CView::OnRButtonUp(nFlags, point);
}

BOOL CMeshGridView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (!m_pCurrentCell || m_pCurrentCell->IsEmpty()) return CView::OnSetCursor(pWnd, nHitTest, message);
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
	return TRUE;
}

BOOL CMeshGridView::OnEraseBkgnd(CDC* pDC)
{
	return CView::OnEraseBkgnd(pDC);
}

void CMeshGridView::OnPopupStationAdd()
{
    GetDocument()->AddStation(m_pCurrentCell->GetColumn(), m_pCurrentCell->GetRow());
}

void CMeshGridView::OnUpdateViewSubMenus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetDocument()->IsValid());
	pCmdUI->SetCheck(GetSettings().IsVisible((EVisualItems)(eVI_BACKGROUND + pCmdUI->m_nID - ID_VIEW_SHOWBACKGROUND)));
}

void CMeshGridView::OnViewShowBackground()
{
	GetSettings().Toggle(eVI_BACKGROUND);
	Invalidate();
}

void CMeshGridView::OnViewShowGrid()
{
	GetSettings().Toggle(eVI_GRID);
	Invalidate();
}

void CMeshGridView::OnViewShowRules()
{
	GetSettings().Toggle(eVI_RULES);
	Invalidate();
}

void CMeshGridView::OnViewShowStations()
{
	GetSettings().Toggle(eVI_STATIONS);
	Invalidate();
}

void CMeshGridView::OnViewShowDataFlow()
{
	GetSettings().Toggle(eVI_DATA_FLOW);
	Invalidate();
}

void CMeshGridView::OnViewShowCoverage()
{
	if (!GetSettings().Toggle(eVI_COVERAGE)) Refresh();
	Invalidate();
}

void CMeshGridView::OnViewShowRouting()
{
	GetSettings().Toggle(eVI_ROUTING);
	Invalidate();
}


void CMeshGridView::OnPopupStationEdit()
{
    GetDocument()->EditStation(m_pCurrentCell->GetColumn(), m_pCurrentCell->GetRow());
}

void CMeshGridView::OnPopupStationRemove()
{
    GetDocument()->RemoveStation(m_pCurrentCell->GetColumn(), m_pCurrentCell->GetRow());
}


void CMeshGridView::OnUpdatePopupStationEdit(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_pCurrentCell != NULL && !m_pCurrentCell->IsEmpty());
}

void CMeshGridView::OnUpdatePopupStationAdd(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_pCurrentCell != NULL);
}

void CMeshGridView::OnUpdateSimulationRun(CCmdUI *pCmdUI)
{
    // TODO: Add your command update UI handler code here
    pCmdUI->Enable(GetDocument()->IsValid() && !m_bIsRunning);
}

void CMeshGridView::OnUpdateSimulationBreak(CCmdUI *pCmdUI)
{
    // TODO: Add your command update UI handler code here
    pCmdUI->Enable(GetDocument()->IsValid() && m_bIsRunning);
}

void CMeshGridView::OnSize(UINT nType, int cx, int cy)
{
    CMeshView::OnSize(nType, cx, cy);
    RefreshCoverage();
    // TODO: Add your message handler code here
}

void CMeshGridView::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    if (nIDEvent == 1)
    {
        if (++m_nStep >= GetSettings().GetDuration()) OnSimulationBreak();
        else
        {
            GetDocument()->Iterate();
        }
    }
    CMeshView::OnTimer(nIDEvent);
}



// CMeshGridView diagnostics

#ifdef _DEBUG
void CMeshGridView::AssertValid() const
{
	CView::AssertValid();
}

void CMeshGridView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif //_DEBUG

void CMeshGridView::OnSimulationRun()
{
    // TODO: Add your command handler code here
    m_bIsRunning = true;
    m_nStep = 0;
    SetTimer(1, 20, NULL);
}

void CMeshGridView::OnSimulationBreak()
{
    // TODO: Add your command handler code here
    m_bIsRunning = false;
    KillTimer(1);
}
