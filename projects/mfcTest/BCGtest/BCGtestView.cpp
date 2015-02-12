// BCGtestView.cpp : implementation of the CBCGtestView class
//

#include "stdafx.h"
#include "BCGtest.h"

#include "BCGtestDoc.h"
#include "BCGtestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBCGtestView

IMPLEMENT_DYNCREATE(CBCGtestView, CView)

BEGIN_MESSAGE_MAP(CBCGtestView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
END_MESSAGE_MAP()

// CBCGtestView construction/destruction

CBCGtestView::CBCGtestView()
{
	// TODO: add construction code here

}

CBCGtestView::~CBCGtestView()
{
}

BOOL CBCGtestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CBCGtestView drawing

void CBCGtestView::OnDraw(CDC* /*pDC*/)
{
	CBCGtestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


// CBCGtestView printing

void CBCGtestView::OnFilePrintPreview()
{
	BCGPPrintPreview (this);
}

BOOL CBCGtestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBCGtestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBCGtestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CBCGtestView diagnostics

#ifdef _DEBUG
void CBCGtestView::AssertValid() const
{
	CView::AssertValid();
}

void CBCGtestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBCGtestDoc* CBCGtestView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBCGtestDoc)));
	return (CBCGtestDoc*)m_pDocument;
}
#endif //_DEBUG


// CBCGtestView message handlers

LRESULT CBCGtestView::OnPrintClient(WPARAM wp, LPARAM lp)
{
	if ((lp & PRF_CLIENT) == PRF_CLIENT)
	{
		CDC* pDC = CDC::FromHandle((HDC) wp);
		ASSERT_VALID(pDC);
		
		OnDraw(pDC);
	}
	
	return 0;
}

