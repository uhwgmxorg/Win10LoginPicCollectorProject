#include "pch.h"
#include "AppSettings.h"
#include "ToolsDll.h"

long CAppSettings::m_lLeft = 10;
long CAppSettings::m_lTop = 10;
long CAppSettings::m_lRight = 410;
long CAppSettings::m_lBottom = 210;

wstring CAppSettings::m_strSourcePath = L"C:\\Users\\%USERNAME%\\AppData\\Local\\Packages\\Microsoft.Windows.ContentDeliveryManager_cw5n1h2txyewy\\LocalState\\Assets";
wstring CAppSettings::m_strDestinationPath = L"C:\\DestinationPath";

// Initial values
wstring CAppSettings::m_strIniFlileName = L"Win10LoginPicCollector.ini";
wstring CAppSettings::m_strSourcePathIni = L"C:\\Users\\%USERNAME%\\AppData\\Local\\Packages\\Microsoft.Windows.ContentDeliveryManager_cw5n1h2txyewy\\LocalState\\Assets";
wstring CAppSettings::m_strDestinationPathIni = L"C:\\DestinationPath";

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
CAppSettings::CAppSettings(wstring intFile)
{
    m_strIniFlileName = intFile;
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
    wstring exe_path(CToolsDllApp::GetExePath() + L"\\");
    wchar_t szValue[SZ_VALUE_SIZE];
    wchar_t szIniFile[MAX_PATH];
    wcscpy_s(szIniFile, (exe_path + m_strIniFlileName).c_str());


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
    wstring exe_path(CToolsDllApp::GetExePath() + L"\\");
    wchar_t szValue[SZ_VALUE_SIZE];
    wchar_t szIniFile[MAX_PATH];
    wcscpy_s(szIniFile, (exe_path + m_strIniFlileName).c_str());

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
/// ResetIniFile
/// </summary>
void CAppSettings::ResetIniFile()
{
    DeleteFileW(m_strIniFlileName.c_str());
    m_strSourcePath = m_strSourcePathIni;
    m_strDestinationPath = m_strDestinationPathIni;
    SaveConfig();
}
