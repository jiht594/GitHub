#include "StdAfx.h"
#include "Bricks.h"
#include "resource.h"
#include "afxdialogex.h"
#include <time.h>
#include "LogUtility.h"
#include <algorithm>

CBricks::CBricks(void)
{
    LogUtility::openConsole();
    MyMain = new MainData[100];
    m_IDBeChecked = -1;
    m_IDBeChecked2222 = -1;
    m_hwnd = NULL;
    m_iCheckedPosX = -1;
    m_iCheckedPosY = -1;
    m_iTempPosX = -1;
    m_iTempPosY = -1;
    m_bAutoCheck = false;
    m_bChecking = false;
    m_bCompleted = false;
}


CBricks::~CBricks(void)
{
    LogUtility::closeConsole();
    delete[] MyMain;
}
BOOL CBricks::startGame(HWND hwnd)
{

    if (m_bCompleted)
    {
        //已经完成一局游戏，开始新的一局
        rearrange();
        m_bCompleted = false;
    }
    else
    {
        //第一次游戏，初始化按钮
        initialButtons(hwnd);
    }
    
    //随机重新排列按钮
    randomRange();

    //AfxBeginThread(MyThreadProc, MyMain);
    //设置自动检测“是否还有可消除按钮”的计时器
    SetTimer(m_hwnd, 1, 1000, NULL);
    return TRUE;
}
void CBricks::randomRange(bool bShowAll)
{
    UNREFERENCED_PARAMETER(bShowAll);
    srand(clock());
    int iRandom = 0;
    int iPossiton = 0;
    int jPossiton = 0;
    int iTempBMPID;
    BOOL bTempDispel;
    BOOL bTempChecked;
    for (int i = 0; i < 100; i++)
    {
        //遍历所有按钮，和另一个随机按钮交换
        iRandom = rand()%100;
        iPossiton = iRandom / 10;
        jPossiton = iRandom % 10;

        if (!m_bCompleted)
        {
            //未完成的情况：重排时忽略已消除的按钮
            if (MyMain[i].bDispel)
            {
                continue;
            }
            //随机按钮也需要忽略已消除的
            while (MyMain[iPossiton * 10 + jPossiton].bDispel)//效率低
            {
                iRandom = rand()%100;
                iPossiton = iRandom / 10;
                jPossiton = iRandom % 10;
            }
        }

        //交换2个按钮的图片
        bTempDispel = MyMain[i].bDispel;
        bTempChecked = MyMain[i].bCheck;
        iTempBMPID = MyMain[i].iBMPID1;

        MyMain[i].bDispel = MyMain[iPossiton * 10 + jPossiton].bDispel;
        MyMain[i].bCheck = MyMain[iPossiton * 10 + jPossiton].bCheck;
        MyMain[i].iBMPID1 = MyMain[iPossiton * 10 + jPossiton].iBMPID1;

        MyMain[iPossiton * 10 + jPossiton].bDispel = bTempDispel;
        MyMain[iPossiton * 10 + jPossiton].bCheck = bTempChecked;
        MyMain[iPossiton * 10 + jPossiton].iBMPID1 = iTempBMPID;

        MyMain[i].pBMPButton->LoadBitmaps(MyMain[i].iBMPID1);
        MyMain[iPossiton * 10 + jPossiton].pBMPButton->LoadBitmaps(MyMain[iPossiton * 10 + jPossiton].iBMPID1);
         //交换完毕

        if (m_bCompleted)
        {
            //如果已经完成了本局游戏，那么开始新一局
            
            //恢复标志位， 去除已经标记的提示框
            MyMain[iPossiton * 10 + jPossiton].bCheck = FALSE;
            MyMain[iPossiton * 10 + jPossiton].bDispel = FALSE;
            MyMain[iPossiton * 10 + jPossiton].pBMPButton->setbDrawRect(FALSE);

            MyMain[i].bCheck = FALSE;
            MyMain[i].bDispel = FALSE;
            MyMain[i].pBMPButton->setbDrawRect(FALSE);

            //重新显示所有按钮
            MyMain[iPossiton * 10 + jPossiton].pBMPButton->EnableWindow(TRUE);
            MyMain[iPossiton * 10 + jPossiton].pBMPButton->ShowWindow(SW_SHOW);
            MyMain[i].pBMPButton->EnableWindow(TRUE);
            MyMain[i].pBMPButton->ShowWindow(SW_SHOW);
        }

        //选中状态恢复到哦啊正常状态
        if (0 == MyMain[iPossiton * 10 + jPossiton].iBMPID1 % 2)
        {
            MyMain[iPossiton * 10 + jPossiton].pBMPButton->LoadBitmaps(--MyMain[iPossiton * 10 + jPossiton].iBMPID1);
        }

        if (0 == MyMain[i].iBMPID1 % 2)
        {
            MyMain[i].pBMPButton->LoadBitmaps(--MyMain[i].iBMPID1);
        }
    }
    //重排时（非重新开始），需要去除“提示”的一对红框
    if (!m_bCompleted)
    {
        resetButtons();
    }
    //新局开始，更新画面
    AfxGetApp()->GetMainWnd()->Invalidate(FALSE);
    UpdateWindow(m_hwnd);
}

