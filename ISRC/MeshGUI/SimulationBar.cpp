#include "StdAfx.h"
#include "resource.h"
#include "SimulationBar.h"
#include "MeshDoc.h"

BEGIN_MESSAGE_MAP(CSimulationBar, CDialogBar)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_NOTIFY_REFLECT(TRBN_THUMBPOSCHANGING, &CSimulationBar::OnTRBNThumbPosChanging)
END_MESSAGE_MAP()

void CSimulationBar::DoDataExchange(CDataExchange *pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPEED_SLIDER, m_speedSlider);
	DDX_Control(pDX, IDC_COVERAGE_SLIDER, m_coverageSlider);
	DDX_Control(pDX, IDC_DURATION_SLIDER, m_durationSlider);

	SetSliderLabel(IDC_SPEED_TEXT, m_speedSlider);
	SetSliderLabel(IDC_COVERAGE_TEXT, m_coverageSlider);
	SetSliderLabel(IDC_DURATION_TEXT, m_durationSlider);

	CMeshSettings& settings = GetSettings();
	settings.SetSpeed(m_speedSlider.GetPos());
	settings.SetCoverageRange(m_coverageSlider.GetEffectivePosition());
	settings.SetDuration(m_durationSlider.GetPos());
	if (settings.IsChanged()) CMeshDoc::GetInstance()->RefreshViews();
}

CMeshSettings& CSimulationBar::GetSettings() const
{
	CMeshDoc* pDoc = CMeshDoc::GetInstance();
	ASSERT(pDoc != NULL);
	return pDoc->GetSettings();
}

void CSimulationBar::SetSliderLabel(int labelId, const CTransparentSliderCtrl& slider)
{
	CString text;
	text.Format(_T("%g"), slider.GetEffectivePosition());
	CWnd* pWnd = GetDlgItem(labelId);
	CString old;
	pWnd->GetWindowText(old);
	if (old == text) return;

	CRect rc;
	pWnd->GetClientRect(rc);
	pWnd->ClientToScreen(rc);
	ScreenToClient(rc);
	InvalidateRect(rc);

	pWnd->SetWindowText(text);

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
	m_coverageSlider.SetMultiplier(0.1);
	return TRUE;
}

void CSimulationBar::OnTRBNThumbPosChanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData();
	*pResult = 0;
}

BOOL CSimulationBar::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class

	NMHDR* pNMHDR = (NMHDR*)lParam;
	if (pNMHDR && pNMHDR->code == TRBN_THUMBPOSCHANGING)
	{
		OnTRBNThumbPosChanging(pNMHDR, pResult);
		return TRUE;
	}
	return CDialogBar::OnNotify(wParam, lParam, pResult);
}
