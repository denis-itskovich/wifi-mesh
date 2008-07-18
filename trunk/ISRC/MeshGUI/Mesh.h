#pragma once

#include "resource.h"

class CMeshApp : public CWinApp
{
public:
	CMeshApp();

	virtual BOOL InitInstance();
	afx_msg void OnAppAbout();

	DECLARE_MESSAGE_MAP()
};

extern CMeshApp theApp;
