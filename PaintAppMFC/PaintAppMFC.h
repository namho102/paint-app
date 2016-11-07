
// PaintAppMFC.h : main header file for the PaintAppMFC application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CPaintAppMFCApp:
// See PaintAppMFC.cpp for the implementation of this class
//

class CPaintAppMFCApp : public CWinApp
{
public:
	CPaintAppMFCApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPaintAppMFCApp theApp;
