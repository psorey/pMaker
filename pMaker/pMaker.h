// pMaker.h : main header file for the pMaker application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CpMakerApp:
// See pMaker.cpp for the implementation of this class
//

class CpMakerApp : public CWinApp
{
public:
	CpMakerApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CpMakerApp theApp;