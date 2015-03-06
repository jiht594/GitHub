// MyFileDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MFC-DLG.h"
#include "MyFileDialog.h"


// CMyFileDialog

IMPLEMENT_DYNAMIC(CMyFileDialog, CFileDialog)

CMyFileDialog::CMyFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{

}

CMyFileDialog::~CMyFileDialog()
{
}


BEGIN_MESSAGE_MAP(CMyFileDialog, CFileDialog)
END_MESSAGE_MAP()



// CMyFileDialog message handlers




INT_PTR CMyFileDialog::DoModal()
{
	// TODO: Add your specialized code here and/or call the base class
	//m_hWnd = this->GetSafeHwnd();
	//this->SetControlText(IDOK, _T("Delete"));
	CWnd *pwin = this->FindWindow(_T("Button"), NULL/* _T("打开(O)")*/);
	pwin->SetWindowText(_T("斯蒂芬但是"));
	return CFileDialog::DoModal();
}
