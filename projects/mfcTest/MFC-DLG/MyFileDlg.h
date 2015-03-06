#pragma once


// MyFileDlg

class CSpecialFileDialog : public CFileDialog
{
 DECLARE_DYNAMIC(CSpecialFileDialog)

public:
 CString m_strFilename;
 BOOL m_bDeleteAll;

// Construction
public:
 CSpecialFileDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
  LPCTSTR lpszDefExt = NULL,
  LPCTSTR lpszFileName = NULL,
  DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
  LPCTSTR lpszFilter = NULL,
  CWnd* pParentWnd = NULL);
 virtual ~CSpecialFileDialog();

// Dialog Data
 //enum { IDD = IDD_FILESPECIAL };

protected:
 virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

 DECLARE_MESSAGE_MAP()
public:
 virtual INT_PTR DoModal();
};