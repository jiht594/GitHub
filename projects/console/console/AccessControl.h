#pragma once
#include <Windows.h>
#include <Winnt.h>
#include <Shlobj.h>

//typedef enum  { 
//    TokenElevationTypeDefault  = 1, //进程以默认用户运行，或者UAC被禁用
//    TokenElevationTypeFull,               //进程的权限被提升，且令牌没有被筛选过
//    TokenElevationTypeLimited         //受限权限运行，令牌被筛选
//} TOKEN_ELEVATION_TYPE , *PTOKEN_ELEVATION_TYPE;


/*
在windows 7上，如果当前登录账号为管理员账号，则系统会分配两个访问令牌给用户，
一个是受限的访问令牌(对应TokenElevationTypeLimited)，
一个是有完整管理员权限的访问令牌(对应TokenElevationTypeFull），
如果程序没有以管理员身份运行，则程序会与受限的访问令牌关联，否则与另外一个关联。
若当前登录账号为标准用户，则系统只会给用户一个默认访问令牌（对应TokenElevationTypeDefault）。
*/
class AccessControl
{
public:
    AccessControl(void);
    virtual ~AccessControl(void);
public:
	bool getProcessElevation(TOKEN_ELEVATION_TYPE* pElevationType, BOOL* pIsAdmin);
	DWORD StartElevatedProcess(LPCTSTR szExecutable, LPCTSTR szCmdLine);
private:
    //BOOL bAdmin;
    TOKEN_ELEVATION_TYPE elevationType;
};

