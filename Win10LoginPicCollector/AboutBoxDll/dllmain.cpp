// dllmain.cpp : Defines the initialization routines for the DLL.
//

#include "pch.h"
#include "framework.h"
#include <afxwin.h>
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE AboutBoxDllDLL = { false, nullptr };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved.
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("AboutBoxDll.DLL wird initialisiert.\n");

		// One-time initialization of the extension DLL
		if (!AfxInitExtensionModule(AboutBoxDllDLL, hInstance))
			return 0;

		// Insert this DLL into resource chain.
		// NOTE: If this extension DLL is implicitly replaced by
		// a regular MFC DLL (e.g. an ActiveX control)
		// instead of an MFC application, then you should include
		// remove the following lines from DllMain, and put them into a separate
		// function exported from the extension DLL.  The regular DLL,
		// that uses this extension DLL should then explicitly call the
		// function to initialize the extension DLL.  Otherwise
		// the CDynLinkLibrary object will not be connected to the resource chain of the // regular DLL.
		// regular DLL, which can lead to serious problems.
		// which can lead to serious problems.

		new CDynLinkLibrary(AboutBoxDllDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("AboutBoxDll.DLL wird beendet.\n");

		// Close library before calling destructors.
		AfxTermExtensionModule(AboutBoxDllDLL);
	}
	return 1;   // OK
}
