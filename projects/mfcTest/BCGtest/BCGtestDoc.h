// BCGtestDoc.h : interface of the CBCGtestDoc class
//


#pragma once


class CBCGtestDoc : public CDocument
{
protected: // create from serialization only
	CBCGtestDoc();
	DECLARE_DYNCREATE(CBCGtestDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CBCGtestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


