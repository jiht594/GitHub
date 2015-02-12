// BCGtestView.h : interface of the CBCGtestView class
//


#pragma once


class CBCGtestView : public CView
{
protected: // create from serialization only
	CBCGtestView();
	DECLARE_DYNCREATE(CBCGtestView)

// Attributes
public:
	CBCGtestDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CBCGtestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg LRESULT OnPrintClient(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in BCGtestView.cpp
inline CBCGtestDoc* CBCGtestView::GetDocument() const
   { return reinterpret_cast<CBCGtestDoc*>(m_pDocument); }
#endif

