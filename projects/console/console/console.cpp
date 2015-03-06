// console.cpp : Defines the entry point for the console application.
//

#include "StdAfx.h"
#include <Windows.h>
#include <string>
#include <locale.h>
#include "StringUtility.h"
#include "LogUtility.h"
#include "comm.h"
#include "SingletonApp.h"
#include "AccessControl.h"
#include <sstream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);


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


	TOKEN_ELEVATION_TYPE t;
	BOOL pIsAdmin = FALSE;
    AccessControl ac;
	if (ac.getProcessElevation(&t, &pIsAdmin))
	{
		if (pIsAdmin == TRUE)
		{
			printf("user is an administrator\n");
			if (t == TokenElevationTypeLimited)
			{
				printf("but process don't have the administrator rights");
			}
			else
			{
				printf("and process do have the administrator rights");
			}
		}
	}
	else
	{
		printf("getProcessElevation failed\n");
	}
	
	system("pause");
	return 0;
}

