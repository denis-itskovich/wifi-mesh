// MeshGUIView.cpp : implementation of the CMeshView class
//
#include "stdafx.h"
#include "Mesh.h"
#include "Palette.h"

#include "MeshDoc.h"
#include "MeshView.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Inner class CMeshView::Cell
// represents single cell in the grid.

class CMeshView::Cell
{
	typedef CList<GridItem*>	StationList;

public:
	Cell() {}

	void		Clear()								{ m_stations.RemoveAll(); m_title.Empty(); }
	bool		IsEmpty() const						{ return m_stations.IsEmpty() != FALSE; }
	CString		GetTitle()							{ return m_title; }
	void		Add(GridItem* pItem)				{ m_title.AppendFormat((IsEmpty() ? L"%d" : L", %d"), pItem->station.id); m_stations.AddHead(pItem); }

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


IMPLEMENT_DYNCREATE(CMeshView, CView)

// CMeshView message map

BEGIN_MESSAGE_MAP(CMeshView, CView)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CMeshView construction/destruction

CMeshView::CMeshView() :
m_pGrid(NULL),
m_pCells(NULL),
m_pCurrentCell(NULL)
{
	m_wifiBitmap.LoadBitmap(IDB_WIFI_STATION);
}

CMeshView::~CMeshView()
{
	if (m_pCells) delete[] m_pCells;
	if (m_primaryCoverage.GetSafeHandle()) m_primaryCoverage.DeleteObject();
	if (m_secondaryCoverage.GetSafeHandle()) m_secondaryCoverage.DeleteObject();
}

// CMeshView drawing

void CMeshView::OnDraw(CDC* pDC)
{
	CMeshDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	CSettingsBar* pSettings = pDoc->GetSettings();
	if (pSettings->m_showBackground) DrawBackground(pDC);

	if (!m_pGrid || !(m_pGrid->size.x * m_pGrid->size.y)) return;

	if (pSettings->m_showCoverage) DrawCoverage(pDC);
	if (pSettings->m_showRules) DrawRules(pDC);
	if (pSettings->m_showGrid) DrawGrid(pDC);
	if (pSettings->m_showStations) DrawStations(pDC);
}

void CMeshView::DrawGrid(CDC* pDC)
{
	CRect rect = GetGridRect();
	CSize size = rect.Size();
	double stepX = GetHorizontalStep();
	double stepY = GetVerticalStep();

	CPen pen(PS_DOT, 1, COLOR_GRID);
	CPen* pOldPen = pDC->SelectObject(&pen);

	CRect rcClip;
	pDC->GetClipBox(rcClip);

	for (double i = rect.left; (i < size.cx) && (i < rcClip.right); i+=stepX)
	{
		if (i < rcClip.left) continue;
		pDC->MoveTo((int)i, max(rect.top, rcClip.top));
		pDC->LineTo((int)i, min(rect.top + size.cy, rcClip.bottom));
	}

	for (double i = rect.top; (i < size.cy) && (i < rcClip.bottom); i+=stepY)
	{
		if (i < rcClip.top) continue;
		pDC->MoveTo(max(rect.left, rcClip.left), (int)i);
		pDC->LineTo(max(rect.left + size.cx, rcClip.right), (int)i);
	}

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}

void CMeshView::DrawCoverage(CDC* pDC)
{
	if (!m_pGrid->items) return;

	double radius = GetDocument()->GetSettings()->m_coverage;

	CRect rcClip;
	pDC->GetClipBox(rcClip);

	CRect gridRect = GetGridRect();

	if (rcClip.Width() >= gridRect.Width() || rcClip.Height() >= gridRect.Height())
	{
		CalcCoverage(m_primaryCoverage, radius / 4);
		CalcCoverage(m_secondaryCoverage, radius / 2);
	}

	rcClip.InflateRect(1,1);

	CBrush primaryBrush(COLOR_COVERAGE_PRIMARY_FILL);
	CBrush secondaryBrush(COLOR_COVERAGE_SECONDARY_FILL);

	CRgn rgnClip; rgnClip.CreateRectRgn(rcClip.left, rcClip.top, rcClip.right, rcClip.bottom);

	CRgn rgnPrimary;
	rgnPrimary.CreateRectRgn(0,0,0,0);
	rgnPrimary.CombineRgn(&rgnClip, &m_primaryCoverage, RGN_AND);

	CRgn rgnSecondary;
	rgnSecondary.CreateRectRgn(0,0,0,0);
	rgnSecondary.CombineRgn(&rgnClip, &m_secondaryCoverage, RGN_AND);
	rgnSecondary.CombineRgn(&rgnSecondary, &rgnPrimary, RGN_DIFF);

	CDC dcMem;
	CBitmap bmp;

	dcMem.CreateCompatibleDC(pDC);
	bmp.CreateBitmap(rcClip.Width(), rcClip.Height(), 1, 32, NULL);

	dcMem.SelectObject(&bmp);
	dcMem.BitBlt(0, 0, rcClip.Width(), rcClip.Height(), pDC, rcClip.left, rcClip.top, SRCCOPY);
	dcMem.SetViewportOrg(-rcClip.left, -rcClip.top);

	dcMem.FillRgn(&rgnSecondary, &secondaryBrush);
	dcMem.FillRgn(&rgnPrimary, &primaryBrush);

	BLENDFUNCTION blend = { AC_SRC_OVER, 0, 128, 0 };
	pDC->AlphaBlend(rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &dcMem, rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), blend);