void CBricks::setCheck(UINT ID)
{
    //去除“提示”功能画上的2个红色方框
    resetButtons();
    if (m_bAutoCheck)
    {
        //resetButtons();
        m_bAutoCheck = false;
    }

    UINT ButtonID = 0;//按钮id从2100开始 10×10个，
    if (ID >= 2100 && ID < 2200)
    {
        ButtonID = ID % 2100;
        for (int i = 0; i < 100; ++i)
        {
            if (ButtonID == (UINT)10 * MyMain[i].i + MyMain[i].j)
            {
                MyMain[i].bCheck = TRUE;
                //m_IDBeChecked = ButtonID;//10i+j
                if (!m_bChecking)
                {
                    //改变本次点击的按钮的状态
                    if (0 == MyMain[i].iBMPID1 % 2)
                    {
                        --MyMain[i].iBMPID1;
                    }
                    else
                    {
                        ++MyMain[i].iBMPID1;
                    }
                }
                else
                {
                    //test。上面的!m_bChecking判断可能是不需要的
                    //永远走不到这里
                    MessageBox(m_hwnd, _T("never show up"), _T("caption"), MB_OK);
                }
                

                //查看之前没有其他按钮是选中状态
                //是m_IDBeChecked！=-1：与本次选中的按钮比较
                if (m_IDBeChecked == -1)
                {
                    //以前没有选中按钮
                    MyMain[i].pBMPButton->LoadBitmaps(MyMain[i].iBMPID1);
                    m_iCheckedPosX = MyMain[i].i;
                    m_iCheckedPosY = MyMain[i].j;
                    m_IDBeChecked = i;
                    MyMain[i].pBMPButton->Invalidate(FALSE);
                }
                else
                {
                    //以前选中了一个按钮，与本次选中的比较，看是否可以消除
                    if (!eliminate(MyMain[i].i, MyMain[i].j, m_iCheckedPosX, m_iCheckedPosY))
                    {
                        //如果不能消除这一对按钮
                        //更新按钮的选中状态
                        if (m_IDBeChecked == i)
                        {
                            //本次与上次选中的是同一个按钮，设置按钮为正常状态
                            MyMain[i].pBMPButton->LoadBitmaps(MyMain[i].iBMPID1);
                            m_IDBeChecked = -1;
                        }
                        else
                        {
                            //不是同一个按钮，设置本次选中按钮为选中状态
                            MyMain[i].pBMPButton->LoadBitmaps(MyMain[i].iBMPID1);
                            if (0 == MyMain[i].iBMPID1 % 2)
                            {
                                --MyMain[m_IDBeChecked].iBMPID1;
                            }
                            MyMain[m_IDBeChecked].pBMPButton->LoadBitmaps(MyMain[m_IDBeChecked].iBMPID1);
                            InvalidateRect(m_hwnd, CRect(MyMain[m_IDBeChecked].iPos[0], MyMain[m_IDBeChecked].iPos[1], MyMain[m_IDBeChecked].iPos[0] + 40, MyMain[m_IDBeChecked].iPos[1] + 40), TRUE);
                            m_iCheckedPosX = MyMain[i].i;
                            m_iCheckedPosY = MyMain[i].j;
                            m_IDBeChecked = i;
                            //m_IDBeChecked2222 = m_IDBeChecked;
                        }

                        MyMain[i].pBMPButton->Invalidate(FALSE);
                    }
                    else
                    {
                        //可以显出这一对按钮时，禁用这2个按钮并隐藏
                        writeLog(MyMain[i].i, MyMain[i].j, m_iCheckedPosX, m_iCheckedPosY);
                        m_path.insert(MyMain[i].i * 10 + MyMain[i].j);
                        if (m_path.size() >= 4 && !m_bChecking)
                        {
                            //这里没有用，“万一”算法有问题时：可能需要m_path
                            //m_path是记录路径，可以计算拐角次数（没有完成）
                            //MessageBox(NULL, _T("> 4"), _T(""), 0);
                            //return;
                        }

                        //播放消除的声音
                        CString strSound;
                        if (strSound.LoadString(IDS_SOUND_ELI))
                        {
                            //PlaySound函数无法同时播放2个音乐
                            //就是说不能同时播放背景音乐和消除的声音
                            //所以背景音乐使用了MCI方式播放，MCI可以同时播放多个音乐文件
                            PlaySound(strSound, NULL, SND_ASYNC);
                        }
                        //消除这2个按钮
                        MyMain[i].pBMPButton->ShowWindow(SW_HIDE);
                        MyMain[i].pBMPButton->EnableWindow(FALSE);
                        MyMain[i].bDispel = TRUE;
                        MyMain[m_IDBeChecked].pBMPButton->ShowWindow(SW_HIDE);
                        MyMain[m_IDBeChecked].pBMPButton->EnableWindow(FALSE);
                        MyMain[m_IDBeChecked].bDispel = TRUE;
                        m_IDBeChecked = -1;
                        //每次消除一对按钮后，自动检测是否还有可消除的按钮对
                        SetTimer(m_hwnd, 1, 1000, NULL);
                    }
                }
            }
        }
    }
    //InvalidateRect(m_hwnd, NULL, TRUE);
}

