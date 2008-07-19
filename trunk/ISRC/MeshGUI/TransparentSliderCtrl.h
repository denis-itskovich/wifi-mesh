#pragma once

class CTransparentSliderCtrl : public CSliderCtrl
{
public:
	CTransparentSliderCtrl() {}
	virtual ~CTransparentSliderCtrl() {}

	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
private:
	unsigned		m_uState;
	CToolTipCtrl	m_toolTip;
};
