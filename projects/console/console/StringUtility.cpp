#include "StdAfx.h"
#include <Windows.h>
#include "StringUtility.h"


StringUtility::StringUtility(void)
{
}


StringUtility::~StringUtility(void)
{
}

wchar_t* StringUtility::AnsiToUnicode(const char *pStr)
{
    int iCount = 0;
    int iSize = 0;
    wchar_t *pStrOut = NULL;
    if (pStr)
    {
        iCount = MultiByteToWideChar(CP_ACP, 0, pStr, -1, NULL, 0);
        iSize = iCount * sizeof(wchar_t);
        pStrOut = new wchar_t[iCount];
        SecureZeroMemory(pStrOut, iSize);
        if (pStrOut)
        {
            MultiByteToWideChar(CP_ACP, 0, pStr, -1, pStrOut, iSize);
        }
    }
    return pStrOut;
}

char* StringUtility::UnicodeToAnsi(const wchar_t *pStr)
{
    char *pStrOut = NULL;
    int iCount = 0;
    int iSize = 0;
	UNREFERENCED_PARAMETER(iSize);
    if (pStr)
    {
        iCount = WideCharToMultiByte(CP_ACP, 0, pStr, -1, NULL, 0, NULL, NULL);
        pStrOut = new char[iCount];
        if (pStrOut)
        {
            WideCharToMultiByte(CP_ACP, 0, pStr, -1, pStrOut, iCount, NULL, NULL);
        }
    }
    return pStrOut;
}

void StringUtility::freeArrayChar(const char *pcstr)
{
    if (pcstr)
    {
        delete[] pcstr;
        pcstr = NULL;
    }
}
void StringUtility::freeArrayWChar(const wchar_t * pwstr)
{
    if (pwstr)
    {
        delete[] pwstr;
        pwstr = NULL;
    }
}