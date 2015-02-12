#pragma once
#include <Windows.h>
#include <Winnt.h>
#include <Shlobj.h>

//typedef enum  { 
//    TokenElevationTypeDefault  = 1, //进程以默认用户运行，或者UAC被禁用
//    TokenElevationTypeFull,               //进程的权限被提升，且令牌没有被筛选过
//    TokenElevationTypeLimited         //受限权限运行，令牌被筛选
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

