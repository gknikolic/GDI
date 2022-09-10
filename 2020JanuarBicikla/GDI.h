
// MFCKol1_2018.h : main header file for the MFCKol1_2018 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMFCKol12018App:
// See MFCKol1_2018.cpp for the implementation of this class
//

class CMFCKol12018App : public CWinApp
{
public:
	CMFCKol12018App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFCKol12018App theApp;
