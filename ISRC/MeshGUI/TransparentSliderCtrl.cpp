#include "StdAfx.h"
#include "TransparentSliderCtrl.h"

BEGIN_MESSAGE_MAP(CTransparentSliderCtrl, CSliderCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CTransparentSliderCtrl::OnNMCustomdraw)
	ON_WM_CREATE()
END_MESSAGE_MAP()

void CTransparentSliderCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	if (pNMCD->dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
		m_uState = pNMCD->uItemState;
		return;
	}

	switch (pNMCD->dwItemSpec)
	{
	case TBCD_CHANNEL:
		{
			CRect rect;
			GetClientRect(rect);
			if (m_uState & CDIS_FOCUS) rect.DeflateRect(1,1,1,1);
			DrawThemeParentBackground(GetSafeHwnd(), pNMCD->hdc, rect);
		}
		break;
	case TBCD_THUMB:
		{
			CString str;
			str.Format(_T("%d"), GetPos());
			m_toolTip.SetTitle(0, str);
			m_toolTip.Popup();
		}
	}
	*pResult = 0;
}

BOOL CTransparentSliderCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: Add your specialized code here and/or call the base class

	if (!CSliderCtrl::Create(dwStyle, rect, pParentWnd, nID)) return FALSE;
	return m_toolTip.Create(this);
}

BOOL CTransparentSliderCtrl::CreateEx(DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: Add your specialized code here and/or call the base class

	return CSliderCtrl::CreateEx(dwExStyle, dwStyle, rect, pParentWnd, nID);
}

int CTransparentSliderCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CSliderCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

void CTransparentSliderCtrl::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	m_toolTip.Create(GetParent()->GetParent());
	CSliderCtrl::PreSubclassWindow();
}
