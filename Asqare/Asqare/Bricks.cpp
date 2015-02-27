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
        //�Ѿ����һ����Ϸ����ʼ�µ�һ��
        rearrange();
        m_bCompleted = false;
    }
    else
    {
        //��һ����Ϸ����ʼ����ť
        initialButtons(hwnd);
    }
    
    //����������а�ť
    randomRange();

    //AfxBeginThread(MyThreadProc, MyMain);
    //�����Զ���⡰�Ƿ��п�������ť���ļ�ʱ��
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
        //�������а�ť������һ�������ť����
        iRandom = rand()%100;
        iPossiton = iRandom / 10;
        jPossiton = iRandom % 10;

        if (!m_bCompleted)
        {
            //δ��ɵ����������ʱ�����������İ�ť
            if (MyMain[i].bDispel)
            {
                continue;
            }
            //�����ťҲ��Ҫ������������
            while (MyMain[iPossiton * 10 + jPossiton].bDispel)//Ч�ʵ�
            {
                iRandom = rand()%100;
                iPossiton = iRandom / 10;
                jPossiton = iRandom % 10;
            }
        }

        //����2����ť��ͼƬ
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
         //�������

        if (m_bCompleted)
        {
            //����Ѿ�����˱�����Ϸ����ô��ʼ��һ��
            
            //�ָ���־λ�� ȥ���Ѿ���ǵ���ʾ��
            MyMain[iPossiton * 10 + jPossiton].bCheck = FALSE;
            MyMain[iPossiton * 10 + jPossiton].bDispel = FALSE;
            MyMain[iPossiton * 10 + jPossiton].pBMPButton->setbDrawRect(FALSE);

            MyMain[i].bCheck = FALSE;
            MyMain[i].bDispel = FALSE;
            MyMain[i].pBMPButton->setbDrawRect(FALSE);

            //������ʾ���а�ť
            MyMain[iPossiton * 10 + jPossiton].pBMPButton->EnableWindow(TRUE);
            MyMain[iPossiton * 10 + jPossiton].pBMPButton->ShowWindow(SW_SHOW);
            MyMain[i].pBMPButton->EnableWindow(TRUE);
            MyMain[i].pBMPButton->ShowWindow(SW_SHOW);
        }

        //ѡ��״̬�ָ���Ŷ������״̬
        if (0 == MyMain[iPossiton * 10 + jPossiton].iBMPID1 % 2)
        {
            MyMain[iPossiton * 10 + jPossiton].pBMPButton->LoadBitmaps(--MyMain[iPossiton * 10 + jPossiton].iBMPID1);
        }

        if (0 == MyMain[i].iBMPID1 % 2)
        {
            MyMain[i].pBMPButton->LoadBitmaps(--MyMain[i].iBMPID1);
        }
    }
    //����ʱ�������¿�ʼ������Ҫȥ������ʾ����һ�Ժ��
    if (!m_bCompleted)
    {
        resetButtons();
    }
    //�¾ֿ�ʼ�����»���
    AfxGetApp()->GetMainWnd()->Invalidate(FALSE);
    UpdateWindow(m_hwnd);
}

