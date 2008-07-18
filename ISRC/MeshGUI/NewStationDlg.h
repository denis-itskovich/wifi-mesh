#pragma once

#include "resource.h"

// CNewStationDlg dialog

class CNewStationDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewStationDlg)

public:
	CNewStationDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewStationDlg();

// Dialog Data
	enum { IDD = IDD_NEW_STATION };

	double m_positionX;
	double m_positionY;
	double m_speed;
	double m_angle;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
