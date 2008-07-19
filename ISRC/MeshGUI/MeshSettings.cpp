#include "stdafx.h"
#include "MeshSettings.h"

CMeshSettings::CMeshSettings()
{
	for (int i = 0; i < eVI_LAST; ++i)
	{
		m_visualItems[i] = true;
	}
	m_nCoverage = 1;
	m_nSpeed = 1;
}
