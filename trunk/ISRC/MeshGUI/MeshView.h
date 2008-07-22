#pragma once

class CMeshView : public CView
{
protected:
    CMeshView() {}

public:
    virtual	~CMeshView() {}

	virtual CMeshDoc*	GetDocument() const;
	virtual void        Refresh() = 0;
};

#ifndef _DEBUG  // debug version in MeshView.cpp
	inline CMeshDoc* CMeshView::GetDocument() const
	{
		return reinterpret_cast<CMeshDoc*>(m_pDocument);
	}
#endif

