// ToolsDll.cpp: Defines the initialization routines for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "ToolsDll.h"

#include <filesystem>

// please refer: 
// https://github.com/nothings/stb
#define STBI_WINDOWS_UTF8
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
/// FileExsist
/// </summary>
/// <param name="lpszFilename"></param>
/// <returns></returns>
bool CToolsDllApp::FileExsist(const wchar_t* lpszFilename)
{
    return _waccess(lpszFilename, 0) == 0;
}

/// <summary>
/// CreateDir
/// </summary>
/// <param name="lpszDirname"></param>
/// <returns></returns>
bool CToolsDllApp::CreateDir(const wchar_t* lpszDirname)
{
    return CreateDirectory(lpszDirname, NULL);
}

/// <summary>
/// CreateDir
/// </summary>
/// <param name="dirname"></param>
/// <returns></returns>
bool CToolsDllApp::CreateDir(wstring dirname)
{
    return CreateDirectory(dirname.c_str(), NULL);
}


/// <summary>
/// GetExePath
/// Get the path of the running exe
/// </summary>
/// <returns></returns>
wstring CToolsDllApp::GetExePath()
{
	wchar_t path[MAX_PATH] = { 0 };
	GetModuleFileNameW(NULL, path, MAX_PATH);

	filesystem::path exe_path(path);

	return wstring(exe_path.parent_path());
}

/// <summary>
/// GetUserPath
/// </summary>
/// <returns></returns>
wstring CToolsDllApp::GetUserPath()
{
    wchar_t path[MAX_PATH] = { 0 };
    
    ::GetEnvironmentVariableW(L"USERPROFILE",path,MAX_PATH);

    return wstring(path);
}

/// <summary>
/// NumberOfFilesIn
/// </summary>
/// <param name="lpszPath"></param>
/// <returns></returns>
int CToolsDllApp::NumberOfFilesIn(const wchar_t* lpszPath)
{
    int counter = 0;
    WIN32_FIND_DATA ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    ffd.dwFileAttributes = 0;
    ffd.ftCreationTime.dwHighDateTime = 0;
    ffd.ftCreationTime.dwLowDateTime = 0;
    ffd.ftLastAccessTime.dwHighDateTime = 0;
    ffd.ftLastAccessTime.dwLowDateTime = 0;
    ffd.ftLastWriteTime.dwHighDateTime = 0;
    ffd.ftLastWriteTime.dwLowDateTime = 0;
    ffd.nFileSizeHigh = 0;
    ffd.nFileSizeLow = 0;
    ffd.dwReserved0 = 0;
    ffd.dwReserved1 = 0;
    ffd.cFileName;
    ffd.cAlternateFileName;

    // Start iterating over the files in the path directory.
    hFind = ::FindFirstFileW(lpszPath, &ffd);

    DWORD dwerr = GetLastError();
    // Get the last Error in text
    wchar_t buf[256];
    FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, dwerr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
    TRACE(_T("Failed on ::FindFirstFileW LastError LastError %u %s\n"), dwerr, buf);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        do // Managed to locate and create an handle to that folder.
        {
            counter++;
        } while (::FindNextFile(hFind, &ffd) == TRUE);
        ::FindClose(hFind);
    }
    else
    {
        TRACE(_T("Failed to find path: %s"), lpszPath);
    }

    return counter;
}

/// <summary>
/// GetAllFilesInDir
/// </summary>
/// <param name="lpszPath"></param>
vector<wstring> CToolsDllApp::GetAllFilesInDir(const wchar_t* lpszPath)
{
    vector<wstring> fileNames;
    wstring search_path = L"*.";
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFileW(lpszPath, &fd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            // read all (real) files in current folder
            // , delete '!' read other 2 default folder . and ..
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                wstring ws(fd.cFileName);
                wstring str(ws.begin(), ws.end());
                fileNames.push_back(str);
            }
        } while (::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }
    return fileNames;
}

/// <summary>
/// GetPath
/// Get a File path from a dialog file picker
/// </summary>
/// <param name="lpszPath"></param>
/// <returns></returns>
wstring CToolsDllApp::GetPath(const wchar_t* lpszPath)
{
    CString cstrPath;

    CFolderPickerDialog* pdlgPickFolder = new CFolderPickerDialog(lpszPath);
    if (pdlgPickFolder->DoModal() == IDOK)
    {
        cstrPath = pdlgPickFolder->GetPathName();
        cstrPath += L"\\";
    }
    else
    {
        cstrPath = lpszPath;
    }
    delete pdlgPickFolder;


    wstring strPath((LPCTSTR)cstrPath);

    // Just for debuging
    //::MessageBox(NULL, L"The selected path is: " + cstrPath, L"For Debuging", MB_OK);

    return strPath;
}

/// <summary>
/// GetMyUserName
/// </summary>
/// <returns></returns>
wstring CToolsDllApp::GetMyUserName()
{
    wchar_t username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    GetUserNameW(username, &username_len);

    return wstring(username);
}

/// <summary>
/// CopyFile
/// </summary>
/// <param name="lpszSourceFile"></param>
/// <param name="lpszDestinationFile"></param>
void CToolsDllApp::CopyFiles(const wchar_t* lpszSourceFile, const wchar_t* lpszDestinationFile)
{
    TRACE(_T("Copy files from %s to %s\n"), lpszSourceFile, lpszDestinationFile);

    wstring path_file(lpszSourceFile);
    // Replace the %USERNAME% with the real username if containing
    if (path_file.find(L"%USERNAME%") != -1)
        path_file.replace(path_file.find(L"%USERNAME%"), wcslen(L"%USERNAME%"), CToolsDllApp::GetMyUserName().c_str());

    vector<wstring> files = GetAllFilesInDir((path_file + L"\\*.*").c_str());
    wstring s_path = path_file;
    wstring d_path(lpszDestinationFile);
    wstring s_file;
    wstring d_file;

    for (auto const& file : files)
    {
        s_file = s_path + file;
        d_file = d_path + file + L".jpg";

#pragma region stb_image.h action 
        // This section opens the image with stb_image.h 
        // and loads the properties of the image
        char szImageFileName[FILENAME_MAX];
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        WideCharToMultiByte(CP_UTF8, 0, s_file.c_str(), -1, szImageFileName, FILENAME_MAX, NULL, NULL);
        unsigned char* image = stbi_load(szImageFileName, &width, &height, &channels, STBI_rgb);
        stbi_image_free(image);
#pragma endregion

        bool copyImage = false;

        // Only copy images in landscape format and with a
        // resolution greater or equal than 1920 X 1080
        if (width >= height && width >= 1920 && height >= 1080)
            copyImage = true;

        if (copyImage)
        {
            ::CopyFile(s_file.c_str(), d_file.c_str(), FALSE);

            DWORD dwerr = GetLastError();
            // Get the last Error in text
            wchar_t buf[256];
            FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, dwerr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
            TRACE(_T("LastError on ::CopyFile LastError: %u %s\n"), dwerr, buf);
        }
    }
}