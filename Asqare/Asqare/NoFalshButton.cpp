// NoFalshButton.cpp : implementation file
//

#include "stdafx.h"
#include "Asqare.h"
#include "NoFalshButton.h"


// CNoFalshButton

IMPLEMENT_DYNAMIC(CNoFalshButton, CBitmapButton)

CNoFalshButton::CNoFalshButton()
{
    m_bDrawRect = false;
}

CNoFalshButton::~CNoFalshButton()
{
}


BEGIN_MESSAGE_MAP(CNoFalshButton, CBitmapButton)
    ON_WM_ERASEBKGND()
//ON_WM_DRAWITEM()
ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CNoFalshButton::OnEraseBkgnd( CDC* pDC )
{
    UNREFERENCED_PARAMETER(pDC);
    return TRUE;
}



// CNoFalshButton message handlers




//void CNoFalshButton::OnDrawClipboard()
//{
//    CBitmapButton::OnDrawClipboard();
//
//    // TODO: Add your message handler code here
//}


//void CNoFalshButton::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
//{
//    CBitmapButton::OnDrawItem(nIDCtl, lpDrawItemStruct);
//}


void CNoFalshButton::PreSubclassWindow()
{
    // TODO: Add your specialized code here and/or call the base class

    CBitmapButton::PreSubclassWindow();
}


//void CNoFalshButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
//{
//    
//}


void CNoFalshButton::OnPaint()
{
    CBitmapButton::OnPaint();
    CPaintDC dc1(this); // device context for painting
    if (m_bDrawRect)
    {
        //Ê¹ÓÃClientDC»­2¸öºì¿ò
        CClientDC dc(this);
        CRect r1; 
        CPen pen(PS_SOLID, 3, RGB(255, 0, 0));
        CPen *p = dc.SelectObject(&pen);
        GetClientRect(&r1);
        this->GetClientRect(&r1);
        r1.top -= 1;
        r1.bottom += 1;
        r1.left -=1;
        r1.right += 1;

        CPoint bottomRight = r1.BottomRight();
        CPoint topleft = r1.TopLeft();
        dc.MoveTo(topleft);
        dc.LineTo(CPoint(r1.top, r1.right));
        dc.LineTo(bottomRight);
        dc.LineTo(CPoint(r1.bottom, r1.left));
        dc.LineTo(topleft);
        dc.SelectObject(p);
    }
}
