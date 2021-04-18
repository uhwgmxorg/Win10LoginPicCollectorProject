// ToolsDll.h: Main header file for the ToolsDll DLL
//

#pragma once

#include <string>
#include <vector>

using namespace std;

#ifndef __AFXWIN_H__
	#error "'pch.h' vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"		// Main symbols


// CToolsDllApp
// For information about the implementation of this class, see ToolsDll.cpp.
//

class AFX_EXT_CLASS CToolsDllApp : public CWinApp
{
public:
	CToolsDllApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

public:
	static bool FileExsist(const wchar_t* lpszFilename);
	static bool CreateDir(const wchar_t* lpszFilename);
	static wstring GetExePath();
	static int NumberOfFilesIn(const wchar_t* lpszPath);
	static vector<wstring> GetAllFilesInDir(const wchar_t* lpszPath);
	static wstring GetPath(const wchar_t* lpszPath = NULL);
	static wstring GetMyUserName();
	static void CopyFile(const wchar_t* lpszSourceFile, const wchar_t* lpszDestinationFile);
};
