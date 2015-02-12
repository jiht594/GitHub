// BCGtest.h : main header file for the BCGtest application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CBCGtestApp:
// See BCGtest.cpp for the implementation of this class
//

class CBCGtestApp : public CBCGPWinApp
{
public:
	CBCGtestApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBCGtestApp theApp;