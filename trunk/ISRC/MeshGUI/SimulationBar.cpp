#include "StdAfx.h"
#include "resource.h"
#include "SimulationBar.h"

BEGIN_MESSAGE_MAP(CSimulationBar, CDialogBar)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_NCPAINT()
END_MESSAGE_MAP()

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

void CSimulationBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogBar::OnPaint() for painting messages
}

void CSimulationBar::OnNcPaint()
{
	TRACE0("test");
	// TODO: Add your message handler code here
	// Do not call CDialogBar::OnNcPaint() for painting messages
}
