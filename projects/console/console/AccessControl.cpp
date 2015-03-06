#include "StdAfx.h"
#include "AccessControl.h"


AccessControl::AccessControl(void):elevationType(TokenElevationTypeDefault)
{
	//bInitSucess = getProcessElevation(&elevationType, &bAdmin);
}


AccessControl::~AccessControl(void)
{
}
/*
��windows 7�ϣ������ǰ��¼�˺�Ϊ����Ա�˺ţ���ϵͳ����������������Ƹ��û���
һ�������޵ķ�������(��ӦTokenElevationTypeLimited)��
һ��������������ԱȨ�޵ķ�������(��ӦTokenElevationTypeFull����
�������û���Թ���Ա������У������������޵ķ������ƹ���������������һ��������
����ǰ��¼�˺�Ϊ��׼�û�����ϵͳֻ����û�һ��Ĭ�Ϸ������ƣ���ӦTokenElevationTypeDefault����
*/

//pIsAdmin  �Ƿ��ǹ���Ա
//pElevationType�����TokenElevationTypeLimited, ����*pIsAdminΪtrue, ��˵��admin��ʹ��asInvoker���н���
//�����ں�������
bool AccessControl::getProcessElevation(TOKEN_ELEVATION_TYPE* pElevationType, BOOL* pIsAdmin) {

	HANDLE hToken = NULL;
	DWORD dwSize; 

	// Get current process token
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
		return(false);

	bool bResult = false;

	// Retrieve elevation type information 
	if (GetTokenInformation(hToken, TokenElevationType, 
		pElevationType, sizeof(TOKEN_ELEVATION_TYPE), &dwSize)) {
			// Create the SID corresponding to the Administrators group
			byte adminSID[SECURITY_MAX_SID_SIZE];
			dwSize = sizeof(adminSID);
			CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, &adminSID, 
				&dwSize);

			if (*pElevationType == TokenElevationTypeLimited) {
				// Get handle to linked token (will have one if we are lua)
				HANDLE hUnfilteredToken = NULL;
				GetTokenInformation(hToken, TokenLinkedToken, (VOID*) 
					&hUnfilteredToken, sizeof(HANDLE), &dwSize);

				// Check if this original token contains admin SID
				if (CheckTokenMembership(hUnfilteredToken, &adminSID, pIsAdmin)) {
					*pIsAdmin = true;
					bResult = true;
				}

				// Don't forget to close the unfiltered token
				CloseHandle(hUnfilteredToken);
			} else {
				*pIsAdmin = IsUserAnAdmin();
				bResult = true;
			}
	}

	// Don't forget to close the process token
	CloseHandle(hToken);

	return bResult;
}

/*
����
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

*/


//ʹ��Button_SetElevationRequiredState(GetDlgItem(IDC_BUTTON1)->GetSafeHwnd(), TRUE);
//���Ը�һ����ť��Ӷ���ͼ��

//ʹ��StartElevatedProcess����ʹprocess�Ը�Ȩ������

//���������������, ��StartElevatedProcess,Ȼ��ExitProcess
DWORD AccessControl::StartElevatedProcess(LPCTSTR szExecutable, LPCTSTR szCmdLine) {

	// Initialize the structure.
	SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };

	// Ask for privileges elevation.
	sei.lpVerb = TEXT("runas");

	// Pass the application to start with high privileges.
	sei.lpFile = szExecutable;

	// Pass the command line.
	sei.lpParameters = szCmdLine;

	// Don't forget this parameter otherwise the window will be hidden.
	sei.nShow = SW_SHOWNORMAL;

	ShellExecuteEx(&sei);
	return(GetLastError());
}

//BOOL IsUserAdmin(VOID)
//	/*++ 
//	Routine Description: This routine returns TRUE if the caller's
//	process is a member of the Administrators local group. Caller is NOT
//	expected to be impersonating anyone and is expected to be able to
//	open its own process and process token. 
//	Arguments: None. 
//	Return Value: 
//	TRUE - Caller has Administrators local group. 
//	FALSE - Caller does not have Administrators local group. --
//	*/ 
//{
//	BOOL b;
//	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
//	PSID AdministratorsGroup; 
//	b = AllocateAndInitializeSid(
//		&NtAuthority,
//		2,
//		SECURITY_BUILTIN_DOMAIN_RID,
//		DOMAIN_ALIAS_RID_ADMINS,
//		0, 0, 0, 0, 0, 0,
//		&AdministratorsGroup); 
//	if(b) 
//	{
//		if (!CheckTokenMembership( NULL, AdministratorsGroup, &b)) 
//		{
//			b = FALSE;
//		} 
//		FreeSid(AdministratorsGroup); 
//	}
//
//	return(b);
//}
