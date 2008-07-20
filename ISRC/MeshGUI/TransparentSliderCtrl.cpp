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

	CDC* pDC = CDC::FromHandle(pNMCD->hdc);

	if (pNMCD->dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
		m_uState = pNMCD->uItemState;
		return;
	}

	*pResult = 0;

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
		if (m_lastPos != GetPos())
		{
			m_lastPos = GetPos();
			NMTRBTHUMBPOSCHANGING posChanged = {{GetSafeHwnd(), GetDlgCtrlID(), TRBN_THUMBPOSCHANGING}, m_lastPos, TB_THUMBPOSITION};
			GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&posChanged);
		}
		break;
	}
}