void CBricks::setCheck(UINT ID)
{
    //ȥ������ʾ�����ܻ��ϵ�2����ɫ����
    resetButtons();
    if (m_bAutoCheck)
    {
        //resetButtons();
        m_bAutoCheck = false;
    }

    UINT ButtonID = 0;//��ťid��2100��ʼ 10��10����
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
                    //�ı䱾�ε���İ�ť��״̬
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
                    //test�������!m_bChecking�жϿ����ǲ���Ҫ��
                    //��Զ�߲�������
                    MessageBox(m_hwnd, _T("never show up"), _T("caption"), MB_OK);
                }
                

                //�鿴֮ǰû��������ť��ѡ��״̬
                //��m_IDBeChecked��=-1���뱾��ѡ�еİ�ť�Ƚ�
                if (m_IDBeChecked == -1)
                {
                    //��ǰû��ѡ�а�ť
                    MyMain[i].pBMPButton->LoadBitmaps(MyMain[i].iBMPID1);
                    m_iCheckedPosX = MyMain[i].i;
                    m_iCheckedPosY = MyMain[i].j;
                    m_IDBeChecked = i;
                    MyMain[i].pBMPButton->Invalidate(FALSE);
                }
                else
                {
                    //��ǰѡ����һ����ť���뱾��ѡ�еıȽϣ����Ƿ��������
                    if (!eliminate(MyMain[i].i, MyMain[i].j, m_iCheckedPosX, m_iCheckedPosY))
                    {
                        //�������������һ�԰�ť
                        //���°�ť��ѡ��״̬
                        if (m_IDBeChecked == i)
                        {
                            //�������ϴ�ѡ�е���ͬһ����ť�����ð�ťΪ����״̬
                            MyMain[i].pBMPButton->LoadBitmaps(MyMain[i].iBMPID1);
                            m_IDBeChecked = -1;
                        }
                        else
                        {
                            //����ͬһ����ť�����ñ���ѡ�а�ťΪѡ��״̬
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
                        //�����Գ���һ�԰�ťʱ��������2����ť������
                        writeLog(MyMain[i].i, MyMain[i].j, m_iCheckedPosX, m_iCheckedPosY);
                        m_path.insert(MyMain[i].i * 10 + MyMain[i].j);
                        if (m_path.size() >= 4 && !m_bChecking)
                        {
                            //����û���ã�����һ���㷨������ʱ��������Ҫm_path
                            //m_path�Ǽ�¼·�������Լ���սǴ�����û����ɣ�
                            //MessageBox(NULL, _T("> 4"), _T(""), 0);
                            //return;
                        }

                        //��������������
                        CString strSound;
                        if (strSound.LoadString(IDS_SOUND_ELI))
                        {
                            //PlaySound�����޷�ͬʱ����2������
                            //����˵����ͬʱ���ű������ֺ�����������
                            //���Ա�������ʹ����MCI��ʽ���ţ�MCI����ͬʱ���Ŷ�������ļ�
                            PlaySound(strSound, NULL, SND_ASYNC);
                        }
                        //������2����ť
                        MyMain[i].pBMPButton->ShowWindow(SW_HIDE);
                        MyMain[i].pBMPButton->EnableWindow(FALSE);
                        MyMain[i].bDispel = TRUE;
                        MyMain[m_IDBeChecked].pBMPButton->ShowWindow(SW_HIDE);
                        MyMain[m_IDBeChecked].pBMPButton->EnableWindow(FALSE);
                        MyMain[m_IDBeChecked].bDispel = TRUE;
                        m_IDBeChecked = -1;
                        //ÿ������һ�԰�ť���Զ�����Ƿ��п������İ�ť��
                        SetTimer(m_hwnd, 1, 1000, NULL);
                    }
                }
            }
        }
    }
    //InvalidateRect(m_hwnd, NULL, TRUE);
}

