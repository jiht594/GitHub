
// Asqare.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAsqareApp:
// �йش����ʵ�֣������ Asqare.cpp
//

class CAsqareApp : public CWinApp
{
public:
	CAsqareApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAsqareApp theApp;