	dcMem.DeleteDC();
	bmp.DeleteObject();

	rgnPrimary.DeleteObject();
	rgnSecondary.DeleteObject();
	rgnClip.DeleteObject();
	secondaryBrush.DeleteObject();
	primaryBrush.DeleteObject();
}

void CMeshView::DrawStations(CDC* pDC)
{
	CSettingsBar* pSettings = GetDocument()->GetSettings();

	CFont font; font.CreatePointFont(100, FONT_STATION);
	CFont* pOldFont = pDC->SelectObject(&font);

	for (int i = 0; i < m_cellsCount; ++i)
	{
		Cell& cell = m_pCells[i];

		if (pSettings->m_showStations && !cell.IsEmpty()) DrawCell(pDC, cell);
	}

	if (m_pCurrentCell) DrawCell(pDC, *m_pCurrentCell, TRUE);

	pDC->SelectObject(pOldFont);
	font.DeleteObject();
}

void CMeshView::DrawCell(CDC* pDC, Cell& cell, BOOL bSelected)
{
	CRect rect = GetCellRect(cell);
	CRect rcClip;
	pDC->GetClipBox(rcClip);

	if (!rcClip.IntersectRect(rcClip, rect)) return;

	if (bSelected)
	{
		CBrush brush(COLOR_CELL_CELLECTION);
		pDC->FillRect(rect, &brush);
		brush.DeleteObject();
	}

	if (cell.IsEmpty()) return;

	CDC bmpDC;
	bmpDC.CreateCompatibleDC(pDC);
	bmpDC.SelectObject(&m_wifiBitmap);
	COLORREF background = bmpDC.GetPixel(0,0);

	pDC->TransparentBlt(rect.left + 2, rect.top + 2, rect.Width() - 4, rect.Height() - 4, &bmpDC, 0, 0, 128, 128, background);
	
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(cell.GetTitle(), CRect(rect), DT_CENTER | DT_BOTTOM | DT_SINGLELINE | DT_WORD_ELLIPSIS);

	bmpDC.DeleteDC();
}

void CMeshView::CalcCoverage(CRgn& rgn, double radius)
{
	if (rgn.GetSafeHandle()) rgn.DeleteObject();
	rgn.CreateRectRgn(0,0,0,0);

	for (int i = 0; i < m_cellsCount; ++i)
	{
		Cell& cell = m_pCells[i];
		if (!cell.IsEmpty()) CalcCoverage(rgn, cell, radius);
	}
}