//练练看消除算法，很渣
//可能会有错误
//需要2个拐角相连时，可能会有问题。如果有，需要m_path记录连接路线，然后计算有几个拐角
BOOL CBricks::eliminate(int x, int y, int x2, int y2)
{
    emptyPathMap();
    //这3个变量:当2个按钮不能一次拐角消除时，需要变换一个按钮的位置，检测
    m_bTransfom = false;//是否变换了位置， 这个目前没用到
    m_Xtransform = false;//改变了x方向
    m_Ytransform = false;//改变了y方向
    BOOL bRet = FALSE;
    int iPosX = x;
    int iPosY = y;
    m_iTempPosX = x2;
    m_iTempPosY = y2;

    //tempCheckId为上一次选中的图片，应该就是x2*10+y2
    int tempCheckId = m_bAutoCheck ? m_IDBeChecked2222 : m_IDBeChecked;
    int iBMPID1 = MyMain[10 * iPosX + iPosY].iBMPID1;
    int iBMPID2 = MyMain[tempCheckId].iBMPID1;
    int iBigBMP = iBMPID1 > iBMPID2 ? iBMPID1 : iBMPID2;
    int iSmallBMP = iBMPID1 < iBMPID2 ? iBMPID1 : iBMPID2;

    //bSamaBMPID：2个按钮是否是同一张图片
    bool bSamaBMPID = false;
    if ((iBMPID1 == iBMPID2) //相同图片ID
        || ((iBigBMP % 2 == 0) && (iBigBMP - iSmallBMP == 1)))//（正常与选中）2种状态的一对图片，也算是相同的
    {
        bSamaBMPID = true;
    }

    if ((10 * iPosX + iPosY != tempCheckId)
        && (bSamaBMPID))
    {
        if (((iPosX == 0) && (x2 == 0))
            ||((iPosY == 0) && (y2 == 0))
            ||((iPosX == 9) && (x2 == 9))
            ||((iPosY == 9) && (y2 == 9)))
        {
            //第1种情况：2个按钮都在边界上，且是同一侧的边界
            bRet = TRUE;
        }
        else if (iPosX == x2 || iPosY == y2)
        {
            //第2种情况：两个按钮在同一行或者同一列

            //先检测是否可以直接相连
            bRet = noRightAngle(iPosX == x2 ? TRUE : FALSE, iPosX, iPosY);
            if (bRet)
            {
            }
            else
            {
                //不能直连，检测一个拐角能否连通
                bRet = oneRightAngle(iPosX, iPosY, x2, y2);
                if (bRet)
                {
                }
                else
                {
                    //一个挂角也无法联通，需要检测2个拐角的情况
                    //也就是变换一个按钮的位置，再检测一个拐角
                    bRet = oneRightAngleAfterTransform(iPosX, iPosY, x2, y2);
                }
            }
        }
        else
        {
            //第3种情况：2个按钮不再一条线上，先判断是否一个拐角可联通
            bRet = oneRightAngle(iPosX, iPosY, x2, y2);
            if (!bRet)
            {
                //位置变换后, 再检测
                bRet = oneRightAngleAfterTransform(iPosX, iPosY, x2, y2);
            }
        }
    }
    return bRet;
}

