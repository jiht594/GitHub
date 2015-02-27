
// AsqareDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Asqare.h"
#include "AsqareDlg.h"
#include "afxdialogex.h"
#include "DEFFFFFF.h"
#include "Bricks.h"
#include "LogUtility.h"
#include "MyMCIWave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CAsqareDlg 对话框




CAsqareDlg::CAsqareDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAsqareDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAsqareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAsqareDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BT_BEGIN, &CAsqareDlg::OnBnClickedBegin)
	ON_WM_SIZE()
	ON_COMMAND_RANGE(IDC_BTN_SINGLEPIN_0, IDC_BTN_SINGLEPIN_END - 1, OnClkButon)
	ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BT_HINT, &CAsqareDlg::OnBnClickedHint)
    ON_WM_ERASEBKGND()
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BT_REARRANGE, &CAsqareDlg::OnBnClickedRearrange)
    ON_BN_CLICKED(IDC_BT_ABOUT, &CAsqareDlg::OnBnClickedAbout)
    ON_BN_CLICKED(IDC_CK_BG_SOUND, &CAsqareDlg::OnBnClickedBackgroundSound)
    ON_MESSAGE(MM_MCINOTIFY,OnMciNotify)
END_MESSAGE_MAP()


// CAsqareDlg 消息处理程序

BOOL CAsqareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_bAlowResize = FALSE;
	m_bCheckedSomeone = FALSE;
	m_pMyBricks = new CBricks();
    m_iHitTimes = 500;
    m_pMyMCIWave = new CMyMCIWave();
    CString strMusic;
    if (!strMusic.LoadString(IDS_SOUND_BG))
    {
        strMusic = "";
    }
    m_pMyMCIWave->OpenWave(strMusic);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAsqareDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAsqareDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

        //贴上主背景图片
		CBitmap bmp;
		bmp.LoadBitmap(IDB_BITMAP_MAINBG);

		// Get the size of the bitmap
		BITMAP bmpInfo;
		bmp.GetBitmap(&bmpInfo);
		// Create an in-memory DC compatible with the
		// display DC we're using to paint
		CDC dcMemory;
		dcMemory.CreateCompatibleDC(&dc);
		// Select the bitmap into the in-memory DC
		CBitmap *pOldBmp=dcMemory.SelectObject(&bmp);        
		//Get Client area
		CRect wndRt;
		this->GetClientRect(&wndRt);
		// Stretch and copy the bits from the in-memory DC into the on-
		// screen DC to actually do the painting
		dc.StretchBlt(0, 0, wndRt.Width(), wndRt.Height(), &dcMemory, 110, 50, wndRt.Width(), wndRt.Height(),/*bmpInfo.bmWidth, bmpInfo.bmHeight,*/ SRCCOPY);

		dcMemory.SelectObject(pOldBmp);
		// Do not call CDialog::OnPaint() for painting messages
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CAsqareDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



HBRUSH CAsqareDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

// 	// TODO:  在此更改 DC 的任何特性
// 	if   (nCtlColor   ==   CTLCOLOR_STATIC)     
// 	{     
// 		pDC->SetBkMode(TRANSPARENT);     
// 		hbr=(HBRUSH)GetStockObject(NULL_BRUSH);   
// 	}
// 	// TODO:  如果默认的不是所需画笔，则返回另一个画笔

    CRect rc;
    CDC* dc = this->GetDC();//这里获得父窗口dc

    switch(pWnd->GetDlgCtrlID()) 
    { 
    case IDC_BT_BEGIN:
    case IDC_BT_HINT:
    case IDC_BT_REARRANGE:
    case IDC_BT_ABOUT:
    case IDC_CK_BG_SOUND:
        //pDC->SetBkMode(TRANSPARENT); //字背景
        pWnd->GetWindowRect(&rc);
        ScreenToClient(&rc);
        //把父窗口背景图片先画到按钮上，作为4个按钮和1个checbox的背景
        //以此实现透明背景的效果
        pDC->BitBlt(0,0,rc.Width(),rc.Height(),dc,rc.left,rc.top,SRCCOPY);
        hbr = (HBRUSH)GetStockObject(NULL_BRUSH); 
        break; 
    default: 
        break; 
    }
    ReleaseDC(dc);
	return hbr;
}


void CAsqareDlg::OnBnClickedBegin()
{
    if (m_pMyBricks)
    {
        m_pMyBricks->startGame(m_hWnd);
        m_pMyMCIWave->m_bRepeat = TRUE;//循环播放背景音乐
        //m_pMyMCIWave->PlayWave();
        initialCtlPos();
    }
    else
    {
        MessageBox(_T("初始化失败。"), NULL, MB_OK);
    }
}


void CAsqareDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}
void CAsqareDlg::OnClkButon(UINT nID)
{
    //nID为按钮ID
    //setCheck选中一个按钮
    //再次setCheck时，判断与上一个按钮是否可以消除
	UINT ID = nID;
	m_pMyBricks->setCheck(ID);
}

void CAsqareDlg::OnClose()
{
    //关闭游戏时，清理资源
	if (m_pMyBricks)
    {
        delete m_pMyBricks;
        m_pMyBricks = NULL;
	}
    if (m_pMyMCIWave)
    {
        m_pMyMCIWave->CloseWave();
        delete m_pMyMCIWave;
        m_pMyMCIWave = NULL;
    }
	CDialogEx::OnClose();
}


BOOL CAsqareDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::PreTranslateMessage(pMsg);
}

