#pragma once
#include <Windows.h>
#include <Winnt.h>
#include <Shlobj.h>

//typedef enum  { 
//    TokenElevationTypeDefault  = 1, //������Ĭ���û����У�����UAC������
//    TokenElevationTypeFull,               //���̵�Ȩ�ޱ�������������û�б�ɸѡ��
//    TokenElevationTypeLimited         //����Ȩ�����У����Ʊ�ɸѡ
//} TOKEN_ELEVATION_TYPE , *PTOKEN_ELEVATION_TYPE;


/*
��windows 7�ϣ������ǰ��¼�˺�Ϊ����Ա�˺ţ���ϵͳ����������������Ƹ��û���
һ�������޵ķ�������(��ӦTokenElevationTypeLimited)��
һ��������������ԱȨ�޵ķ�������(��ӦTokenElevationTypeFull����
�������û���Թ���Ա������У������������޵ķ������ƹ���������������һ��������
����ǰ��¼�˺�Ϊ��׼�û�����ϵͳֻ����û�һ��Ĭ�Ϸ������ƣ���ӦTokenElevationTypeDefault����
*/
class AccessControl
{
public:
    AccessControl(void);
    virtual ~AccessControl(void);
public:
	bool getProcessElevation(TOKEN_ELEVATION_TYPE* pElevationType, BOOL* pIsAdmin);
	DWORD StartElevatedProcess(LPCTSTR szExecutable, LPCTSTR szCmdLine);
private:
    //BOOL bAdmin;
    TOKEN_ELEVATION_TYPE elevationType;
};

