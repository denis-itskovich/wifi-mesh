#pragma once

#include "resource.h"

// CNewMeshDlg dialog

class CNewMeshDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewMeshDlg)

public:
	CNewMeshDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewMeshDlg();

// Dialog Data
	enum { IDD = IDD_NEWDOCUMENT };
	CSize GetSize() { return m_size; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CSize	m_size;
public:
};
