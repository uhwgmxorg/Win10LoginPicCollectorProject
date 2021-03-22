// ToolsDll.cpp: Defines the initialization routines for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "ToolsDll.h"

#include <filesystem>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked to MFC DLLs,
//      all functions exported from this DLL which are called in
//      MFC, the AFX_MANAGE_STATE macro must be // added to the beginning of the function.
//      must be added to the beginning of the function.
//
// Example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// Hier normaler Funktionsrumpf
//		}
//
//      It is very important that this macro is displayed in every function
//      before all MFC calls.  This means,
//      that it must be displayed as the first statement within the
//      function, even before any declarations of object variables,
//      since their constructors might generate calls to the MFC // DLL.
//      could.
//
//      See the technical notes for MFC 33 and 58 for more // details.
//      details.
//

// CToolsDllApp

BEGIN_MESSAGE_MAP(CToolsDllApp, CWinApp)
END_MESSAGE_MAP()


// CToolsDllApp creation

CToolsDllApp::CToolsDllApp()
{
	// TODO: Insert code for construction here.
	// Position all important initializations in InitInstance.
}


// The only CToolsDllApp object

CToolsDllApp theApp;


// CToolsDllApp initialization

BOOL CToolsDllApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

/// <summary>
/// GetExePath
/// Get the path of the running exe
/// </summary>
/// <returns></returns>
std::wstring CToolsDllApp::GetExePath()
{
	wchar_t path[MAX_PATH] = { 0 };
	GetModuleFileNameW(NULL, path, MAX_PATH);

	std::filesystem::path exe_path(path);

	return std::wstring(exe_path.parent_path());
}