//提示按钮
void CAsqareDlg::OnBnClickedHint()
{
    //SetTimer(1, 2000, NULL);
    if (m_pMyBricks->autoCheck())
    {
        m_iHitTimes--;//提示次数减一
        CString strHint = _T("提示 (");
        TCHAR hitTimes[5] = {0};
        _itot_s(m_iHitTimes, hitTimes, 10);
        strHint.Append(hitTimes);
        strHint.Append(_T(")"));
        GetDlgItem(IDC_BT_HINT)->SetWindowText(strHint);//更新提示按钮上的文字（次数）
        //提示次数没有时，禁用该按钮
        if (m_iHitTimes > 0)
        {
            GetDlgItem(IDC_BT_HINT)->EnableWindow(TRUE);
        }
        else
        {
            GetDlgItem(IDC_BT_HINT)->EnableWindow(FALSE);
        }
    }
}


BOOL CAsqareDlg::OnEraseBkgnd(CDC* pDC)
{
    UNREFERENCED_PARAMETER(pDC);
    return TRUE;//CDialogEx::OnEraseBkgnd(pDC);
}


void CAsqareDlg::OnTimer(UINT_PTR nIDEvent)
{
    switch (nIDEvent)
    {
    case 1:
        //无论是否检测到可消除按钮，都停止timer
        ::KillTimer(m_hWnd, 1);
        //设置后台自动检测的标志位
        m_pMyBricks->setChecking(true);
        //如果没检测到可消除按钮，提示对话框，并重新排列按钮
        if (!m_pMyBricks->autoCheck())
        {
            LogUtility::showMessageInConsole(_T("no connection\n"));
            if (!m_pMyBricks->completed())
            {
                MessageBox(_T("没有能连的了！\n"));
                m_pMyBricks->rearrange();
            }
            else
            {
                MessageBox(_T("完成游戏！\n"));
            }
        }
        //如果已完成本局游戏，重新使“开始按钮”可用
        if (m_pMyBricks->completed())
        {
            GetDlgItem(IDC_BT_BEGIN)->EnableWindow(TRUE);
            GetDlgItem(IDC_BT_HINT)->EnableWindow(FALSE);
            GetDlgItem(IDC_BT_REARRANGE)->EnableWindow(FALSE);
        }
        //恢复标志位
        m_pMyBricks->setChecking(false);
        break;
    default:
        break;
    }
    CDialogEx::OnTimer(nIDEvent);
}


void CAsqareDlg::OnBnClickedRearrange()
{
    //重新排列按钮
    if (m_pMyBricks)
    {
        SetTimer(1, 1000, NULL);
        m_pMyBricks->rearrange();
    }
}

//关于按钮
void CAsqareDlg::OnBnClickedAbout()
{
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
}


void CAsqareDlg::OnBnClickedBackgroundSound()
{
    //通过checkbox是否选中来决定是否播放背景音乐
    //m_bPlaying为true:正在播放音乐，false：没在播放音乐
    if (BST_CHECKED == IsDlgButtonChecked(IDC_CK_BG_SOUND))
    {
        if (!m_pMyMCIWave->m_bPlaying)
        {
            m_pMyMCIWave->PlayWave();
        }
    }
    else
    {
        if (m_pMyMCIWave->m_bPlaying)
        {
            m_pMyMCIWave->StopWave();
        }
    }
}

void CAsqareDlg::initialCtlPos()
{
    GetDlgItem(IDC_BT_BEGIN)->EnableWindow(FALSE); 
    GetDlgItem(IDC_BT_BEGIN)->SetWindowPos(NULL, 440, 260, 80, 25, SWP_NOZORDER);
    GetDlgItem(IDC_BT_HINT)->SetWindowPos(NULL, 440, 300, 80, 25, SWP_NOZORDER | SWP_SHOWWINDOW);

    //初始化开始按钮，提示按钮和checkbox等控件
    if (m_pMyBricks->completed())
    {
        m_iHitTimes = 500;
    }
    CString strHit = _T("提示 (");
    TCHAR hitTimes[5] = {0};
    _itot_s(m_iHitTimes, hitTimes, 10);
    strHit.Append(hitTimes);
    strHit.Append(_T(")"));
    GetDlgItem(IDC_BT_HINT)->SetWindowText(strHit);
    if (m_iHitTimes > 0)
    {
        GetDlgItem(IDC_BT_HINT)->EnableWindow(TRUE);
    }
    GetDlgItem(IDC_BT_REARRANGE)->EnableWindow(TRUE);
    GetDlgItem(IDC_BT_REARRANGE)->SetWindowPos(NULL, 440, 340, 80, 25, SWP_NOZORDER | SWP_SHOWWINDOW);
    GetDlgItem(IDC_BT_ABOUT)->SetWindowPos(NULL, 440, 380, 80, 25, SWP_NOZORDER);
    GetDlgItem(IDC_CK_BG_SOUND)->SetWindowPos(NULL, 440, 420, 80, 25, SWP_NOZORDER | SWP_SHOWWINDOW);
    //     CButton *pCheckbox = (CButton *)GetDlgItem(IDC_CK_BG_SOUND);
    //     pCheckbox->SetCheck(1);	
}


//m_pMyMCIWave->PlayWave()中设置的回调函数
//用来背景音乐循环播放
HRESULT CAsqareDlg::OnMciNotify(WPARAM wparam,LPARAM lParam) 
{     
    UNREFERENCED_PARAMETER(lParam);
    switch (wparam) 
    { 
    case MCI_NOTIFY_SUCCESSFUL://音乐播放完毕
        {   
            if (m_pMyMCIWave->m_bRepeat)
            {
                m_pMyMCIWave->PlayWave();//再放一遍
            }
        }
        break;  
    default:
        break; 
    }
    return TRUE;
} 