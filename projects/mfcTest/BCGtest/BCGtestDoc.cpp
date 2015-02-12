// BCGtestDoc.cpp : implementation of the CBCGtestDoc class
//

#include "stdafx.h"
#include "BCGtest.h"

#include "BCGtestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBCGtestDoc

IMPLEMENT_DYNCREATE(CBCGtestDoc, CDocument)

BEGIN_MESSAGE_MAP(CBCGtestDoc, CDocument)
END_MESSAGE_MAP()


// CBCGtestDoc construction/destruction

CBCGtestDoc::CBCGtestDoc()
{
	// TODO: add one-time construction code here

}

CBCGtestDoc::~CBCGtestDoc()
{
}

BOOL CBCGtestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CBCGtestDoc serialization

void CBCGtestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CBCGtestDoc diagnostics

#ifdef _DEBUG
void CBCGtestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBCGtestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBCGtestDoc commands
