
// AsqareDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CAsqareDlg �Ի���




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


// CAsqareDlg ��Ϣ�������

BOOL CAsqareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAsqareDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

        //����������ͼƬ
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CAsqareDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



HBRUSH CAsqareDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

// 	// TODO:  �ڴ˸��� DC ���κ�����
// 	if   (nCtlColor   ==   CTLCOLOR_STATIC)     
// 	{     
// 		pDC->SetBkMode(TRANSPARENT);     
// 		hbr=(HBRUSH)GetStockObject(NULL_BRUSH);   
// 	}
// 	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������

    CRect rc;
    CDC* dc = this->GetDC();//�����ø�����dc

    switch(pWnd->GetDlgCtrlID()) 
    { 
    case IDC_BT_BEGIN:
    case IDC_BT_HINT:
    case IDC_BT_REARRANGE:
    case IDC_BT_ABOUT:
    case IDC_CK_BG_SOUND:
        //pDC->SetBkMode(TRANSPARENT); //�ֱ���
        pWnd->GetWindowRect(&rc);
        ScreenToClient(&rc);
        //�Ѹ����ڱ���ͼƬ�Ȼ�����ť�ϣ���Ϊ4����ť��1��checbox�ı���
        //�Դ�ʵ��͸��������Ч��
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
        m_pMyMCIWave->m_bRepeat = TRUE;//ѭ�����ű�������
        //m_pMyMCIWave->PlayWave();
        initialCtlPos();
    }
    else
    {
        MessageBox(_T("��ʼ��ʧ�ܡ�"), NULL, MB_OK);
    }
}


void CAsqareDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}
void CAsqareDlg::OnClkButon(UINT nID)
{
    //nIDΪ��ťID
    //setCheckѡ��һ����ť
    //�ٴ�setCheckʱ���ж�����һ����ť�Ƿ��������
	UINT ID = nID;
	m_pMyBricks->setCheck(ID);
}

void CAsqareDlg::OnClose()
{
    //�ر���Ϸʱ��������Դ
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
	// TODO: �ڴ����ר�ô����/����û���

	return CDialogEx::PreTranslateMessage(pMsg);
}

//��ʾ��ť
void CAsqareDlg::OnBnClickedHint()
{
    //SetTimer(1, 2000, NULL);
    if (m_pMyBricks->autoCheck())
    {
        m_iHitTimes--;//��ʾ������һ
        CString strHint = _T("��ʾ (");
        TCHAR hitTimes[5] = {0};
        _itot_s(m_iHitTimes, hitTimes, 10);
        strHint.Append(hitTimes);
        strHint.Append(_T(")"));
        GetDlgItem(IDC_BT_HINT)->SetWindowText(strHint);//������ʾ��ť�ϵ����֣�������
        //��ʾ����û��ʱ�����øð�ť
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
        //�����Ƿ��⵽��������ť����ֹͣtimer
        ::KillTimer(m_hWnd, 1);
        //���ú�̨�Զ����ı�־λ
        m_pMyBricks->setChecking(true);
        //���û��⵽��������ť����ʾ�Ի��򣬲��������а�ť
        if (!m_pMyBricks->autoCheck())
        {
            LogUtility::showMessageInConsole(_T("no connection\n"));
            if (!m_pMyBricks->completed())
            {
                MessageBox(_T("û���������ˣ�\n"));
                m_pMyBricks->rearrange();
            }
            else
            {
                MessageBox(_T("�����Ϸ��\n"));
            }
        }
        //�������ɱ�����Ϸ������ʹ����ʼ��ť������
        if (m_pMyBricks->completed())
        {
            GetDlgItem(IDC_BT_BEGIN)->EnableWindow(TRUE);
            GetDlgItem(IDC_BT_HINT)->EnableWindow(FALSE);
            GetDlgItem(IDC_BT_REARRANGE)->EnableWindow(FALSE);
        }
        //�ָ���־λ
        m_pMyBricks->setChecking(false);
        break;
    default:
        break;
    }
    CDialogEx::OnTimer(nIDEvent);
}


void CAsqareDlg::OnBnClickedRearrange()
{
    //�������а�ť
    if (m_pMyBricks)
    {
        SetTimer(1, 1000, NULL);
        m_pMyBricks->rearrange();
    }
}

//���ڰ�ť
void CAsqareDlg::OnBnClickedAbout()
{
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
}


void CAsqareDlg::OnBnClickedBackgroundSound()
{
    //ͨ��checkbox�Ƿ�ѡ���������Ƿ񲥷ű�������
    //m_bPlayingΪtrue:���ڲ������֣�false��û�ڲ�������
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

    //��ʼ����ʼ��ť����ʾ��ť��checkbox�ȿؼ�
    if (m_pMyBricks->completed())
    {
        m_iHitTimes = 500;
    }
    CString strHit = _T("��ʾ (");
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


//m_pMyMCIWave->PlayWave()�����õĻص�����
//������������ѭ������
HRESULT CAsqareDlg::OnMciNotify(WPARAM wparam,LPARAM lParam) 
{     
    UNREFERENCED_PARAMETER(lParam);
    switch (wparam) 
    { 
    case MCI_NOTIFY_SUCCESSFUL://���ֲ������
        {   
            if (m_pMyMCIWave->m_bRepeat)
            {
                m_pMyMCIWave->PlayWave();//�ٷ�һ��
            }
        }
        break;  
    default:
        break; 
    }
    return TRUE;
} 