//2个按钮是否可以直线消除
BOOL CBricks::noRightAngle(BOOL XOrY, int x, int y)
{
    //XOrY是方向，表示2个按钮时在同一行还是同一列
    BOOL bPassageway = TRUE;
    int iBigOne;
    int iSmallOne;
    if (XOrY)
    {
        iBigOne = m_iTempPosY > y ? m_iTempPosY : y;
        iSmallOne = m_iTempPosY > y ? y : m_iTempPosY;
        //判断：从small到big一排按钮，是否都是消除（dispel）过的
        for (int i = 1; i < iBigOne - iSmallOne; i++)
        {
            if (!MyMain[10 * x + iSmallOne + i].bDispel)
            {
                bPassageway = FALSE;
                break;
            }
        }
    }
    else
    {
        iBigOne = m_iTempPosX > x ? m_iTempPosX : x;
        iSmallOne = m_iTempPosX > x ? x : m_iTempPosX;
        if (iBigOne - iSmallOne != 1)
        {
            //判断：从small到big一排按钮，是否都是消除（dispel）过的
            for (int i = 1; i < iBigOne - iSmallOne; i++)
            {
                if (!MyMain[10 * (iSmallOne + i) + y].bDispel)
                {
                    bPassageway = FALSE;
                    break;
                }
            }
        }
    }
    return bPassageway;
}

