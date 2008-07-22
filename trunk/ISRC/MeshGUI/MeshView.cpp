#include "stdafx.h"
#include "MeshDoc.h"
#include "MeshView.h"

#ifdef _DEBUG
CMeshDoc* CMeshView::GetDocument() const // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMeshDoc)));
    return (CMeshDoc*)m_pDocument;
}
#endif // _DEBUG
