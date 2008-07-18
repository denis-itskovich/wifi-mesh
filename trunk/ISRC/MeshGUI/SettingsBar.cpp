// SettingsBar.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "SettingsBar.h"


// CSettingsBar

IMPLEMENT_DYNCREATE(CSettingsBar, CDialogBar)

CSettingsBar::CSettingsBar() :
	  m_showRouting(TRUE)
	, m_showDataFlow(TRUE)
	, m_showStations(TRUE)
	, m_showBackground(TRUE)
	, m_showRules(TRUE)
	, m_showGrid(TRUE)
	, m_showCoverage(TRUE)
	, m_speed(1)
	, m_coverage(1)
{
}

CSettingsBar::~CSettingsBar()
{
}

void CSettingsBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	if (!pDX->m_bSaveAndValidate)
	{
		((CSliderCtrl*)GetDlgItem(IDC_SIMULATION_SPEED))->SetRange(1,100);
		((CSliderCtrl*)GetDlgItem(IDC_COVERAGE_RANGE))->SetRange(1, 50);
	}

	DDX_Check(pDX, IDC_SIMULATION_SHOW_BACKGROUND, m_showBackground);
	DDX_Check(pDX, IDC_SIMULATION_SHOW_GRID, m_showGrid);
	DDX_Check(pDX, IDC_SIMULATION_SHOW_RULES, m_showRules);
	DDX_Check(pDX, IDC_SIMULATION_SHOW_STATIONS, m_showStations);
	DDX_Check(pDX, IDC_SIMULATION_SHOW_DATA, m_showDataFlow);
	DDX_Check(pDX, IDC_SIMULATION_SHOW_ROUTING, m_showRouting);
	DDX_Check(pDX, IDC_SIMULATION_SHOW_COVERAGE, m_showCoverage);

	DDX_Slider(pDX, IDC_SIMULATION_SPEED, m_speed);
	DDX_Slider(pDX, IDC_COVERAGE_RANGE, m_coverage);

	DDX_Text(pDX, IDC_SPEED_TEXT, m_speedText);
	DDX_Text(pDX, IDC_SPEED_TEXT, m_coverageText);

	m_speedText.Format(L"%d", m_speed);
	m_coverageText.Format(L"%d", m_coverage);
	SetDlgItemText(IDC_SPEED_TEXT, m_speedText);
	SetDlgItemText(IDC_COVERAGE_TEXT, m_coverageText);
}

BEGIN_MESSAGE_MAP(CSettingsBar, CDialogBar)
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SIMULATION_SPEED, &CSettingsBar::OnSpeedChanged)
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_COVERAGE_RANGE, &CSettingsBar::OnCoverageChanged)
	ON_BN_CLICKED(IDC_SIMULATION_SHOW_BACKGROUND, &CSettingsBar::OnSettingChanged)
	ON_BN_CLICKED(IDC_SIMULATION_SHOW_GRID, &CSettingsBar::OnSettingChanged)
	ON_BN_CLICKED(IDC_SIMULATION_SHOW_RULES, &CSettingsBar::OnSettingChanged)
	ON_BN_CLICKED(IDC_SIMULATION_SHOW_STATIONS, &CSettingsBar::OnSettingChanged)
	ON_BN_CLICKED(IDC_SIMULATION_SHOW_DATA, &CSettingsBar::OnSettingChanged)
	ON_BN_CLICKED(IDC_SIMULATION_SHOW_ROUTING, &CSettingsBar::OnSettingChanged)
	ON_BN_CLICKED(IDC_SIMULATION_SHOW_COVERAGE, &CSettingsBar::OnSettingChanged)
END_MESSAGE_MAP()


BOOL CSettingsBar::Create(CWnd* pParentWnd)
{
	return CDialogBar::Create(pParentWnd, IDD_SIMULATION_SETTINGS, WS_CHILD | CBRS_LEFT | CBRS_RIGHT, IDD_SIMULATION_SETTINGS);
}

// CSettingsBar diagnostics

#ifdef _DEBUG
void CSettingsBar::AssertValid() const
{
	CDialogBar::AssertValid();
}

#ifndef _WIN32_WCE
void CSettingsBar::Dump(CDumpContext& dc) const
{
	CDialogBar::Dump(dc);
}
#endif
#endif //_DEBUG


// CSettingsBar message handlers

void CSettingsBar::OnSettingChanged()
{
	UpdateData();
	AfxGetApp()->GetMainWnd()->Invalidate();
}

void CSettingsBar::OnSpeedChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData();
}

void CSettingsBar::OnCoverageChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnSettingChanged();
}
