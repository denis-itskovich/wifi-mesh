#pragma once

// forward declaration
typedef struct _Grid Grid;

class CMeshView : public CView
{
	// forward declaration of inner class
	class Cell;

protected:
	CMeshView();
	DECLARE_DYNCREATE(CMeshView)

public:
	virtual	~CMeshView();

	CMeshDoc*		GetDocument() const;
	void			Refresh();

protected:
	virtual void	DrawBackground(CDC* pDC);
	virtual void	DrawGrid(CDC* pDC);
	virtual void	DrawStations(CDC* pDC);
	virtual void	DrawCoverage(CDC* pDC);
	virtual void	DrawCell(CDC* pDC, Cell& cell, BOOL bSelected = FALSE);

	virtual void	CalcCoverage(CRgn& pRgn, Cell& cell, double radius);
	virtual void	CalcCoverage(CRgn& pRgn, double radius);
	virtual void	DrawRules(CDC* pDC);

private:
	double			GetHorizontalStep() const;
	double			GetVerticalStep() const;
	CRect			GetGridRect() const;
	CRect			GetCellRect(const Cell& cell) const;
	CRect			GetVerticalRuleRect() const;
	CRect			GetHorizontalRuleRect() const;
	Cell&			GetCell(int row, int column);
	Cell*			GetCellByLocation(CPoint point);
	Grid*			GetGrid() const { return GetDocument()->GetGrid(); }
	CMeshSettings&	GetSettings() const { return GetDocument()->GetSettings(); }
	int				GetCellsCount() const { return m_rows * m_columns; }

protected:
	// Message handlers
	DECLARE_MESSAGE_MAP()

	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void	OnMouseLeave();
	afx_msg void	OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void	OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL	OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL	OnEraseBkgnd(CDC* pDC);

public:
	// overrides
	virtual void	OnDraw(CDC* pDC);  // overridden to draw this view

private:
	Cell*	m_pCells;
	Cell*	m_pCurrentCell;
	int		m_rows;
	int		m_columns;
	CRgn	m_primaryCoverage;
	CRgn	m_secondaryCoverage;
	CBitmap m_wifiBitmap;

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

#ifndef _DEBUG  // debug version in MeshGUIView.cpp
	inline CMeshDoc* CMeshView::GetDocument() const
	{
		return reinterpret_cast<CMeshDoc*>(m_pDocument);
	}
#endif

