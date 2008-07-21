#pragma once

#include "afxcmn.h"
#include "MeshSettings.h"

// CSettingsBar form view

class CSettingsBar : public CDialogBar
{
	DECLARE_DYNCREATE(CSettingsBar)

public:
	CSettingsBar(CMeshSettings* pSettings);           // protected constructor used by dynamic creation
	virtual ~CSettingsBar();

	BOOL Create(CWnd* pParentWnd);
	enum { IDD = IDD_SIMULATION_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSettingChanged();
	afx_msg void OnSpeedChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCoverageChanged(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()

protected:
	CString			m_speedText;
	CString			m_coverageText;
	CMeshSettings*	m_pSettings;

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
};


