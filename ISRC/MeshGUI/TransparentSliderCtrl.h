#pragma once

class CTransparentSliderCtrl : public CSliderCtrl
{
public:
	CTransparentSliderCtrl() : m_multiplier(1.0) {}
	virtual ~CTransparentSliderCtrl() {}

	void SetMultiplier(double multi) { m_multiplier = multi; }
	double GetEffectivePosition() const { return (double)GetPos() * m_multiplier; }
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
private:
	unsigned		m_uState;
	int				m_lastPos;
	double			m_multiplier;
};
