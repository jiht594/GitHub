// console.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <string>
#include <locale.h>
#include "StringUtility.h"
#include "LogUtility.h"
#include "comm.h"
#include "SingletonApp.h"
#include "AccessControl.h"

int _tmain(int argc, _TCHAR* argv[])
{
    char *p = "123Ë¹µÙ·Ò456";
    wchar_t *pwstr = StringUtility::AnsiToUnicode(p);
    wchar_t *p2 = _T("123Ë¹µÙ·Ò456");
    char *pstr = StringUtility::UnicodeToAnsi(p2);
    StringUtility::freeArrayChar(pstr);
    StringUtility::freeArrayWChar(pwstr);

    bool bExist = SingletonApp::isSingleApp(_T("spacename1"), _T("mutexname"));
    if (bExist)
    {
        _tprintf(_T("exist\n"));
    }
    else
    {
        _tprintf(_T("not exist\n"));
    }
    //system("pause");
    SingletonApp::release();
    //AccessControl ac;
    //ac.isAdmin();
    HANDLE hToken = NULL;

    // Get current process token
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
        return(FALSE);
    bool bAdmin = false;
    DWORD dwSize = 0;
    TOKEN_ELEVATION_TYPE tokenEleType = TokenElevationTypeDefault;
    TOKEN_ELEVATION_TYPE *pTokenEleType = &tokenEleType;
    if (GetTokenInformation(hToken, TokenElevationType, pTokenEleType, sizeof(TOKEN_ELEVATION_TYPE), &dwSize))
    {
        //LogUtility::showSystemError();
        PSID pSID = (PSID)new BYTE[SECURITY_MAX_SID_SIZE];
        SecureZeroMemory(pSID, SECURITY_MAX_SID_SIZE);
        DWORD cbSid;
        
        CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, pSID, &cbSid);
        
        BOOL bMember = FALSE;
        if (tokenEleType == TokenElevationTypeLimited)
        {
            TOKEN_LINKED_TOKEN tlt;
            SecureZeroMemory(&tlt, sizeof(tlt));
            if (GetTokenInformation(hToken, TokenLinkedToken, &tlt, sizeof(TOKEN_LINKED_TOKEN), &dwSize))
            {
                if ( CheckTokenMembership(tlt.LinkedToken, pSID,& bMember))
                {
                   bAdmin = true;;
                }
                
            }
        }
        else
        {
            if (IsUserAnAdmin())
                bAdmin = true;
        }
        LogUtility::showSystemError();
        if (bAdmin)
        {
            LogUtility::showMessageInConsole(_T("true\n"));
        }
        
        delete[] pSID;
        pSID = NULL;
    }
    
    
	return 0;
}

