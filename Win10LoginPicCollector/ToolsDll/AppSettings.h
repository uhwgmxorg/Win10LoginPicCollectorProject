#pragma once

#include <string>
#include <vector>

#define SZ_VALUE_SIZE 300

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
    static std::wstring m_strSourcePath;
    static std::wstring m_strDestinationPath;

private:
    // Other member vars
    static std::wstring m_strIniFlileName;
    static std::wstring m_strSourcePathIni;
    static std::wstring m_strDestinationPathIni;

public:
    CAppSettings();
    CAppSettings(std::wstring intFile);
    ~CAppSettings();
    static unsigned int LoadConfig();
    static unsigned int SaveConfig();

    static bool FileExsist(const wchar_t* lpszFilename);
    static bool CreateDir(const wchar_t* lpszFilename);
    static void ResetIniFile();
    static int NumberOfFilesIn(const wchar_t* lpszPath);
    static std::vector<std::wstring> GetAllFilesInDir(const wchar_t* lpszPath);
    static std::wstring GetPath(const wchar_t* lpszPath = NULL);
    static std::wstring GetMyUserName();
    static void CopyFile(const wchar_t* lpszSourceFile, const wchar_t* lpszDestinationFile);
};

