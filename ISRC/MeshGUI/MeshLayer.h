#pragma once

class CMeshLayer
{
public:
    CMeshLayer(CRect rect, BYTE alpha = 0xFF);
    virtual ~CMeshLayer();

    virtual void OnSize(CRect rcNew);
    virtual void OnShow(bool bShow);
    virtual void OnDraw(CDC* pDC);

protected:
    virtual void Draw(CDC* pDC) = 0;

    virtual void CreateCache();
    virtual void DestroyCache();
    virtual CRect GetClientRect();

private:
    CBitmap m_bmpCache;
    CRect   m_rect;
};