//判断2个按钮可以通过一个拐角消除
BOOL CBricks::oneRightAngle(int x, int y, const int m_x, const int m_y)
{
    BOOL bRet = FALSE;
    int iPosX = x;
    int iPosY = y;
    m_iTempPosX = iPosX;
    m_iTempPosY = m_y;
    if (MyMain[10 * m_iTempPosX + m_iTempPosY].bDispel)
    {
        bRet = noRightAngle(TRUE, iPosX, iPosY) && noRightAngle(FALSE, m_x, m_y);
    }
    if (!bRet)
    {
        m_iTempPosX = m_x;
        m_iTempPosY = iPosY;
        if (MyMain[10 * m_iTempPosX + m_iTempPosY].bDispel)
        {
            bRet = noRightAngle(FALSE, iPosX, iPosY) && noRightAngle(TRUE, m_x, m_y);
        }
    }


    //下面是处理，变换后的情况

    //原理：（当一次拐角不能消除时：需要处理2次拐角的情况
    //假如需要测试A，B按钮是否可以消除
    //这里我们取一个新按钮C
    //当AC可以直接相连消除，并且BC可以通过一次拐角消除
    //说明AB可以2次拐角消除）

    //参考上面（）中的原理
    //本函数的功能是检测BC可以一次拐角相连
    //当B按钮是边界的时候，需要考虑在“边界外连线”消除的情况
    //在这里C并不是一个按钮，而是边界外的一个虚拟按钮。
    if (!bRet/* && !m_bTransfom*/)
    {
        if ((iPosY == 9 || iPosY == 0) && m_Ytransform && MyMain[10 * m_x + iPosY].bDispel)
        {
            bRet = isEmptyLine(m_x, iPosY, m_x, m_y);
            if (bRet)
            {
                writeLog(x, y, m_x, m_y, m_x, iPosY, m_x, m_y);
                insertPathPoint(x, y, m_x, m_y, m_x, iPosY, m_x, m_y);
            }
        }
        else if ((iPosX == 9 || iPosX == 0) && m_Xtransform  && MyMain[10 * iPosX + m_y].bDispel)
        {
            bRet = isEmptyLine(iPosX, m_y, m_x, m_y);
            if (bRet)
            {
                writeLog(x, y, m_x, m_y, iPosX, m_y, m_x, m_y);
                insertPathPoint(x, y, m_x, m_y, iPosX, m_y, m_x, m_y);
            }
        }
    }

    if (!bRet/* && !m_bTransfom*/)
    {
        if ((m_y == 9 || m_y == 0)  && m_Ytransform && MyMain[10 * iPosX + m_y].bDispel)
        {
            bRet = isEmptyLine(iPosX, m_y, iPosX, iPosY);
            if (bRet)
            {
                writeLog(x, y, m_x, m_y, iPosX, m_y, iPosX, iPosY);
                insertPathPoint(x, y, m_x, m_y, iPosX, m_y, iPosX, iPosY);
            }
        }
        else if ((m_x == 9 || m_x == 0) && m_Xtransform  && MyMain[10 * m_x + iPosY].bDispel)
        {
            bRet = isEmptyLine(m_x, iPosY, iPosX, iPosY);
            if (bRet)
            {
                writeLog(x, y, m_x, m_y, m_x, iPosY, iPosX, iPosY);
                insertPathPoint(x, y, m_x, m_y, m_x, iPosY, iPosX, iPosY);
            }
        }
    }
    return bRet;
}

//不需拐角就可以消除
// BOOL CBricks::noRightAngle(BOOL XOrY, int x, int y, int tempX, int tempY)
// {
//     //BOOL bRet = FALSE;
//     BOOL bPassageway = TRUE;
//     int iBigOne;
//     int iSmallOne;
//     if (x != tempX || y != tempY)
//     {
//         if (XOrY)
//         {
//             iBigOne = tempY > y ? tempY : y;
//             iSmallOne = tempY > y ? y : tempY;
//             for (int i = 1; i < iBigOne - iSmallOne; i++)
//             {
//                 if (!MyMain[10 * x + iSmallOne + i].bDispel)
//                 {
//                     bPassageway = FALSE;
//                     break;
//                 }
//             }
//         }
//         else
//         {
//             iBigOne = tempX > x ? tempX : x;
//             iSmallOne = tempX > x ? x : tempX;
//             for (int i = 1; i < iBigOne - iSmallOne; i++)
//             {
//                 if (!MyMain[10 * (iSmallOne + i) + y].bDispel)
//                 {
//                     bPassageway = FALSE;
//                     break;
//                 }
//             }
//         }
//     }
//     return bPassageway ? TRUE : FALSE;
// }

