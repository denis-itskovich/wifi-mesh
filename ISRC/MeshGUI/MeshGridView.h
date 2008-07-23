#pragma once

#include "MeshView.h"
// forward declaration
typedef struct _Grid Grid;

class CMeshGridView : public CMeshView
{
	// forward declaration of inner class
	class Cell;

protected:
	CMeshGridView();
	DECLARE_DYNCREATE(CMeshGridView)

public:
	virtual	~CMeshGridView();
    virtual void        Refresh();

protected:
	virtual void	DrawBackground(CDC* pDC);
	virtual void	DrawGrid(CDC* pDC);
	virtual void	DrawStations(CDC* pDC);
	virtual void	DrawCoverage(CDC* pDC);
	virtual void	DrawCell(CDC* pDC, Cell& cell, BOOL bSelected = FALSE);

	virtual void	CalcCoverage(CRgn& pRgn, const Cell& cell, double radius) const;
	virtual void	CalcCoverage(CRgn& pRgn, double radius) const;
	virtual void	DrawRules(CDC* pDC);

private:
    void            RefreshCoverage();
	double			GetHorizontalStep() const;
	double			GetVerticalStep() const;
	CRect			GetGridRect() const;
	CRect			GetCellRect(const Cell& cell) const;
    void            GetCellRgn(const Cell& cell, CRgn& rgn, bool bEmpty) const;
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

	afx_msg void	OnViewShowBackground();
	afx_msg void	OnViewShowCoverage();
	afx_msg void	OnViewShowGrid();
	afx_msg void	OnViewShowRules();
	afx_msg void	OnViewShowStations();
	afx_msg void	OnViewShowDataFlow();
	afx_msg void	OnViewShowRouting();

    afx_msg void    OnUpdateViewSubMenus(CCmdUI *pCmdUI);
    afx_msg void    OnViewShowbackground();

    afx_msg void    OnPopupStationAdd();
    afx_msg void    OnPopupStationEdit();
    afx_msg void    OnPopupStationRemove();

    afx_msg void    OnUpdatePopupStationEdit(CCmdUI *pCmdUI);
    afx_msg void    OnUpdatePopupStationAdd(CCmdUI *pCmdUI);
    afx_msg void    OnSize(UINT nType, int cx, int cy);
    afx_msg void    OnTimer(UINT_PTR nIDEvent);

public:
	// overrides
	virtual void	OnDraw(CDC* pDC);  // overridden to draw this view

private:
	Cell*	m_pCells;
	Cell*	m_pCurrentCell;
	int		m_rows;
	int		m_columns;
    double  m_range;
	CRgn	m_primaryCoverage;
	CRgn	m_secondaryCoverage;
	CBitmap m_wifiBitmap;
    int     m_nStep;
    bool    m_bIsRunning;

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
    afx_msg void OnUpdateSimulationRun(CCmdUI *pCmdUI);
    afx_msg void OnUpdateSimulationBreak(CCmdUI *pCmdUI);
    afx_msg void OnSimulationRun();
    afx_msg void OnSimulationBreak();
};
