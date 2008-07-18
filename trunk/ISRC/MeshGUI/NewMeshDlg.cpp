// NewMeshDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewMeshDlg.h"

#define MAX_STEP_HORIZONTAL		1000
#define MAX_STEP_VERTICAL		1000
#define MAX_SIZE_WIDTH			1000
#define MAX_SIZE_HEIGHT			1000


// CNewMeshDlg dialog

IMPLEMENT_DYNAMIC(CNewMeshDlg, CDialog)

CNewMeshDlg::CNewMeshDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewMeshDlg::IDD, pParent),
	m_size(1,1)
{
}

CNewMeshDlg::~CNewMeshDlg()
{
}

void CNewMeshDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_SIZE_WIDTH, m_size.cx);
	DDV_MinMaxLong(pDX, m_size.cx, 1, MAX_SIZE_WIDTH);

	DDX_Text(pDX, IDC_SIZE_HEIGHT, m_size.cy);
	DDV_MinMaxLong(pDX, m_size.cy, 1, MAX_SIZE_HEIGHT);

}


BEGIN_MESSAGE_MAP(CNewMeshDlg, CDialog)
END_MESSAGE_MAP()


// CNewMeshDlg message handlers
