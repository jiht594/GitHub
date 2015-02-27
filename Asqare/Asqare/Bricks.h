#pragma once
#include "DEFFFFFF.h"
#include "NoFalshButton.h"
#include <set>

struct tagMainData;

class CBricks
{
public:
	CBricks(void);
	~CBricks(void);

    //����4��������Ҫ���ĸ�����
	BOOL startGame(HWND hWnd/*������:����,ͼ������, ͼƬ��С*/);//��ʼ����ť
	void setCheck(UINT ID);//��������ж��Ƿ������

    //�Զ�����Ƿ��п��������ġ��Զ������2�����
    //��һ�֣���̨timer��ʱ����Ƿ��п��������İ�ť�ԣ����û�����Զ��������а�ť
    //�ڶ��֣�����ֶ��������ʾ��ť�������п��������İ�ť��ʱ������2����ť�ϻ��Ϻ�ɫ��������ʾ���
    //2�����ʹ��m_bChecking��������ǣ�getChecking()��setChecking()���Ի�ȡ�������������
    bool autoCheck();
    BOOL rearrange();//��������ͼ��;

    bool getChecking() const { return m_bChecking; }
    void setChecking(bool val) { m_bChecking = val; }
    bool completed() const { return m_bCompleted; }
private:
    //�㷨�������Ƿ��������2����ť
    BOOL eliminate(const int x, const int y, const int x2, const int y2);
    //�ֱ���ֱ�ߣ�1���սǺ�2���ս�����������
    BOOL noRightAngle(BOOL bXOrY, int iValueX, int iValueY);
    BOOL oneRightAngle(int x, int y, const int m_x, const int m_y);
    BOOL oneRightAngleAfterTransform( int iPosX, int iPosY, int x2, int y2 );
    //��noRightAngle���ƣ������ǲ�����ͬ������x1y1�Ǳ��������ģ�Dispel==true����
    bool isEmptyLine( int x1, int y1, int x2, int y2 );

    //��ʼ�����а�ť
    void initialButtons(HWND);
    //����������а�ť
    void randomRange(bool bShowAll = true);
    //ȥ������ʾ���ܡ����ϵ�2����ɫ����
    void resetButtons();

    //2����־����
    void writeLog( int, int, const int, const int, const int, int iPosY, const int, const int );
    void writeLog( int x, int y, int , int );

    //��¼����·�����ɹ�����սǴ�������û��ʹ�ã�
    void insertPathPoint( int x, int y, const int m_x, const int m_y, const int , int , const int , const int  );
    void emptyPathMap();
private:
	pMainData MyMain; //��Ҫ���ݽṹ���������а�ť ͼƬ�ȵ�
	int m_IDBeChecked; //ѡ�еİ�ťID����һ�ε���İ�ť�����id�Ƚϣ����ж��Ƿ����������
    int m_IDBeChecked2222;//ѡ�еİ�ťID�����ֶ����ʱʹ��
    //��һ��ѡ�еİ�ť����
	int m_iCheckedPosX;
	int m_iCheckedPosY;
    //��ʱʹ�õ�����
	int m_iTempPosX;
	int m_iTempPosY;
	CNoFalshButton/*CBitmapButton*//*CButton*/ m_ButonArray[10][10]; //ȫ����ť�� ��ΪMyMain��һ���֡�
	HWND m_hwnd;//���ھ��
    bool m_bAutoCheck;//�Ƿ����Զ��жϣ������������ʾ��ť���͡���ʱ����Ƿ��п�������һ�ԡ�������������������ֶ����
    //��3���Ǽ��2�ιս�ʱ�õı�����
    bool m_bTransfom;
    bool m_Xtransform;
    bool m_Ytransform;
    bool m_bChecking;//�Ƿ��Ƕ�ʱ����Ƿ��п�������һ�ԡ�
    std::set< int> m_path;//��¼��ͨ·���������ж�һ�ιսǣ���û�õ���
    bool m_bCompleted;//�Ƿ�����˱�����Ϸ
};