//�����������㷨������
//���ܻ��д���
//��Ҫ2���ս�����ʱ�����ܻ������⡣����У���Ҫm_path��¼����·�ߣ�Ȼ������м����ս�
BOOL CBricks::eliminate(int x, int y, int x2, int y2)
{
    emptyPathMap();
    //��3������:��2����ť����һ�ιս�����ʱ����Ҫ�任һ����ť��λ�ã����
    m_bTransfom = false;//�Ƿ�任��λ�ã� ���Ŀǰû�õ�
    m_Xtransform = false;//�ı���x����
    m_Ytransform = false;//�ı���y����
    BOOL bRet = FALSE;
    int iPosX = x;
    int iPosY = y;
    m_iTempPosX = x2;
    m_iTempPosY = y2;

    //tempCheckIdΪ��һ��ѡ�е�ͼƬ��Ӧ�þ���x2*10+y2
    int tempCheckId = m_bAutoCheck ? m_IDBeChecked2222 : m_IDBeChecked;
    int iBMPID1 = MyMain[10 * iPosX + iPosY].iBMPID1;
    int iBMPID2 = MyMain[tempCheckId].iBMPID1;
    int iBigBMP = iBMPID1 > iBMPID2 ? iBMPID1 : iBMPID2;
    int iSmallBMP = iBMPID1 < iBMPID2 ? iBMPID1 : iBMPID2;

    //bSamaBMPID��2����ť�Ƿ���ͬһ��ͼƬ
    bool bSamaBMPID = false;
    if ((iBMPID1 == iBMPID2) //��ͬͼƬID
        || ((iBigBMP % 2 == 0) && (iBigBMP - iSmallBMP == 1)))//��������ѡ�У�2��״̬��һ��ͼƬ��Ҳ������ͬ��
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
            //��1�������2����ť���ڱ߽��ϣ�����ͬһ��ı߽�
            bRet = TRUE;
        }
        else if (iPosX == x2 || iPosY == y2)
        {
            //��2�������������ť��ͬһ�л���ͬһ��

            //�ȼ���Ƿ����ֱ������
            bRet = noRightAngle(iPosX == x2 ? TRUE : FALSE, iPosX, iPosY);
            if (bRet)
            {
            }
            else
            {
                //����ֱ�������һ���ս��ܷ���ͨ
                bRet = oneRightAngle(iPosX, iPosY, x2, y2);
                if (bRet)
                {
                }
                else
                {
                    //һ���ҽ�Ҳ�޷���ͨ����Ҫ���2���սǵ����
                    //Ҳ���Ǳ任һ����ť��λ�ã��ټ��һ���ս�
                    bRet = oneRightAngleAfterTransform(iPosX, iPosY, x2, y2);
                }
            }
        }
        else
        {
            //��3�������2����ť����һ�����ϣ����ж��Ƿ�һ���սǿ���ͨ
            bRet = oneRightAngle(iPosX, iPosY, x2, y2);
            if (!bRet)
            {
                //λ�ñ任��, �ټ��
                bRet = oneRightAngleAfterTransform(iPosX, iPosY, x2, y2);
            }
        }
    }
    return bRet;
}

