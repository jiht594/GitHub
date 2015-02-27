
// MFC-DLGDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC-DLG.h"
#include "MFC-DLGDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCDLGDlg dialog




CMFCDLGDlg::CMFCDLGDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCDLGDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCDLGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCDLGDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CMFCDLGDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMFCDLGDlg message handlers

BOOL CMFCDLGDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_mouthcalctrl.Create(WS_TABSTOP | WS_CHILD | WS_BORDER | MCS_SHORTDAYSOFWEEK | MCS_NOTRAILINGDATES | MCS_NOTODAY | MCS_WEEKNUMBERS, CPoint(2, 2), this, 10000);

	DWORD dw = 0;
	dw = m_mouthcalctrl.SetColor(MCSC_MONTHBK, 0xFF0000);
	dw = m_mouthcalctrl.SetColor(MCSC_TITLETEXT, 0xFF0000);
	dw = m_mouthcalctrl.SetColor(MCSC_BACKGROUND, 0xFF0000);
	MonthCal_SetColor(m_mouthcalctrl.GetSafeHwnd(), MCSC_MONTHBK, 0xFF0000);
	m_mouthcalctrl.ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCDLGDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCDLGDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCDLGDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCDLGDlg::OnBnClickedButton1()
{
    CString cmdLine = _T("cmd.exe dir");
    SECURITY_ATTRIBUTES sa={sizeof ( sa ),NULL,TRUE};  
    SECURITY_ATTRIBUTES *psa=NULL;  
    DWORD dwShareMode=FILE_SHARE_READ|FILE_SHARE_WRITE;  
    OSVERSIONINFO osVersion={0};  
    osVersion.dwOSVersionInfoSize =sizeof ( osVersion );  
    if ( GetVersionEx ( &osVersion ) )  
    {  
        if ( osVersion.dwPlatformId ==VER_PLATFORM_WIN32_NT )  
        {  
            psa=&sa;  
            dwShareMode|=FILE_SHARE_DELETE;  
        }  
    }  
    //根据版本设置共享模式和安全属性  
    HANDLE hConsoleRedirect=CreateFile (  
        _T("D:\\1s.txt"),  
        GENERIC_WRITE,  
        dwShareMode,  
        psa,  
        OPEN_ALWAYS,  
        FILE_ATTRIBUTE_NORMAL,  
        NULL );  
    ASSERT ( hConsoleRedirect!=INVALID_HANDLE_VALUE );  
    STARTUPINFO s={sizeof ( s ) };  
    s.dwFlags =STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;  
    //使用标准柄和显示窗口  
    s.hStdOutput =hConsoleRedirect;//将文件作为标准输出句柄  
    s.wShowWindow =SW_SHOW;//隐藏控制台窗口  
    PROCESS_INFORMATION pi={0};  
    if ( CreateProcess ( NULL,cmdLine.GetBuffer(),NULL,NULL,TRUE,CREATE_NEW_CONSOLE,NULL,NULL,&s,&pi ) )  
    {  
        //创建进程,执行Ping程序,测试网络是否连通  
        WaitForSingleObject ( pi.hProcess ,INFINITE );  
        //等待进程执行完毕  
        CloseHandle ( pi.hProcess );  
        CloseHandle ( pi.hThread );  
        //关闭进程和主线程句柄  
    }  
    cmdLine.ReleaseBuffer();
    CloseHandle ( hConsoleRedirect );  
    //关闭控制台定向输出文件句柄  
    CFile myFile ( _T("D:\\1.txt"),CFile::modeRead );  
    ASSERT ( myFile.m_hFile!=NULL );  
    TCHAR * pszNetStatus=new TCHAR[myFile.GetLength () +1];  
    ZeroMemory ( pszNetStatus,myFile.GetLength () +1 );  
    myFile.Read ( pszNetStatus,myFile.GetLength () );  
    myFile.Close ();  

    MessageBox(pszNetStatus);
}
