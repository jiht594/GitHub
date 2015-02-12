#pragma once

static int const NAMESPACE_CREATE_SUCCESS = 0;
//int const NAMESPACE_CREATE_FAIL = 1;
static int const NAMESPACE_OPEN_SUCCESS = 2;
static int const NAMESPACE_OPEN_FAIL = 3;
static int const SID_CREATE_FAIL = 4;
static int const ADD_SID_TO_BOUNDARY_FAIL = 5;
static int const SD_CREATE_FAIL = 6;
static int const NAMESPACE_ACCESS_DENY = 7;
static HANDLE g_hSingleton = NULL;
class SingletonApp
{
public:
    SingletonApp(void);
    ~SingletonApp(void);
public:
    static bool isSingleApp(const TCHAR *spaceName, const TCHAR * mutexName);
    static void release()
    {
        if (g_hSingleton)
        {
            CloseHandle(g_hSingleton);
            g_hSingleton = NULL;
        }
    }
private:
    static int createNamespace(const TCHAR *spaceName);
    static bool checkSingletonObject(const TCHAR *spaceName, const TCHAR * mutexName);
};

