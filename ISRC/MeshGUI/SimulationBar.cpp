#include "StdAfx.h"
#include "resource.h"
#include "SimulationBar.h"

BEGIN_MESSAGE_MAP(CSimulationBar, CDialogBar)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

void CSimulationBar::DoDataExchange(CDataExchange *pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPEED_SLIDER, m_speedSlider);
	DDX_Control(pDX, IDC_COVERAGE_SLIDER, m_coverageSlider);
	DDX_Control(pDX, IDC_DURATION_SLIDER, m_durationSlider);
}

BOOL CSimulationBar::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	DrawThemeParentBackground(GetSafeHwnd(), pDC->GetSafeHdc(), NULL);
	return TRUE;
}

HBRUSH CSimulationBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// TODO:  Change any attributes of the DC here
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_SPEED_SLIDER:
	case IDC_COVERAGE_SLIDER:
	case IDC_DURATION_SLIDER:;
		return CDialogBar::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
	}

	// TODO:  Return a different brush if the default is not desired
	return CDialogBar::OnCtlColor(pDC, pWnd, nCtlColor);;
}

BOOL CSimulationBar::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: Add your specialized code here and/or call the base class
	if (!CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID)) return FALSE;
	return OnInitDialogBar();
}

BOOL CSimulationBar::OnInitDialogBar()
{
	UpdateData(FALSE);
	return TRUE;
}