//2����ť�Ƿ����ֱ������
BOOL CBricks::noRightAngle(BOOL XOrY, int x, int y)
{
    //XOrY�Ƿ��򣬱�ʾ2����ťʱ��ͬһ�л���ͬһ��
    BOOL bPassageway = TRUE;
    int iBigOne;
    int iSmallOne;
    if (XOrY)
    {
        iBigOne = m_iTempPosY > y ? m_iTempPosY : y;
        iSmallOne = m_iTempPosY > y ? y : m_iTempPosY;
        //�жϣ���small��bigһ�Ű�ť���Ƿ���������dispel������
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
            //�жϣ���small��bigһ�Ű�ť���Ƿ���������dispel������
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

//�ж�2����ť����ͨ��һ���ս�����
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


    //�����Ǵ����任������

    //ԭ������һ�ιսǲ�������ʱ����Ҫ����2�ιսǵ����
    //������Ҫ����A��B��ť�Ƿ��������
    //��������ȡһ���°�ťC
    //��AC����ֱ����������������BC����ͨ��һ�ιս�����
    //˵��AB����2�ιս�������

    //�ο����棨���е�ԭ��
    //�������Ĺ����Ǽ��BC����һ�ιս�����
    //��B��ť�Ǳ߽��ʱ����Ҫ�����ڡ��߽������ߡ����������
    //������C������һ����ť�����Ǳ߽����һ�����ⰴť��
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

//����սǾͿ�������
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

//2����ť����ֱ���������������noRightAngle��������
//���������һ����ťx1y1�Ǳ���������DispelΪtrue
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

//�Զ�����Ƿ��п���������һ�԰�ť
//�����ʹ�á���ʾ��ť������autoCheck����
//���ڰ�ť�ϻ��Ϻ�ɫ��������ʾ��Ϸ��
bool CBricks::autoCheck()
{
    bool bRet = false;
    MainData md1;//������һ����ť���ٱ���md2�����Ƿ��п���������
    MainData md2;//�ڶ�����ť

    if (!m_bChecking)
    {
        //�Ǻ�̨���ʱ��OnTimer�У���ȥ����������ʾ�ĺ��
        resetButtons();
    }
    m_bAutoCheck = true;//���ֶ�
    bool bAllDispel = true;//��ť�Ƿ�ȫ�������ı�־λ
    for (int i = 0; i < 100; i++)
    {
        if (bRet)
        {
            break;
        }
        md1 = MyMain[i];
        if (MyMain[i].bDispel)
        {
            //���������İ�ť
            continue;
        }
        else
        {
            //����а�ťû������
            bAllDispel = false;
        }

        for (int j = i + 1; j < 100; j++)
        {
            md2 = MyMain[j];
            if (MyMain[j].bDispel)
            {
                //���ذ�ť����
                continue;
            }

            int iCheckButon = -1;//����m_IDBeChecked2222��ʹ�ú�ָ�m_IDBeChecked2222��ԭ��ֵ
            if (m_IDBeChecked2222 != -1)
            {
                iCheckButon = m_IDBeChecked2222;
            }
            //m_IDBeChecked2222������¼һ����ť����Eliminate������ʹ��---���ֶ�����ʾ��ť �� ��̨��� 2�������
            //m_IDBeCheckedҲ��������¼һ����ť����Eliminate������ʹ��---�ֶ�
            m_IDBeChecked2222 = j;

            if (eliminate(i/10, i%10, j/10, j%10))
            {
                //��������
                writeLog(i/10, i%10, j/10, j%10);
                m_path.insert(i);
                m_path.insert(j);
                if (m_path.size() >= 4)//����·�����������жϼ����սǣ���û�õ�
                {
                    //MessageBox(NULL, _T("> 4"), _T(""), 0);
                    //continue;;
                }

                if (m_bChecking)
                {
                    //��̨��⣺����Ҫ������ʾ����break
                    //m_bAutoCheck = true;
                    KillTimer(m_hwnd, 1);
                    m_IDBeChecked2222 = iCheckButon;
                    bRet = true;
                    break;
                }
                else
                {
                    //����ʾ����ť���
                    //����2����ɫ��ʾ��
                    MyMain[i].pBMPButton->setbDrawRect(true);
                    MyMain[j].pBMPButton->setbDrawRect(true);
                    MyMain[i].pBMPButton->Invalidate();
                    MyMain[j].pBMPButton->Invalidate();
                    //����
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
        //���а�ť�������ˣ�˵��������Ϸ�Ѿ����
        m_bCompleted = true;
    }
    //m_IDBeChecked2222 = -1;
    return bRet;
}

//ȥ������ʾ���ܡ����ϵ�2����ɫ����
void CBricks::resetButtons()
{
    for (int i = 0; i < 100; i++)
    {
        if (MyMain[i].pBMPButton->getbDrawRect())
        {
            //���ñ�־λ
            MyMain[i].pBMPButton->setbDrawRect(false);
            //����ˢ������
            CRect rect;
            MyMain[i].pBMPButton->GetWindowRect(&rect);
            rect.top -=2;
            rect.bottom += 3;
            rect.left -= 2;
            rect.right += 3;
            CWnd *pWnd = AfxGetApp()->GetMainWnd();
            pWnd->ScreenToClient(&rect);
            //���»���
            pWnd->InvalidateRect(&rect);
            UpdateWindow(m_hwnd);
        }
    }
}

//��־����
void CBricks::writeLog( int x, int y, const int m_x, const int m_y, const int tx, int ty, const int kx, const int ky )
{
    CString strMsg;
    strMsg.Format(_T("one angle A:%d-%d, B%d-%d, empty line C:%d-%d, D%d-%d")
        , x, y, m_x, m_y, tx, ty, kx, ky);
    LogUtility::showMessageInConsole(strMsg);
}
//��־����
void CBricks::writeLog( int x, int y, int x2, int y2)
{
    CString strMsg;
    strMsg.Format(_T("orignal X:%d-%d, Y%d-%d")
        , x, y, x2, y2);
    LogUtility::showMessageInConsole(strMsg);
}
//��¼����·����û�����
void CBricks::insertPathPoint( int x1, int y1, const int x2, const int y2, const int x3, int y3, const int x4, const int y4 )
{
     m_path.insert(x1 * 10 + y1);
     m_path.insert(x2 * 10 + y2);
     m_path.insert(x3 * 10 + y3);
     m_path.insert(x4 * 10 + y4);
}
//���·��
void CBricks::emptyPathMap()
{
    m_path.clear();
}
//�������а�ť
BOOL CBricks::rearrange()
{
    //���ѡ�еİ�ť�ı�־λ
    m_IDBeChecked = -1;
    //��ʼ������Ű�ť
    randomRange();
    return true;
}
//��ʼ����ť
void CBricks::initialButtons(HWND hwnd)
{
    if (!hwnd)
    {
        MessageBox(NULL, _T("��ʼ��ʧ�ܡ�"), NULL, 1);
        return /*FALSE*/;
    }
    m_hwnd = hwnd;
    CWnd *pWnd = CWnd::FromHandle(hwnd);
    //m_bAlowResize = TRUE;
    //��ť��ʼλ��
    int iButtonPosX = 23;
    int iButtonPosY = 23;
    //��ť��С
    int iButtonSizeW = 40;
    int iButtonSizeH = 40;
    //ʵ��ÿ����ť��λ��
    int iNewButtonPosX = 0;
    int iNewButtonPosY = 0;
    //���ô��ڴ�С��λ��
    int cx = GetSystemMetrics(SM_CXSCREEN);
    int cy = GetSystemMetrics(SM_CYSCREEN);
    SetWindowPos(hwnd, NULL, cx/2 - 270, cy/2 - 240, 540, 540, SWP_NOZORDER);

    int k = 2001;//��ť��ͼƬid��2001��ʼ
    int iCountPerPicture = 6;//ÿ��ͼƬ�ж��ٸ�
    int countFor_iCountPerPicture = 0;
    int iBricks = 0;//��ť����
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j<10; j++)
        {
            //������ť��������λ��
            iNewButtonPosX = iButtonPosX  + (iButtonSizeW * j);
            iNewButtonPosY = iButtonPosY  + (iButtonSizeW * i);
            m_ButonArray[i][j].Create(_T(""),WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,CRect(0,0,0,0),pWnd,IDC_BTN_SINGLEPIN_0+ i * 10 + j);
            m_ButonArray[i][j].SetWindowPos(NULL, iNewButtonPosX , iNewButtonPosY, iButtonSizeW, iButtonSizeH, SWP_NOZORDER | SWP_SHOWWINDOW);
            //���������¡����㡢���ã�ֻ����������һ��ͼƬ��
            m_ButonArray[i][j].LoadBitmaps(k, 0, 0, 0);
            //����ͼƬ��С������ť��С
            m_ButonArray[i][j].SizeToContent();
            //���ð�ť������ԡ�
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
            if (k > 2042)//ͼƬ��Դ�����ID
            {
                MessageBox(NULL, _T("û���㹻��ͼƬ"), NULL, 1);
                break;
            }
        }
        if (k > 2042)
        {
            MessageBox(NULL, _T("û���㹻��ͼƬ"), NULL, 1);
            break;
        }
    }
}

BOOL CBricks::oneRightAngleAfterTransform( int iPosX, int iPosY, int x2, int y2 )
{
    BOOL bRet = false;
    //�ı�һ����ť��x���꣬
    //�µ����갴ť��ɰ�ť����ֱ����ͨ
    //Ȼ���ж��µ����꣬�Ƿ��������һ����ť����һ���սǵķ�ʽ��ͨ��
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
        //��һ�ιսǲ�������ʱ����Ҫ����2�ιսǵ����
        //������Ҫ����A��B��ť�Ƿ��������
        //��������ȡһ���°�ťC
        //��AC����ֱ����������������BC����ͨ��һ�ιս�����
        //˵��AB����2�ιս�����
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
