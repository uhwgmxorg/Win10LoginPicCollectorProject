
// Win10LoginPicCollector.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CWin10LoginPicCollectorApp:
// See Win10LoginPicCollector.cpp for the implementation of this class
//

class CWin10LoginPicCollectorApp : public CWinApp
{
public:
	CWin10LoginPicCollectorApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	afx_msg void OnButtonReloadDestination();
	afx_msg void OnButtonReloadSource();
};

extern CWin10LoginPicCollectorApp theApp;
