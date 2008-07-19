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
public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL CreateEx(DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual void PreSubclassWindow();
};