//2个按钮可以直接相连，与上面的noRightAngle函数类似
//不过这里第一个按钮x1y1是被消除过的Dispel为true
bool CBricks::isEmptyLine( int x1, int y1, int x2, int y2 )
{
    int xSmall = x1 > x2 ? x2 : x1;
    int xBig = x1 > x2 ? x1 : x2;
    int ySmall = y1 > y2 ? y2 : y1;
    int yBig = y1 > y2 ? y1 : y2;

    //     if (!MyMain[10 * x1 + y1].bDispel)
    //     {
    //         return false;
    //     }

    if (x1 == x2)
    {
        for (int j  = ySmall; j < yBig; ++j)
        {
            if (j == y2)
            {
                continue;
            }
            if (!MyMain[10 * x1 + j].bDispel)
            {
                return false;
            }
        }
    }
    else if (y1 == y2)
    {
        for (int i = xSmall; i < xBig; ++i)
        {
            if (i == x2)
            {
                continue;
            }
            if (!MyMain[10 * i + y1].bDispel)
            {
                return false;
            }
        }
    }

    return true;
}

//自动检测是否有可以消除的一对按钮
//如果是使用“提示按钮”触发autoCheck函数
//则在按钮上画上红色方框，来提示游戏者
bool CBricks::autoCheck()
{
    bool bRet = false;
    MainData md1;//遍历第一个按钮，再遍历md2，看是否有可以消除的
    MainData md2;//第二个按钮

    if (!m_bChecking)
    {
        //非后台检测时（OnTimer中），去除可消除提示的红框
        resetButtons();
    }
    m_bAutoCheck = true;//非手动
    bool bAllDispel = true;//按钮是否全部消除的标志位
    for (int i = 0; i < 100; i++)
    {
        if (bRet)
        {
            break;
        }
        md1 = MyMain[i];
        if (MyMain[i].bDispel)
        {
            //跳过消除的按钮
            continue;
        }
        else
        {
            //如果有按钮没被消除
            bAllDispel = false;
        }

        for (int j = i + 1; j < 100; j++)
        {
            md2 = MyMain[j];
            if (MyMain[j].bDispel)
            {
                //隐藏按钮跳过
                continue;
            }

            int iCheckButon = -1;//保存m_IDBeChecked2222，使用后恢复m_IDBeChecked2222的原来值
            if (m_IDBeChecked2222 != -1)
            {
                iCheckButon = m_IDBeChecked2222;
            }
            //m_IDBeChecked2222用来记录一个按钮，供Eliminate函数中使用---非手动（提示按钮 和 后台检测 2种情况）
            //m_IDBeChecked也是用来记录一个按钮，供Eliminate函数中使用---手动
            m_IDBeChecked2222 = j;

            if (eliminate(i/10, i%10, j/10, j%10))
            {
                //可以消除
                writeLog(i/10, i%10, j/10, j%10);
                m_path.insert(i);
                m_path.insert(j);
                if (m_path.size() >= 4)//连接路径（可用来判断几个拐角），没用到
                {
                    //MessageBox(NULL, _T("> 4"), _T(""), 0);
                    //continue;;
                }

                if (m_bChecking)
                {
                    //后台检测：不需要画出提示方框，break
                    //m_bAutoCheck = true;
                    KillTimer(m_hwnd, 1);
                    m_IDBeChecked2222 = iCheckButon;
                    bRet = true;
                    break;
                }
                else
                {
                    //“提示”按钮检测
                    //画出2个红色提示框
                    MyMain[i].pBMPButton->setbDrawRect(true);
                    MyMain[j].pBMPButton->setbDrawRect(true);
                    MyMain[i].pBMPButton->Invalidate();
                    MyMain[j].pBMPButton->Invalidate();
                    //更新
                    UpdateWindow(m_hwnd);
                }
                bRet = true;
                m_bAutoCheck = true;
                break;
            }
            else
            {
                m_IDBeChecked2222 = iCheckButon;
            }
        }
        if (bRet)
        {
            break;
        }
    }

    if (bAllDispel)
    {
        //所有按钮都隐藏了，说明本局游戏已经完成
        m_bCompleted = true;
    }
    //m_IDBeChecked2222 = -1;
    return bRet;
}

