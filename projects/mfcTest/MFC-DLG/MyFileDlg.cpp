// MyFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC-DLG.h"
#include "MyFileDlg.h"


// MyFileDlg

IMPLEMENT_DYNAMIC(CSpecialFileDialog, CFileDialog)
	CSpecialFileDialog::CSpecialFileDialog(BOOL bOpenFileDialog,
	LPCTSTR lpszDefExt, LPCTSTR lpszFileName, DWORD dwFlags,
	LPCTSTR lpszFilter, CWnd* pParentWnd)
	: CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName,
	dwFlags, lpszFilter, pParentWnd)
{
	m_ofn.Flags |= OFN_ENABLETEMPLATE;
	m_ofn.lpTemplateName = _T("werwe");
	m_ofn.lpstrTitle = _T("Delete File");
	m_bDeleteAll = FALSE;
	m_bVistaStyle = FALSE;
}

CSpecialFileDialog::~CSpecialFileDialog()
{
}

void CSpecialFileDialog::DoDataExchange(CDataExchange* pDX)
{
	CFileDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSpecialFileDialog, CFileDialog)
	//ON_BN_CLICKED(IDC_DELETE, OnBnClickedDelete)
END_MESSAGE_MAP()


INT_PTR CSpecialFileDialog::DoModal()
{
	// TODO: Add your specialized code here and/or call the base class
	//OnInitDone();
	//m_hWnd = this->GetSafeHwnd();
	//this->SetControlText(IDOK, _T("Delete"));
	//this->FindWindowW
	return CFileDialog::DoModal();
}
