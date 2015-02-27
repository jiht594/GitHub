#include "StdAfx.h"
#include <Windows.h>
#include <afxmt.h>
#include "LogUtility.h"


TCHAR* LogUtility::m_szFilePath = NULL;
FILE* LogUtility::m_stream_out = NULL;
FILE* LogUtility::m_stream_in = NULL;

#ifdef WINVER
TCHAR* LogUtility::m_szDelimiter = _T("\r\n");
#else
TCHAR* LogUtility::m_szDelimiter = _T("\n")
#endif


BOOL WINAPI HandlerRoutine(  DWORD dwCtrlType )
{
    switch(dwCtrlType)
    {
    case CTRL_CLOSE_EVENT:
        LogUtility::closeConsole();
        break;
    default:
        break;
    }
    return TRUE;
}
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
#ifdef _DEBUG
    if(AllocConsole())
    {
        TCHAR szBuff[128];
        _stprintf_s(szBuff, 128, _T("Debug窗口 进程ID:%d"), GetCurrentProcessId());
        SetConsoleTitle(szBuff);  //设置控制台标题
        _tfreopen_s(&m_stream_out, _T("conout$"), _T("w+t"), stdout);   
        _tfreopen_s(&m_stream_in, _T("conin$"), _T("r+t"), stdin);   
        if (m_stream_in && m_stream_out)
        {
            _ftprintf_s(m_stream_out, _T("console is open\n"));
        }
         
        SetConsoleCtrlHandler(HandlerRoutine, TRUE);
        return true;
    }
    else
    {
        return false;
    }
#else// _DEBUG
    return false;
#endif
}
void LogUtility::showMessageInConsole(const TCHAR *szMsg)
{
    if (!m_stream_in || !m_stream_out)
    {
        closeConsole();
        openConsole();
    }
    TCHAR *sz = getFormatTimeString();
    _tprintf(_T("%s, Thread ID:%d, Message:%s%s"), sz, GetCurrentThreadId(), szMsg, m_szDelimiter);
    delete[] sz;
}
void LogUtility::closeConsole()
{
#ifdef _DEBUG
    if (m_stream_out)
    {
        fclose(m_stream_out);
    }   
    if (m_stream_in)
    {
        fclose(m_stream_in);
    }
//     fclose(stderr);   
//     fclose(stdin);   
    FreeConsole();
#endif // _DEBUG
}

void LogUtility::writeLogFile(const TCHAR* msg)
{
    if (m_szFilePath)
    {
        CMutex mutex(FALSE, _T("LOGGINGMUTEXT_JIHAITAO"));
        CFile file;

        mutex.Lock();
        TRY 
        {
	        file.Open(m_szFilePath, CFile::modeCreate | CFile::modeNoTruncate  | CFile::shareDenyWrite | CFile::modeReadWrite);
	        CString sz;
	        TCHAR *szTime = getFormatTimeString();
	        sz.Format(_T("%s, Message:%s%s"), szTime, msg, m_szDelimiter);
	        delete[] szTime;
	        file.SeekToEnd();
	        file.Write(sz, sz.GetLength());
	        file.Close();
        }
        CATCH (CFileException, e)
        {
            UNREFERENCED_PARAMETER(e);
            TCHAR *errMsg = _T("----------------\nFaile to write file----------------\r\n");
            _tprintf(errMsg);
            file.Close();
        }
        END_CATCH
        mutex.Unlock();
    }
}
void LogUtility::setLogFilePath(TCHAR* sz)
{
    if (sz && _tcslen(sz) < MAX_PATH)
    {
        m_szFilePath = sz;
    }
}

TCHAR* LogUtility::getFormatTimeString()
{
    SYSTEMTIME systemTime;
    GetLocalTime(&systemTime);
    CString strTime;
    strTime.Format(_T("%d/%d/%d-%d:%d:%d/%d")
        , systemTime.wYear
        , systemTime.wMonth
        , systemTime.wDay
        , systemTime.wHour
        , systemTime.wMinute
        , systemTime.wSecond
        , systemTime.wMilliseconds);
    int length = strTime.GetLength() + 1;
    TCHAR *szStr = new TCHAR[length];
    SecureZeroMemory(szStr, length);
    _tcscpy_s(szStr, length, strTime);
    return szStr;
}