//去除“提示功能“画上的2个红色方框
void CBricks::resetButtons()
{
    for (int i = 0; i < 100; i++)
    {
        if (MyMain[i].pBMPButton->getbDrawRect())
        {
            //设置标志位
            MyMain[i].pBMPButton->setbDrawRect(false);
            //计算刷新区域
            CRect rect;
            MyMain[i].pBMPButton->GetWindowRect(&rect);
            rect.top -=2;
            rect.bottom += 3;
            rect.left -= 2;
            rect.right += 3;
            CWnd *pWnd = AfxGetApp()->GetMainWnd();
            pWnd->ScreenToClient(&rect);
            //更新画面
            pWnd->InvalidateRect(&rect);
            UpdateWindow(m_hwnd);
        }
    }
}

//日志函数
void CBricks::writeLog( int x, int y, const int m_x, const int m_y, const int tx, int ty, const int kx, const int ky )
{
    CString strMsg;
    strMsg.Format(_T("one angle A:%d-%d, B%d-%d, empty line C:%d-%d, D%d-%d")
        , x, y, m_x, m_y, tx, ty, kx, ky);
    LogUtility::showMessageInConsole(strMsg);
}
//日志函数
void CBricks::writeLog( int x, int y, int x2, int y2)
{
    CString strMsg;
    strMsg.Format(_T("orignal X:%d-%d, Y%d-%d")
        , x, y, x2, y2);
    LogUtility::showMessageInConsole(strMsg);
}
//记录连线路径。没有完成
void CBricks::insertPathPoint( int x1, int y1, const int x2, const int y2, const int x3, int y3, const int x4, const int y4 )
{
     m_path.insert(x1 * 10 + y1);
     m_path.insert(x2 * 10 + y2);
     m_path.insert(x3 * 10 + y3);
     m_path.insert(x4 * 10 + y4);
}
//清空路径
void CBricks::emptyPathMap()
{
    m_path.clear();
}
//重新排列按钮
BOOL CBricks::rearrange()
{
    //清除选中的按钮的标志位
    m_IDBeChecked = -1;
    //开始随机重排按钮
    randomRange();
    return true;
}
//初始化按钮
void CBricks::initialButtons(HWND hwnd)
{
    if (!hwnd)
    {
        MessageBox(NULL, _T("初始化失败。"), NULL, 1);
        return /*FALSE*/;
    }
    m_hwnd = hwnd;
    CWnd *pWnd = CWnd::FromHandle(hwnd);
    //m_bAlowResize = TRUE;
    //按钮初始位置
    int iButtonPosX = 23;
    int iButtonPosY = 23;
    //按钮大小
    int iButtonSizeW = 40;
    int iButtonSizeH = 40;
    //实际每个按钮的位置
    int iNewButtonPosX = 0;
    int iNewButtonPosY = 0;
    //设置窗口大小和位置
    int cx = GetSystemMetrics(SM_CXSCREEN);
    int cy = GetSystemMetrics(SM_CYSCREEN);
    SetWindowPos(hwnd, NULL, cx/2 - 270, cy/2 - 240, 540, 540, SWP_NOZORDER);

    int k = 2001;//按钮的图片id从2001开始
    int iCountPerPicture = 6;//每种图片有多少个
    int countFor_iCountPerPicture = 0;
    int iBricks = 0;//按钮计数
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j<10; j++)
        {
            //创建按钮，并设置位置
            iNewButtonPosX = iButtonPosX  + (iButtonSizeW * j);
            iNewButtonPosY = iButtonPosY  + (iButtonSizeW * i);
            m_ButonArray[i][j].Create(_T(""),WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,CRect(0,0,0,0),pWnd,IDC_BTN_SINGLEPIN_0+ i * 10 + j);
            m_ButonArray[i][j].SetWindowPos(NULL, iNewButtonPosX , iNewButtonPosY, iButtonSizeW, iButtonSizeH, SWP_NOZORDER | SWP_SHOWWINDOW);
            //正常、按下、焦点、禁用（只是用了正常一种图片）
            m_ButonArray[i][j].LoadBitmaps(k, 0, 0, 0);
            //根据图片大小决定按钮大小
            m_ButonArray[i][j].SizeToContent();
            //设置按钮相关属性。
            MyMain[iBricks].i = i;
            MyMain[iBricks].j = j;
            MyMain[iBricks].iNum = iBricks;
            MyMain[iBricks].bCheck = false;
            MyMain[iBricks].bDispel = false;
            MyMain[iBricks].iPos[0] = iNewButtonPosX;
            MyMain[iBricks].iPos[1] = iNewButtonPosY;
            MyMain[iBricks].iBMPID1 = k;
            MyMain[iBricks].iBMPID2 = k + 1;
            MyMain[iBricks].pBMPButton = &m_ButonArray[i][j];
            //MyMain[iBricks].pBMPButton->SetWindowPos(NULL, MyMain[i].iPos[0], MyMain[i].iPos[1] , 40, 40, SWP_NOZORDER);
            iBricks++;

            countFor_iCountPerPicture++;
            if (iCountPerPicture == countFor_iCountPerPicture)
            {
                k += 2;
                countFor_iCountPerPicture = 0;
            }
            if (k > 2042)//图片资源的最大ID
            {
                MessageBox(NULL, _T("没有足够的图片"), NULL, 1);
                break;
            }
        }
        if (k > 2042)
        {
            MessageBox(NULL, _T("没有足够的图片"), NULL, 1);
            break;
        }
    }
}

