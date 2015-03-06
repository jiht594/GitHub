#include "StdAfx.h"
#include <Windows.h>
#include <Sddl.h>
#include "SingletonApp.h"


SingletonApp::SingletonApp(void)
{
}


SingletonApp::~SingletonApp(void)
{
}
bool SingletonApp::isSingleApp(const TCHAR *spaceName, const TCHAR * mutexName)
{
    bool bAppExist = false;
    int iStatusNamespace = createNamespace(spaceName);
    if (iStatusNamespace == NAMESPACE_CREATE_SUCCESS)
    {
        bAppExist = checkSingletonObject(spaceName, mutexName);
    }
    else if (iStatusNamespace == NAMESPACE_OPEN_SUCCESS)
    {
        bAppExist = checkSingletonObject(spaceName, mutexName);
    }
    else
    {
        //sometimes need % access right.
    }
    return bAppExist;
}

int SingletonApp::createNamespace(const TCHAR *spaceName)
{ 
    // Create the boundary descriptor
    int errorMsg = NAMESPACE_CREATE_SUCCESS;
    HANDLE hBoundary = CreateBoundaryDescriptor(_T("boundary name"), 0);

    // Create a SID corresponding to the Local Administrator group
    //这个SID决定谁可以创建命名空间及在命名空间中创建内核对象
    BYTE localAdminSID[SECURITY_MAX_SID_SIZE];
    PSID pLocalAdminSID = &localAdminSID;
    DWORD cbSID = sizeof(localAdminSID);
    if (!CreateWellKnownSid( /*WinWorldSid*//*WinBuiltinUsersSid*/ WinBuiltinAdministratorsSid, NULL, pLocalAdminSID, &cbSID))
    {
            //AddText(TEXT("AddSIDToBoundaryDescriptor failed: %u\r\n"), GetLastError());
            errorMsg = SID_CREATE_FAIL;
            //     LPTSTR pStrSID = NULL;
            //     if (ConvertSidToStringSid(pLocalAdminSID, &pStrSID) != 0)
            //     {
            //         //返回SID字符串，eg: s-1-5-32-532
            //         MessageBox(NULL, pStrSID, NULL, MB_OK);
            //     }
    }
    else
    {
        // Associate the Local Admin SID to the boundary descriptor
        // --> only applications running under an administrator user
        //     will be able to access the kernel objects in the same namespace
        if (!AddSIDToBoundaryDescriptor(&hBoundary, pLocalAdminSID))
        {
            //AddText(TEXT("AddSIDToBoundaryDescriptor failed: %u\r\n"), GetLastError());
            errorMsg = ADD_SID_TO_BOUNDARY_FAIL;
        }
        else
        {
            // Create the namespace for Local Administrators only
            //这个函数的第一个参数（BA：built-in administrator），这个是RID，表示对象的**完整性级别**。
            //命名空间和其中内核对象的可读性。设置为NULL，则任何进程都可以打开命名空间和其中对象
            //但是不可以create*
            SECURITY_ATTRIBUTES sa;
            sa.nLength = sizeof(sa);
            sa.bInheritHandle = FALSE;
            if (!ConvertStringSecurityDescriptorToSecurityDescriptor(
                TEXT("D:(A;;GA;;;BA)"), 
                SDDL_REVISION_1, &sa.lpSecurityDescriptor, NULL))
            {
                    //AddText(TEXT("Security Descriptor creation failed: %u\r\n"), GetLastError());
                    errorMsg = SD_CREATE_FAIL;
            }
            else
            {
                HANDLE hNamespace = CreatePrivateNamespace(/*NULL*/&sa, hBoundary, spaceName);

                // Don't forget to release memory for the security descriptor
                LocalFree(sa.lpSecurityDescriptor);

                // Check the private namespace creation result
                DWORD dwLastError = GetLastError();
                if (hNamespace == NULL) 
                {
                    // Nothing to do if access is denied
                    // --> this code must run under a Local Administrator account
                    if (dwLastError == ERROR_ACCESS_DENIED) 
                    {
                        //AddText(TEXT("Access denied when creating the namespace.\r\n"));
                        //AddText(TEXT("   You must be running as Administrator.\r\n\r\n"));
                        errorMsg = NAMESPACE_ACCESS_DENY;
                    } 
                    else if (dwLastError == ERROR_ALREADY_EXISTS) 
                    {
                            // If another instance has already created the namespace, 
                            // we need to open it instead. 
                            //AddText(TEXT("CreatePrivateNamespace failed: %u\r\n"), dwLastError);
                            hNamespace = OpenPrivateNamespace(hBoundary, spaceName);
                            if (hNamespace == NULL)
                            {
                                //AddText(TEXT("   and OpenPrivateNamespace failed: %u\r\n"),  dwLastError);
                                errorMsg = NAMESPACE_OPEN_FAIL;
                            } 
                            else 
                            {
                                // AddText(TEXT("   but OpenPrivateNamespace succeeded\r\n\r\n"));
                                //成功打开
                                errorMsg = NAMESPACE_OPEN_SUCCESS;
                            }
                        }
                        else
                        {
                            //AddText(TEXT("Unexpected error occured: %u\r\n\r\n"),dwLastError);
                            return -1;
                        }
                }
                else
                {
                    errorMsg = NAMESPACE_CREATE_SUCCESS;
                }
            }
        }
    }
    return errorMsg;
}
bool SingletonApp::checkSingletonObject(const TCHAR *spaceName, const TCHAR * mutexName)
{
	UNREFERENCED_PARAMETER(mutexName);
    bool bExist = false;
    // Try to create the mutex object with a name 
    // based on the private namespace 
    // “命名空间名/内核对象名” 
    TCHAR szMutexName[MAX_PATH] = {0};
    _stprintf_s(szMutexName, _countof(szMutexName), TEXT("%s\\%s"), spaceName, TEXT("Singleton"));
    g_hSingleton = CreateMutex(NULL, FALSE, szMutexName);
    int iErrorCode = GetLastError();
    if (iErrorCode == ERROR_ALREADY_EXISTS) {
        // There is already an instance of this Singleton object
        //AddText(TEXT("Another instance of Singleton is running:\r\n"));
        //AddText(TEXT("--> Impossible to access application features.\r\n"));
        bExist = true;
    } 
    else if (iErrorCode == ERROR_ACCESS_DENIED)
    {
        //AddText(TEXT("we cannot accesee the object in the namaspace.\r\nAnother instance of Singleton is running:\r\n "));
        //AddText(TEXT("--> Impossible to access application features.\r\n"));
        //权限不够，只能open，不能create。
        //需要满足CreatePrivateNamespace的第一个参数权限
        bExist = true;
    }
    else  {
        // First time the Singleton object is created
        //AddText(TEXT("First instance of Singleton:\r\n"));
        //AddText(TEXT("--> Access application features now.\r\n"));
    }
    return bExist;
}

