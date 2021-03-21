// ToolsDll.h: Main header file for the ToolsDll DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "'pch.h' vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"		// Main symbols


// CToolsDllApp
// For information about the implementation of this class, see ToolsDll.cpp.
//

class CToolsDllApp : public CWinApp
{
public:
	CToolsDllApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