BOOL CBricks::oneRightAngleAfterTransform( int iPosX, int iPosY, int x2, int y2 )
{
    BOOL bRet = false;
    //改变一个按钮的x坐标，
    //新的坐标按钮与旧按钮可以直线联通
    //然后判断新的坐标，是否可以与另一个按钮，以一个拐角的方式联通。
    m_Xtransform = true;
    for (int i = 0; i < 10; i++)
    {
        if (MyMain[10 * i + iPosY].bDispel && isEmptyLine(i, iPosY, iPosX, iPosY))
        {
            bRet = oneRightAngle(i, iPosY, x2, y2);
        }
        if (!bRet && MyMain[10 * i + y2].bDispel  && isEmptyLine(i, y2, x2, y2))
        {
            bRet = oneRightAngle(iPosX, iPosY, i, y2);
        }
        if (bRet)
        {
            return true;
        }
    }
    if (!bRet)
    {
        //当一次拐角不能消除时：需要处理2次拐角的情况
        //假如需要测试A，B按钮是否可以消除
        //这里我们取一个新按钮C
        //当AC可以直接相连消除，并且BC可以通过一次拐角消除
        //说明AB可以2次拐角消除
        m_Xtransform = false;
        m_Ytransform = true;
        for (int j = 0; j < 10; j++)
        {
            if (MyMain[10 * iPosX + j].bDispel  && isEmptyLine(iPosX, j, iPosX, iPosY))
            {
                bRet = oneRightAngle(iPosX, j, x2, y2);
            }
            if (!bRet && MyMain[10 * x2+ j].bDispel  && isEmptyLine(x2, j, x2, y2))
            {
                bRet = oneRightAngle(iPosX, iPosY, x2, j);
            }
            if (bRet)
            {
                return TRUE;
            }
        }
    }
    m_Ytransform = false;
    return bRet;
}
