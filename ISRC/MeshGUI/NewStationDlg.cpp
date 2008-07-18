// NewStationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewStationDlg.h"


// CNewStationDlg dialog

IMPLEMENT_DYNAMIC(CNewStationDlg, CDialog)

CNewStationDlg::CNewStationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewStationDlg::IDD, pParent)
	, m_positionX(0)
	, m_positionY(0)
	, m_speed(0)
	, m_angle(0)
{

}

CNewStationDlg::~CNewStationDlg()
{
}

void CNewStationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_STATION_POS_X, m_positionX);
	DDV_MinMaxDouble(pDX, m_positionX, 0, 1000);

	DDX_Text(pDX, IDC_STATION_POS_Y, m_positionY);
	DDV_MinMaxDouble(pDX, m_positionY, 0, 1000);

	DDX_Text(pDX, IDC_STATION_SPEED, m_speed);
	DDV_MinMaxDouble(pDX, m_speed, 0, 1000);

	DDX_Text(pDX, IDC_STATION_ANGLE, m_angle);
	DDV_MinMaxDouble(pDX, m_angle, 0, 360);
}


BEGIN_MESSAGE_MAP(CNewStationDlg, CDialog)
END_MESSAGE_MAP()


// CNewStationDlg message handlers
