#pragma once

EXTERN_C
{
	#include <Grid.h>
};

#include "MeshSettings.h"

class CMeshDoc : public CDocument
{
protected: // create from serialization only
	CMeshDoc();
	DECLARE_DYNCREATE(CMeshDoc)
public:
	virtual ~CMeshDoc();

public:
	virtual BOOL	OnNewDocument();
	virtual void	Serialize(CArchive& ar);

	virtual Grid*	GetGrid();
	CMeshSettings&	GetSettings() { return m_settings; }

	void			AddStation(int x, int y);
    void            EditStation(int x, int y);
    void            RemoveStation(int x, int y);
    void            MoveStation(int srcX, int srcY, int dstX, int dstY);

	void			Iterate();
	void			Refresh();
	void			RefreshViews();
	bool			IsChanged();
	bool			IsValid() const;

	static CMeshDoc* GetInstance() { return m_pInstance; }
protected:
	DECLARE_MESSAGE_MAP()

private:
	bool			IsStarted() const;
    GridItem*       GetStationAt(int x, int y);

	Grid*			m_pGrid;
	CMeshSettings	m_settings;
	unsigned long	m_currentId;
	bool			m_bInitialized;
	bool			m_bChanged;

	static CMeshDoc* m_pInstance;

#ifdef _DEBUG
public:
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
