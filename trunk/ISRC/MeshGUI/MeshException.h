#pragma once

#include "stdafx.h"
#include <strsafe.h>
#include <Status.h>

class CMeshException : public CException
{
public:
	CMeshException(EStatus status, BOOL bAutoDelete = TRUE) : CException(bAutoDelete), m_status(status) {}
	virtual ~CMeshException() {}
	virtual BOOL GetErrorMessage(LPTSTR str, UINT nMaxError, PUINT pnHelpContext = NULL) const
	{
		lstrcpyn(str, GetStatusMessage(m_status), nMaxError);
		return TRUE;
	}

private:
	EStatus m_status;
};

#define MESH_CHECK_STATUS(status) { EStatus __status = (status); if (__status != eSTATUS_COMMON_OK) THROW(new CMeshException(__status, TRUE)); }
