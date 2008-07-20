#pragma once

enum EVisualItems
{
	eVI_BACKGROUND,
	eVI_COVERAGE,
	eVI_GRID,
	eVI_RULES,
	eVI_STATIONS,
	eVI_DATA_FLOW,
	eVI_ROUTING,

	eVI_SETTINGS,

	// terminator
	eVI_LAST
};

class CMeshSettings
{
public:
	CMeshSettings();
	bool	IsChanged()									{ bool tmp = m_bChanged; m_bChanged = false; return tmp; }

	bool	IsVisible(EVisualItems eVisualItem) const	{ return m_visualItems[eVisualItem]; }
	void	Hide(EVisualItems eVisualItem) 				{ m_visualItems[eVisualItem] = false; SetChanged(); }
	void	Show(EVisualItems eVisualItem) 				{ m_visualItems[eVisualItem] = true; SetChanged(); }
	bool	Toggle(EVisualItems eVisualItem)			{ if (IsVisible(eVisualItem)) {Hide(eVisualItem); return true;} Show(eVisualItem); return false; }

	int		GetSpeed() const			{ return m_nSpeed; }
	double	GetCoverageRange() const	{ return m_nCoverage; }
	int		GetDuration() const			{ return m_nDuration; }

	void	SetSpeed(int speed)				{ Update(speed, m_nSpeed); }
	void	SetCoverageRange(double range)	{ Update(range, m_nCoverage, eVI_COVERAGE); }
	void    SetDuration(int duration)		{ Update(duration, m_nDuration); }

private:
	template <class T>
	void	Update(T value, T& member, EVisualItems item = eVI_LAST) { if (value != member) { member = value; SetChanged(item != eVI_LAST ? IsVisible(item) : true); } }
	void	SetChanged(bool changed = true)	{ m_bChanged = m_bChanged || changed; }
	bool	m_visualItems[eVI_LAST];

	int		m_nSpeed;
	double	m_nCoverage;
	int		m_nDuration;
	bool	m_bChanged;
};
