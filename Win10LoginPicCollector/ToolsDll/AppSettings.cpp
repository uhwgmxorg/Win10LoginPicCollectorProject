#include "pch.h"
#include "AppSettings.h"

long CAppSettings::m_lLeft = 10;
long CAppSettings::m_lTop = 10;
long CAppSettings::m_lRight = 410;
long CAppSettings::m_lBottom = 210;

std::wstring CAppSettings::m_strSourcePath = L"C:\\Users\\%USERNAME%\\AppData\\Local\\Packages\\Microsoft.Windows.ContentDeliveryManager_cw5n1h2txyewy\\LocalState\\Assets";
std::wstring CAppSettings::m_strDestinationPath = L"C:\\DestinationPath";

std::wstring CAppSettings::m_IniFlileName = L".\\Win10LoginPicCollector.ini";
std::wstring CAppSettings::m_strSourcePathIni = L"C:\\Users\\%USERNAME%\\AppData\\Local\\Packages\\Microsoft.Windows.ContentDeliveryManager_cw5n1h2txyewy\\LocalState\\Assets";

/// <summary>
/// Constructor
/// </summary>
CAppSettings::CAppSettings()
{
    LoadConfig();
}

/// <summary>
/// Constructor
/// </summary>
/// <param name="intFile"></param>
CAppSettings::CAppSettings(std::wstring intFile)
{
    m_IniFlileName = intFile;
    LoadConfig();
}

/// <summary>
/// Destructor
/// </summary>
CAppSettings::~CAppSettings()
{
    SaveConfig();
}

/// <summary>
/// LoadConfig
/// </summary>
/// <returns>lastError the last Win 32 Error</returns>
unsigned int CAppSettings::LoadConfig()
{
    wchar_t szValue[SZ_VALUE_SIZE];

    wchar_t szIniFile[MAX_PATH];
    wcscpy_s(szIniFile, m_IniFlileName.c_str());

    // [Window] section
    m_lLeft = GetPrivateProfileIntW(L"Window", L"Left", m_lLeft, szIniFile);
    m_lTop = GetPrivateProfileIntW(L"Window", L"Top", m_lTop, szIniFile);
    m_lRight = GetPrivateProfileIntW(L"Window", L"Right", m_lRight, szIniFile);
    m_lBottom = GetPrivateProfileIntW(L"Window", L"Bottom", m_lBottom, szIniFile);

    // [App] section
    GetPrivateProfileStringW(L"App", L"SourcePath", m_strSourcePath.c_str(), szValue, SZ_VALUE_SIZE, szIniFile);
    m_strSourcePath = szValue;
    GetPrivateProfileStringW(L"App", L"DestinationPath", m_strDestinationPath.c_str(), szValue, SZ_VALUE_SIZE, szIniFile);
    m_strDestinationPath = szValue;

    DWORD dwerr = GetLastError();
    // Get the last Error in text
    wchar_t buf[256];
    FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, dwerr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
    TRACE(_T("Failed on LoadConfig LastError LastError %u %s\n"), dwerr, buf);

    // if file not found then create it with the initial values
    if (dwerr == 2) 
        SaveConfig();

    return dwerr;
}

/// <summary>
/// SaveConfig
/// </summary>
/// <returns>lastError the last Win 32 Error</returns>
unsigned int CAppSettings::SaveConfig()
{
    bool flag;
    wchar_t szValue[SZ_VALUE_SIZE];

    wchar_t szIniFile[MAX_PATH];
    wcscpy_s(szIniFile, m_IniFlileName.c_str());


#pragma warning(disable : 4996) // error C4996: 'swprintf': function has been changed to conform with the ISO C standard, adding an extra character count parameter.

    // .ini file sections
    // [Window] section
    swprintf(szValue, L"%d", m_lLeft);
    flag = WritePrivateProfileStringW(L"Window", L"Left", szValue, szIniFile);
    swprintf(szValue, L"%d", m_lTop);
    flag = WritePrivateProfileStringW(L"Window", L"Top", szValue, szIniFile);
    swprintf(szValue, L"%d", m_lRight);
    flag = WritePrivateProfileStringW(L"Window", L"Right", szValue, szIniFile);
    swprintf(szValue, L"%d", m_lBottom);
    flag = WritePrivateProfileStringW(L"Window", L"Bottom", szValue, szIniFile);

    // [App] section
    swprintf(szValue, L"%s", m_strSourcePath.c_str());
    flag = WritePrivateProfileStringW(L"App", L"SourcePath", szValue, szIniFile);
    swprintf(szValue, L"%s", m_strDestinationPath.c_str());
    flag = WritePrivateProfileStringW(L"App", L"DestinationPath", szValue, szIniFile);

#pragma warning(default:4996)


    unsigned int lastError = GetLastError();

    return lastError;
}

/// <summary>
/// FileExsist
/// </summary>
/// <param name="lpszFilename"></param>
/// <returns></returns>
bool CAppSettings::FileExsist(const wchar_t* lpszFilename)
{
    return _waccess(lpszFilename, 0) == 0;
}

/// <summary>
/// CreateDir
/// </summary>
/// <param name="lpszDirname"></param>
/// <returns></returns>
bool CAppSettings::CreateDir(const wchar_t* lpszDirname)
{
    return CreateDirectory(lpszDirname, NULL);
}

/// <summary>
/// ResetIniFile
/// </summary>
void CAppSettings::ResetIniFile()
{
    DeleteFileW(m_IniFlileName.c_str());
    m_strSourcePath = m_strSourcePathIni;
    SaveConfig();
}

/// <summary>
/// NumberOfFilesIn
/// </summary>
/// <param name="lpszPath"></param>
/// <returns></returns>
int CAppSettings::NumberOfFilesIn(const wchar_t* lpszPath)
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
std::vector<std::wstring> CAppSettings::GetAllFilesInDir(const wchar_t* lpszPath)
{
    std::vector<std::wstring> fileNames;
    std::wstring search_path = L"*.";
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFileW( lpszPath,&fd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            // read all (real) files in current folder
            // , delete '!' read other 2 default folder . and ..
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                std::wstring ws(fd.cFileName);
                std::wstring str(ws.begin(), ws.end());
                fileNames.push_back(str);
            }
        } while (::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }
    return fileNames;
}

/// <summary>
/// GetPath
/// Get a File path from 
/// </summary>
/// <param name="lpszPath"></param>
/// <returns></returns>
std::wstring CAppSettings::GetPath(const wchar_t* lpszPath)
{
    CString cstrPath;

    CFolderPickerDialog* pdlgPickFolder = new CFolderPickerDialog(lpszPath);
    pdlgPickFolder->DoModal();
    cstrPath = pdlgPickFolder->GetPathName();
    delete pdlgPickFolder;

    cstrPath += L"\\";

    std::wstring strPath((LPCTSTR)cstrPath);

    // Just for debuging
    //::MessageBox(NULL, L"The selected path is: " + cstrPath, L"For Debuging", MB_OK);

    return strPath;
}

/// <summary>
/// CopyFile
/// </summary>
/// <param name="lpszSourceFile"></param>
/// <param name="lpszDestinationFile"></param>
void CAppSettings::CopyFile(const wchar_t* lpszSourceFile, const wchar_t* lpszDestinationFile)
{

}