void CMeshView::CalcCoverage(CRgn& rgn, Cell& cell, double radius)
{
	int dx = (int)(GetHorizontalStep() * radius);
	int dy = (int)(GetVerticalStep() * radius);

	CRect dstRect = GetCellRect(cell);
	dstRect.InflateRect(CRect(dx, dy, dx, dy));

	CRgn newRgn; newRgn.CreateEllipticRgn(dstRect.left, dstRect.top, dstRect.right, dstRect.bottom);
	rgn.CombineRgn(&rgn, &newRgn, RGN_OR);
	newRgn.DeleteObject();
}

void CMeshView::DrawBackground(CDC* pDC)
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
	CFont* pOldFont = pDC->SelectObject(&font);

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
	pDC->TextOut(textPos.x, textPos.y, text);
	pDC->SelectObject(pOldFont);
	font.DeleteObject();

	pDC->RestoreDC(dcOld);
}

void CMeshView::DrawRules(CDC* pDC)
{
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

	for (unsigned i = 0; i < m_pGrid->size.x; ++i)
	{
		CRect rect = GetCellRect(GetCell(0, i));
		rect.top = rcHoriz.top + 1;
		rect.bottom = rcHoriz.bottom - 1;

		CRect tmp;
		tmp.IntersectRect(rect, rcClip);
		if (tmp.IsRectEmpty()) continue;

		CString num; num.Format(L"%d", i);

		pDC->MoveTo(rect.left, rcHoriz.Height() / 2);
		pDC->LineTo(rect.left, rcHoriz.Height());

		pDC->DrawText(num, rect, DT_CENTER | DT_BOTTOM | DT_SINGLELINE);
	}

	for (unsigned i = 0; i < m_pGrid->size.y; ++i)
	{
		CRect rect = GetCellRect(GetCell(i, 0));
		rect.left = rcVert.left + 1;
		rect.right = rcVert.right - 1;

		CRect tmp;
		tmp.IntersectRect(rect, rcClip);
		if (tmp.IsRectEmpty()) continue;

		CString num; num.Format(L"%d", i);

		pDC->MoveTo(rcVert.Width() / 2, rect.top);
		pDC->LineTo(rcVert.Width(), rect.top);

		pDC->DrawText(num, rect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	}

	pDC->SelectObject(pOldFont);
	font.DeleteObject();
}

CRect CMeshView::GetGridRect() const
{
	CRect rect;
	GetClientRect(&rect);

	CMeshDoc* pDoc = (CMeshDoc*)GetDocument();
	if (!pDoc->GetSettings()->m_showRules) return rect;

	rect.SubtractRect(rect, GetHorizontalRuleRect());
	rect.SubtractRect(rect, GetVerticalRuleRect());
	return rect;
}

CRect CMeshView::GetHorizontalRuleRect() const
{
	CRect rect;
	GetClientRect(rect);
	rect.bottom = rect.top + 16;
	return rect;
}

CRect CMeshView::GetVerticalRuleRect() const
{
	CRect rect;
	GetClientRect(rect);
	rect.right = rect.left + 16;
	return rect;
}

double CMeshView::GetHorizontalStep() const
{
	CRect rect = GetGridRect();
	CSize size = rect.Size();
	return (double)m_pGrid->gridStep.x * (double)size.cx / (double)m_pGrid->size.x;
}

double CMeshView::GetVerticalStep() const
{
	CRect rect = GetGridRect();
	CSize size = rect.Size();
	return (double)m_pGrid->gridStep.y * (double)size.cy / (double)m_pGrid->size.y;
}

CRect CMeshView::GetCellRect(const Cell& cell) const
{
	CRect rect = GetGridRect();
	double dx = GetHorizontalStep();
	double dy = GetVerticalStep();
	int row = cell.GetRow();
	int column = cell.GetColumn();

	rect.right = rect.left + (int)(dx * (column + 1));
	rect.left += (int)(dx * column);
	rect.bottom = rect.top + (int)(dy * (row + 1));
	rect.top += (int)(dy * row);

	return rect;
}

CMeshView::Cell& CMeshView::GetCell(int row, int column)
{
	return m_pCells[row * m_pGrid->size.x + column];
}

void CMeshView::Refresh()
{
	if (!m_pCells) return;

	for (int i = 0; i < m_cellsCount; ++i)
	{
		m_pCells[i].Clear();
	}

	GridItem* pItem = NULL;
	GridFirstItem(m_pGrid, &pItem);

	while (pItem)
	{
		int x = (int)(pItem->position.x + 0.5);
		int y = (int)(pItem->position.y + 0.5);

		Cell& curCell = GetCell(y, x);
		curCell.Add(pItem);
		GridNextItem(m_pGrid, &pItem);
	}

}

void CMeshView::SetGrid(Grid* pGrid)
{
	if (m_pCells) delete[] m_pCells;
	m_pCells = m_pCurrentCell = NULL;
	m_pGrid = pGrid;
	m_cellsCount = m_pGrid ? (m_pGrid->size.x * m_pGrid->size.y) : 0;
	if (m_cellsCount) m_pCells = new Cell[m_cellsCount];
	for (unsigned i = 0; i < m_pGrid->size.y; ++i)
	{
		for (unsigned j = 0; j < m_pGrid->size.x; ++j)
		{
			Cell& cell = GetCell(i, j);
			cell.SetLocation(i, j);
		}
	}
	Refresh();
}

CMeshView::Cell* CMeshView::GetCellByLocation(CPoint point)
{
	if (!m_pCells) return NULL;
	CRect gridRect = GetGridRect();
	if (!gridRect.PtInRect(point)) return NULL;

	int column = (int)((double)(point.x - gridRect.left) / GetHorizontalStep());
	int row = (int)((double)(point.y - gridRect.top) / GetVerticalStep());
	return &GetCell(row, column);
}


// CMeshView message handlers

void CMeshView::OnMouseMove(UINT nFlags, CPoint point)
{
	Cell* pNewCurrCell = GetCellByLocation(point);
	if (!pNewCurrCell || pNewCurrCell == m_pCurrentCell) return;

	if (m_pCurrentCell) InvalidateRect(GetCellRect(*m_pCurrentCell));
	if (pNewCurrCell) InvalidateRect(GetCellRect(*pNewCurrCell));

	m_pCurrentCell = pNewCurrCell;
}

void CMeshView::OnMouseLeave()
{
	if (!m_pCurrentCell) return;
	InvalidateRect(GetCellRect(*m_pCurrentCell));
	m_pCurrentCell = NULL;
}

void CMeshView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	GetDocument()->AddStation(m_pCurrentCell->GetColumn(), m_pCurrentCell->GetRow());
}

void CMeshView::OnRButtonUp(UINT nFlags, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_MENU1);

	ClientToScreen(&point);
	CMenu* pPopup = menu.GetSubMenu(0);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);

	CView::OnRButtonUp(nFlags, point);
}

BOOL CMeshView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (!m_pCurrentCell || m_pCurrentCell->IsEmpty()) return CView::OnSetCursor(pWnd, nHitTest, message);
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
	return TRUE;
}

BOOL CMeshView::OnEraseBkgnd(CDC* pDC)
{
	return CView::OnEraseBkgnd(pDC);
}

// CMeshView diagnostics

#ifdef _DEBUG
void CMeshView::AssertValid() const
{
	CView::AssertValid();
}

void CMeshView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMeshDoc* CMeshView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMeshDoc)));
	return (CMeshDoc*)m_pDocument;
}
#endif //_DEBUG
