
// AsqareDlg.h : ͷ�ļ�
//

#pragma once
class CBricks;
class CMyMCIWave;

// CAsqareDlg �Ի���
class CAsqareDlg : public CDialogEx
{
// ����
public:
	CAsqareDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ASQARE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


    // ʵ��
private:
    void initialCtlPos();

protected:
	HICON m_hIcon;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClkButon(UINT nID);
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBegin();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnClose();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnBnClickedHint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedRearrange();
    afx_msg void OnBnClickedAbout();
    afx_msg void OnBnClickedBackgroundSound();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    HRESULT OnMciNotify(WPARAM wparam,LPARAM lParam); //��������ѭ������
private:
	CBitmap m_bmpBackground;
	CBitmapButton/*CButton*/ m_ButonArray[10][10];
	BOOL m_bAlowResize;
	BOOL m_bCheckedSomeone;
	CBricks *m_pMyBricks;
    UINT m_iHitTimes;
    CMyMCIWave *m_pMyMCIWave;
};
