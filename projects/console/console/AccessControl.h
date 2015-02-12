#pragma once
#include <Windows.h>
#include <Winnt.h>
#include <Shlobj.h>

//typedef enum  { 
//    TokenElevationTypeDefault  = 1, //������Ĭ���û����У�����UAC������
//    TokenElevationTypeFull,               //���̵�Ȩ�ޱ�������������û�б�ɸѡ��
//    TokenElevationTypeLimited         //����Ȩ�����У����Ʊ�ɸѡ
//} TOKEN_ELEVATION_TYPE , *PTOKEN_ELEVATION_TYPE;

class AccessControl
{
public:
    AccessControl(void);
    virtual ~AccessControl(void);
public:
    BOOL isAdmin();
    TOKEN_ELEVATION_TYPE getElevationType();
private:
    bool getProcessElevation(TOKEN_ELEVATION_TYPE* pElevationType, BOOL* pIsAdmin);
private:
    bool bInitSucess;
    BOOL bAdmin;
    TOKEN_ELEVATION_TYPE elevationType;
};

