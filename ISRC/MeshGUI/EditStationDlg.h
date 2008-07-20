#pragma once

#include "resource.h"

// CEditStationDlg dialog

class CEditStationDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditStationDlg)

public:
    CEditStationDlg(const CString& title, int gridWidth, int gridHeight);
	virtual ~CEditStationDlg();

// Dialog Data
	enum { IDD = IDD_EDIT_STATION };

	double  m_positionX;
	double  m_positionY;
	double  m_speed;
	double  m_angle;
    int     m_id;
    CString m_title;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();

private:
    int m_gridWidth;
    int m_gridHeight;
};
