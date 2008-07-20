// EditStationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EditStationDlg.h"


// CEditStationDlg dialog

IMPLEMENT_DYNAMIC(CEditStationDlg, CDialog)

CEditStationDlg::CEditStationDlg(const CString& title, int gridWidth, int gridHeight)
	: CDialog(CEditStationDlg::IDD)
	, m_positionX(0)
	, m_positionY(0)
	, m_speed(0)
	, m_angle(0)
    , m_title(title)
    , m_gridWidth(gridWidth)
    , m_gridHeight(gridHeight)
{
}

CEditStationDlg::~CEditStationDlg()
{
}

void CEditStationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_STATION_POS_X, m_positionX);
	DDV_MinMaxDouble(pDX, m_positionX, 0, m_gridWidth);

	DDX_Text(pDX, IDC_STATION_POS_Y, m_positionY);
	DDV_MinMaxDouble(pDX, m_positionY, 0, m_gridHeight);

	DDX_Text(pDX, IDC_STATION_SPEED, m_speed);
	DDV_MinMaxDouble(pDX, m_speed, 0, max(m_gridWidth, m_gridHeight));

	DDX_Text(pDX, IDC_STATION_ANGLE, m_angle);
	DDV_MinMaxDouble(pDX, m_angle, 0, 360);

    DDX_Text(pDX, IDC_STATION_ID, m_id);
    DDV_MinMaxInt(pDX, m_id, 0, 2000);
}


BEGIN_MESSAGE_MAP(CEditStationDlg, CDialog)
END_MESSAGE_MAP()


// CEditStationDlg message handlers

BOOL CEditStationDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    SetWindowText(m_title);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
