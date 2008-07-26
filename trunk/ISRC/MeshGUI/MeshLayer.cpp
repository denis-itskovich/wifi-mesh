#include "stdafx.h"
#include "MeshLayer.h"

CMeshLayer::CMeshLayer(CRect rect, BYTE alpha) :
m_rect(rect)
{

}

CMeshLayer::~CMeshLayer()
{

}

void CMeshLayer::OnSize(CRect rcNew)
{

}

void CMeshLayer::OnShow(bool bShow)
{

}

void CMeshLayer::OnDraw(CDC* pDC)
{

}

void CMeshLayer::CreateCache()
{

}

void CMeshLayer::DestroyCache()
{

}

CRect CMeshLayer::GetClientRect()
{
    return m_rect;
}
