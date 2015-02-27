#pragma once


// CNoFalshButton

class CNoFalshButton : public CBitmapButton
{
	DECLARE_DYNAMIC(CNoFalshButton)

public:
	CNoFalshButton();
	virtual ~CNoFalshButton();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    bool getbDrawRect() const { return m_bDrawRect; }
    void setbDrawRect(bool val) { m_bDrawRect = val; }
protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();
private:
    bool m_bDrawRect;//ʹ�á���ʾ������ʱ��ֵΪ�棬����OnPaint�л�������������2����ť
    virtual void PreSubclassWindow();
};


