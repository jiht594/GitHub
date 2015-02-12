
// MFCSMView.cpp : implementation of the CMFCSMView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCSM.h"
#endif

#include "MFCSMDoc.h"
#include "MFCSMView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCSMView

IMPLEMENT_DYNCREATE(CMFCSMView, CView)

BEGIN_MESSAGE_MAP(CMFCSMView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCSMView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMFCSMView construction/destruction

CMFCSMView::CMFCSMView()
{
	// TODO: add construction code here

}

CMFCSMView::~CMFCSMView()
{
}

BOOL CMFCSMView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMFCSMView drawing

void CMFCSMView::OnDraw(CDC* /*pDC*/)
{
	CMFCSMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CMFCSMView printing


void CMFCSMView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCSMView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMFCSMView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMFCSMView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMFCSMView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCSMView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCSMView diagnostics

#ifdef _DEBUG
void CMFCSMView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCSMView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCSMDoc* CMFCSMView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCSMDoc)));
	return (CMFCSMDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCSMView message handlers


void CMFCSMView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CView::OnLButtonDown(nFlags, point);
}
