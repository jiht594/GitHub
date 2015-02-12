#include "StdAfx.h"
#include <Windows.h>
#include "LogUtility.h"


LogUtility::LogUtility(void)
{
}


LogUtility::~LogUtility(void)
{
}
void LogUtility::showSystemErrorByErrorID(DWORD errorID)
{
    MessageBox(NULL,  getSystemErrorMsgByID(errorID), NULL, MB_OK);
}
void LogUtility::showSystemError()
{
    MessageBox(NULL,  getSystemErrorMsgByID(GetLastError()), NULL, MB_OK);
}
TCHAR* LogUtility::getSystemErrorMsgByID(DWORD dwErrorID)
{
    TCHAR *ptstrErrorMsg = NULL;
    HLOCAL hlocal = NULL;
    DWORD systemLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);

    BOOL fOk = FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
        NULL, dwErrorID, systemLocale, (PTSTR)&hlocal, 0, NULL );
    if (!fOk)
    {
        HMODULE hDll = LoadLibraryEx(_T("netmsg.dll"), NULL, DONT_RESOLVE_DLL_REFERENCES);
        if (hDll != NULL)
        {
            fOk = FormatMessage(FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
                hDll, dwErrorID, systemLocale, (PTSTR)&hlocal, 0, NULL);
            FreeLibrary(hDll);
        }
    }

    if (fOk && hlocal != NULL)
    {
        ptstrErrorMsg = (PTSTR)LocalLock(hlocal);
    }
    else
    {
        ptstrErrorMsg = IDS_ERROR_UNKONWN;
    }
    return ptstrErrorMsg;
}
bool LogUtility::openConsole()
{
    if(AllocConsole())
    {
        TCHAR szBuff[128];
        _stprintf_s(szBuff, 128, _T("Debug窗口 进程ID:%d"), GetCurrentProcessId());
        SetConsoleTitle(szBuff);  //设置控制台标题
        freopen("conin$", "r+t", stdin);   
        freopen("conout$", "w+t", stdout);   
        freopen("conout$", "w+t", stderr);
        return true;
    }
    else
    {
        return false;
    }
}
void LogUtility::showMessageInConsole(const TCHAR *szMsg)
{
    _tprintf(_T("%s"), szMsg);
}
void LogUtility::closeConsole()
{
    fclose(stderr);   
    fclose(stdout);   
    fclose(stdin);   
    FreeConsole();  
}