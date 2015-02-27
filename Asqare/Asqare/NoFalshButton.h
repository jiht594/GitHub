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
    bool m_bDrawRect;//使用“提示”功能时，值为真，将在OnPaint中画出可以消除的2个按钮
    virtual void PreSubclassWindow();
};


