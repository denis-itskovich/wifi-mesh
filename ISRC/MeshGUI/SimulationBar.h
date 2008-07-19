#pragma once

#include "TransparentSliderCtrl.h"

class CSimulationBar : public CDialogBar
{
public:
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialogBar();

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:
	CTransparentSliderCtrl	m_speedSlider;
	CTransparentSliderCtrl	m_coverageSlider;
	CTransparentSliderCtrl	m_durationSlider;
};
