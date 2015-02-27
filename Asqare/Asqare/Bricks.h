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

    //下面4个是最重要的四个函数
	BOOL startGame(HWND hWnd/*带参数:行列,图样个数, 图片大小*/);//初始化按钮
	void setCheck(UINT ID);//鼠标点击后，判断是否可消除

    //自动检测是否有可以消除的。自动检测有2中情况
    //第一种，后台timer定时检测是否还有可以消除的按钮对，如果没有则自动重新排列按钮
    //第二种，玩家手动点击“提示按钮”，则有可以消除的按钮对时，在这2个按钮上画上红色方框来提示玩家
    //2中情况使用m_bChecking变量来标记，getChecking()和setChecking()可以获取和设置这个变量
    bool autoCheck();
    BOOL rearrange();//重新排列图标;

    bool getChecking() const { return m_bChecking; }
    void setChecking(bool val) { m_bChecking = val; }
    bool completed() const { return m_bCompleted; }
private:
    //算法函数：是否可能消除2个按钮
    BOOL eliminate(const int x, const int y, const int x2, const int y2);
    //分别是直线，1个拐角和2个拐角相连可消除
    BOOL noRightAngle(BOOL bXOrY, int iValueX, int iValueY);
    BOOL oneRightAngle(int x, int y, const int m_x, const int m_y);
    BOOL oneRightAngleAfterTransform( int iPosX, int iPosY, int x2, int y2 );
    //与noRightAngle类似，不过是参数不同，并且x1y1是被消除过的（Dispel==true）的
    bool isEmptyLine( int x1, int y1, int x2, int y2 );

    //初始化所有按钮
    void initialButtons(HWND);
    //随机排列所有按钮
    void randomRange(bool bShowAll = true);
    //去除“提示功能“画上的2个红色方框
    void resetButtons();

    //2个日志函数
    void writeLog( int, int, const int, const int, const int, int iPosY, const int, const int );
    void writeLog( int x, int y, int , int );

    //记录连接路径。可供计算拐角次数。（没有使用）
    void insertPathPoint( int x, int y, const int m_x, const int m_y, const int , int , const int , const int  );
    void emptyPathMap();
private:
	pMainData MyMain; //主要数据结构，包括所有按钮 图片等等
	int m_IDBeChecked; //选中的按钮ID（下一次点击的按钮与这个id比较，来判断是否可以消除）
    int m_IDBeChecked2222;//选中的按钮ID，非手动检测时使用
    //上一次选中的按钮坐标
	int m_iCheckedPosX;
	int m_iCheckedPosY;
    //临时使用的坐标
	int m_iTempPosX;
	int m_iTempPosY;
	CNoFalshButton/*CBitmapButton*//*CButton*/ m_ButonArray[10][10]; //全部按钮， 作为MyMain的一部分。
	HWND m_hwnd;//窗口句柄
    bool m_bAutoCheck;//是否是自动判断（‘包括点击提示按钮’和‘定时检测是否还有可消除的一对’两种情况），区别于手动点击
    //这3个是检测2次拐角时用的变量。
    bool m_bTransfom;
    bool m_Xtransform;
    bool m_Ytransform;
    bool m_bChecking;//是否是定时检测是否还有可消除的一对’
    std::set< int> m_path;//记录联通路径（用来判断一次拐角），没用到。
    bool m_bCompleted;//是否完成了本次游戏
};

