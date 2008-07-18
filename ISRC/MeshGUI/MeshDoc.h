// MeshGUIDoc.h : interface of the CMeshDoc class
//

#pragma once

#include "SettingsBar.h"

EXTERN_C
{
	#include <Grid.h>
};

class CMeshView;

class CMeshDoc : public CDocument
{
protected: // create from serialization only
	CMeshDoc();
	DECLARE_DYNCREATE(CMeshDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual Grid* GetGrid();

	CMeshView* GetMainView();
	CSettingsBar* GetSettings();
	void AddStation(int x, int y);
	void Iterate();

// Implementation
public:
	virtual ~CMeshDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// Generated message map functions
	afx_msg void OnUpdateSimulationRun(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSimulationBreak(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSimulationSettings(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewSettings(CCmdUI* pCmdUI);
	afx_msg void OnSimulationRun();

protected:
	DECLARE_MESSAGE_MAP()


private:
	BOOL IsGridValid() const;
	BOOL IsStarted() const;

	Grid	m_grid;
	BOOL	m_bStarted;
	BOOL	m_bInitialized;
	ULONG	m_currentId;

};
