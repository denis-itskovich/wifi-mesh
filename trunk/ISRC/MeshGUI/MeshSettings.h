#pragma once

enum EVisualItems
{
	eVI_BACKGROUND,
	eVI_COVERAGE,
	eVI_GRID,
	eVI_RULES,
	eVI_STATIONS,
	eVI_DATA_FLOW,

	eVI_SETTINGS,

	// terminator
	eVI_LAST
};

class CMeshSettings
{
public:
	CMeshSettings();
	bool	IsVisible(EVisualItems eVisualItem) const	{ return m_visualItems[eVisualItem]; }
	void	Hide(EVisualItems eVisualItem) 				{ m_visualItems[eVisualItem] = false; }
	void	Show(EVisualItems eVisualItem) 				{ m_visualItems[eVisualItem] = true; }

	int		GetSpeed() const			{ return m_nSpeed; }
	int		GetCoverageRange() const	{ return m_nCoverage; }

private:
	bool m_visualItems[eVI_LAST];

	int	 m_nSpeed;
	int  m_nCoverage;
};
