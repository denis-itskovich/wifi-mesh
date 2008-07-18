#pragma once
#include "afxcmn.h"



// CSettingsBar form view

class CSettingsBar : public CDialogBar
{
	DECLARE_DYNCREATE(CSettingsBar)

public:
	CSettingsBar();           // protected constructor used by dynamic creation
	virtual ~CSettingsBar();

	BOOL Create(CWnd* pParentWnd);
	enum { IDD = IDD_SIMULATION_SETTINGS };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	BOOL m_showRouting;
	BOOL m_showDataFlow;
	BOOL m_showStations;
	BOOL m_showBackground;
	BOOL m_showRules;
	BOOL m_showGrid;
	BOOL m_showCoverage;
	int	 m_speed;
	int  m_coverage;

protected:
	CString m_speedText;
	CString m_coverageText;

	afx_msg void OnSettingChanged();
	afx_msg void OnSpeedChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCoverageChanged(NMHDR *pNMHDR, LRESULT *pResult);
};


