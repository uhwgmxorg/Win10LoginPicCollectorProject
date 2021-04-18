#pragma once

#include <string>
#include <vector>

#define SZ_VALUE_SIZE 300

using namespace std;

class AFX_EXT_CLASS CAppSettings
{
public:
    // .ini file sections
    // [Window] section
    static long m_lLeft;
    static long m_lTop;
    static long m_lRight;
    static long m_lBottom;

    // [App] section
    static wstring m_strSourcePath;
    static wstring m_strDestinationPath;

private:
    // Other member vars
    static wstring m_strIniFlileName;
    static wstring m_strSourcePathIni;
    static wstring m_strDestinationPathIni;

public:
    CAppSettings();
    CAppSettings(std::wstring intFile);
    ~CAppSettings();
    static unsigned int LoadConfig();
    static unsigned int SaveConfig();
    static void ResetIniFile();
};

