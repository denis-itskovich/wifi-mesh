#pragma once

#include "SimulationBar.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual ~CMainFrame();

	// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSimulationSettings();

	DECLARE_MESSAGE_MAP()

protected:  // control bar embedded members
	CStatusBar		m_wndStatusBar;
	CToolBar		m_wndToolBar;
	CReBar			m_wndReBar;
	CSimulationBar	m_simulationBar;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	afx_msg void OnUpdateViewSimulationToolbar(CCmdUI *pCmdUI);
	afx_msg void OnViewSimulationToolbar();
};
