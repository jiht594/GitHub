//************************************
// 写log用
// 写入文件
// 开启控制台（窗口程序）
// 通过error号，获得错误信息
// 
//************************************
#pragma once
#define IDS_ERROR_UNKONWN  _T("找不到错误代码")

static HANDLE g_hProcess = NULL;
static HANDLE g_hThread = NULL;

class LogUtility
{
public:
    LogUtility(void);
    ~LogUtility(void);
public:
    static void showSystemError();
    static void showSystemErrorByErrorID(const DWORD errorID);
    static bool openConsole();
    static void showMessageInConsole(const TCHAR *szMsg);
    static void closeConsole();
    static void writeLogFile(const TCHAR *szMsg);
    static void setLogFilePath(TCHAR*);
private:
    static TCHAR* getSystemErrorMsgByID(const DWORD errorID);
    //static TCHAR* getRowDelimiter(){return m_szDelimiter;};
    static TCHAR* getFormatTimeString();
    static TCHAR *m_szFilePath;
    static TCHAR *m_szDelimiter;
    static FILE *m_stream_out;
    static FILE *m_stream_in;
};

