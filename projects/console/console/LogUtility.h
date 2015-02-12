#pragma once
#define IDS_ERROR_UNKONWN  _T("ÕÒ²»µ½´íÎó´úÂë")

static HANDLE g_hProcess = NULL;
static HANDLE g_hThread = NULL;

class LogUtility
{
public:
    LogUtility(void);
    ~LogUtility(void);
public:
    static void showSystemError();
    static void showSystemErrorByErrorID(DWORD errorID);
    static bool openConsole();
    static void showMessageInConsole(const TCHAR *szMsg);
    static void closeConsole();
private:
    static TCHAR* getSystemErrorMsgByID(DWORD errorID);
};

