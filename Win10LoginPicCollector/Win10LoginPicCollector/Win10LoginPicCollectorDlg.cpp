
// Win10LoginPicCollectorDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "afxdialogex.h"

#define SPDLOG_WCHAR_TO_UTF8_SUPPORT 
#include "../spdlog/include/spdlog/spdlog.h"
#include <spdlog/sinks/rotating_file_sink.h>

#include "..\AboutBoxDll\AboutBox.h"

#include "Win10LoginPicCollector.h"
#include "Win10LoginPicCollectorDlg.h"

#define	THUMBNAIL_WIDTH		100
#define	THUMBNAIL_HEIGHT	90

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static UINT BASED_CODE indicators[] =
{
	ID_INDICATOR_PANE,
	ID_INDICATOR_TIME
};


// CWin10LoginPicCollectorDlg dialog

/// <summary>
/// Constructor
/// </summary>
/// <param name="pParent"></param>
CWin10LoginPicCollectorDlg::CWin10LoginPicCollectorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WIN10LOGINPICCOLLECTOR_DIALOG, pParent)
	, m_iNumFiles(0)
	, m_strSourcePath(_T(""))
	, m_strDestinationPath(_T(""))
	, m_strCoutFilesSource(_T(""))
	, m_strCoutFilesDestination(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

#pragma region log in file
	wstring wuser_path = CToolsDllApp::GetUserPath();
	string user_path(wuser_path.begin(),wuser_path.end());
    user_path +=  "\\Win10LoginPicCollectorProject";
	auto logger = spdlog::rotating_logger_mt("Win10LoginPicCollector", user_path + "\\" + "LogFile.log", 1048576, 9);
	spdlog::set_default_logger(logger);
	spdlog::flush_on(spdlog::level::trace);
#pragma endregion

	spdlog::set_level(spdlog::level::trace);
	spdlog::info("Start  Win10LoginPicCollector");
}

/// <summary>
/// DoDataExchange
/// </summary>
/// <param name="pDX"></param>
void CWin10LoginPicCollectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_STATIC_NUMFILES, m_iNumFiles);
	DDV_MinMaxInt(pDX, m_iNumFiles, 0, 99999);
	DDX_Control(pDX, IDC_LIST2, m_listBox);
	DDX_Text(pDX, IDC_STATIC_SOURCE_PATH, m_strSourcePath);
	DDX_Text(pDX, IDC_STATIC_DESTINATION_PATH, m_strDestinationPath);
	DDX_Control(pDX, IDC_LISTCTRL_DESTINATION, m_ctrlDestination);
	DDX_Text(pDX, IDC_STATIC_SOURCE_COUNT_FILES, m_strCoutFilesSource);
	DDX_Text(pDX, IDC_STATIC_DESTINATION_COUNT_FILES, m_strCoutFilesDestination);
}

BEGIN_MESSAGE_MAP(CWin10LoginPicCollectorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_BUTTON_EXIT, &CWin10LoginPicCollectorDlg::OnButtonExit)
	ON_COMMAND(ID_BUTTON_SELECT_SOURCE_PATH, &CWin10LoginPicCollectorDlg::OnButtonSelectSourcePath)
	ON_COMMAND(ID_BUTTON_EDIT_INI, &CWin10LoginPicCollectorDlg::OnButtonEditIni)
	ON_COMMAND(ID_BUTTON_ABOUT, &CWin10LoginPicCollectorDlg::OnButtonAbout)
	ON_COMMAND(ID_BUTTON_SELECT_DESTINATION_PATH, &CWin10LoginPicCollectorDlg::OnButtonSelectDestinationPath)
	ON_COMMAND(ID_BUTTON_RESET_CONFIG, &CWin10LoginPicCollectorDlg::OnButtonResetConfig)
	ON_BN_CLICKED(ID_BUTTON_COPY, &CWin10LoginPicCollectorDlg::OnClickedButtonCopy)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY(NM_CLICK, IDC_LISTCTRL_DESTINATION, &CWin10LoginPicCollectorDlg::OnNMClickListctrlDestination)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCTRL_DESTINATION, &CWin10LoginPicCollectorDlg::OnNMDblclkListctrlDestination)
	ON_BN_CLICKED(IDC_BUTTON_RELOAD_DESTINATION, &CWin10LoginPicCollectorDlg::OnClickedButtonReloadDestination)
	ON_BN_CLICKED(IDC_BUTTON_RELOAD_SOURCE, &CWin10LoginPicCollectorDlg::OnClickedButtonReloadSource)
END_MESSAGE_MAP()

/// <summary>
/// OnInitDialog
/// </summary>
/// <returns></returns>
BOOL CWin10LoginPicCollectorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_strVersion = L"1.0.0.8";

#pragma region Set a Bold Font in Dlg Statics
	// Get current font.
	CFont* pFont = GetDlgItem(IDC_STATIC_SOURCE)->GetFont();
	LOGFONT LogFont = { 0 };
	pFont->GetLogFont(&LogFont);
	LogFont.lfWeight = 700;
	CFont staticFont;
	staticFont.CreateFontIndirect(&LogFont);
	// Sets the new font back to static text.
	GetDlgItem(IDC_STATIC_SOURCE)->SetFont(&staticFont);
	GetDlgItem(IDC_STATIC_SOURCE_COUNT_FILES)->SetFont(&staticFont);
	GetDlgItem(IDC_STATIC_FILESS)->SetFont(&staticFont);
    GetDlgItem(IDC_STATIC_DESTINATION)->SetFont(&staticFont);
	GetDlgItem(IDC_STATIC_DESTINATION_COUNT_FILES)->SetFont(&staticFont); 
    GetDlgItem(IDC_STATIC_FILESD)->SetFont(&staticFont);
#pragma endregion

#pragma region Add ToolBar to Dlg
	// Create the ToolBar
	if (!m_ctrlToolBar1.Create(this) || !m_ctrlToolBar1.LoadToolBar(IDR_TOOLBAR1))
	{
		// If we run into an error do this:
		DWORD dwerr = GetLastError();
		// Get the last Error in text
		wchar_t buf[256];
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dwerr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL);

		TRACE(_T("Failed to Create Dialog Toolbar Error %s\n"), buf);
		EndDialog(IDCANCEL);
	}

	CRect	rcClientOld; // Old Client Rect
	CRect	rcClientNew; // New Client Rect with Tollbar Added
	GetClientRect(rcClientOld); // Retrive the Old Client WindowSize
	// Called to reposition and resize control bars in the client area of a window
	// The reposQuery FLAG does not really traw the Toolbar.  It only does the calculations.
	// And puts the new ClientRect values in rcClientNew so we can do the rest of the Math.
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0, reposQuery, rcClientNew);

	// All of the Child Windows (Controls) now need to be moved so the Tollbar does not cover them up.
	// Offest to move all child controls after adding Tollbar
	CPoint ptOffset(rcClientNew.left - rcClientOld.left, rcClientNew.top - rcClientOld.top);

	CRect	rcChild;
	CWnd* pwndChild = GetWindow(GW_CHILD);  //Handle to the Dialog Controls
	while (pwndChild) // Cycle through all child controls
	{
		pwndChild->GetWindowRect(rcChild); //  Get the child control RECT
		ScreenToClient(rcChild);
		rcChild.OffsetRect(ptOffset); // Changes the Child Rect by the values of the claculated offset
		pwndChild->MoveWindow(rcChild, FALSE); // Move the Child Control
		pwndChild = pwndChild->GetNextWindow();
	}

	CRect	rcWindow;
	GetWindowRect(rcWindow); // Get the RECT of the Dialog
	rcWindow.right += rcClientOld.Width() - rcClientNew.Width();    // Increase width to new Client Width
	rcWindow.bottom += rcClientOld.Height() - rcClientNew.Height(); // Increase height to new Client Height
	MoveWindow(rcWindow, FALSE); // Redraw Window

	// Now we REALLY Redraw the Toolbar
	m_ctrlToolBar1.SetBarStyle(CBRS_ALIGN_TOP | CBRS_TOOLTIPS | CBRS_FLYBY);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
#pragma endregion

#pragma region Add StatusBar to Dlg
    // Here is what you need, to create a status bar
	m_StatusBar.Create(this);     //Create status bar
	m_StatusBar.SetIndicators(indicators, 2);

	// Find the Size of Dialog box
	CRect rect;
	GetClientRect(&rect);

	// Size the two panes
	m_StatusBar.SetPaneInfo(0, ID_INDICATOR_PANE, SBPS_NORMAL, rect.Width() - 60);
	m_StatusBar.SetPaneInfo(1, ID_INDICATOR_TIME, SBPS_STRETCH, 0);

	// This is where we actually draw it 
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, ID_INDICATOR_TIME);

	// Timer is Set to Update the Time on the status Bar.   
	SetTimer(100, 1000, NULL);
#pragma endregion

	// Load the App settings
	m_appSettings.LoadConfig();
	// Set the window position
	SetWindowPos(NULL, m_appSettings.m_lLeft, m_appSettings.m_lTop, m_appSettings.m_lRight - m_appSettings.m_lLeft, m_appSettings.m_lBottom - m_appSettings.m_lTop, SWP_SHOWWINDOW | SWP_NOSIZE);

#pragma region Set Version and architecture in Window Text
	// Set Version and architecture in Window Text
	CString strWindowText;
	CString strVersion = m_strVersion;
	CString strArchitecture;
	if (sizeof(void*) == 4)
		strArchitecture = "x32";
	else
		strArchitecture = "x64";
#ifdef DEBUG
	strWindowText.Format(L"Window 10 Login Picture Collector --- Debug Version %s %s", strVersion, strArchitecture);
#else
	strWindowText.Format(L"Window 10 Login Picture Collector --- Release %s %s", strVersion, strArchitecture);
#endif // DEBUG
	SetWindowText(strWindowText);
#pragma endregion

	// Set the LstCtrl Syle
	DWORD dwStyle = 0x00000000;
	dwStyle = LVS_REPORT;
	m_ctrlDestination.ModifyStyle(dwStyle, 0);
	DWORD dwExStyle = 0x00000000;
	dwExStyle = LVS_EX_AUTOSIZECOLUMNS;
	//m_ctrlDestination.SetExtendedStyle(dwExStyle);

	// Init GDI+
	// see pch.h
	GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, &m_gdiplusStartupOutput);

	InitSourceBranch();
	InitDestinationBranch();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

/// <summary>
/// OnSysCommand
/// </summary>
/// <param name="nID"></param>
/// <param name="lParam"></param>
void CWin10LoginPicCollectorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{

		// Status output
		CString strStatus;
		strStatus.Format(L"About Win10LoginPicCollectorDlg");
		m_StatusBar.SetPaneText(0, strStatus);

		CAboutBox aboutBox;
		aboutBox.InitParam();
		aboutBox.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWin10LoginPicCollectorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//------------------------------/
//-  ToolBar Button Events     -/
//------------------------------/
#pragma region My ToolBar Button Events

/// <summary>
/// OnButtonExit
/// </summary>
void CWin10LoginPicCollectorDlg::OnButtonExit()
{
	TRACE(_T("OnButtonExit was pressed\n"));
	SaveAppSettings();
	EndDialog(IDOK);
}

/// <summary>
/// OnButtonSelectSourcePath
/// </summary>
void CWin10LoginPicCollectorDlg::OnButtonSelectSourcePath()
{
	TRACE(_T("OnButtonSelectSourcePath was pressed\n"));

	wstring strPath = CToolsDllApp::GetPath(m_appSettings.m_strSourcePath.c_str());
	TRACE(_T("GetPath() = %s\n"), strPath.c_str());
	m_appSettings.m_strSourcePath = strPath;
	m_appSettings.SaveConfig();

	InitSourceBranch();
}

/// <summary>
/// OnButtonSelectDestinationPath
/// </summary>
void CWin10LoginPicCollectorDlg::OnButtonSelectDestinationPath()
{
	TRACE(_T("OnButtonSelectDestinationPath was pressed\n"));

	wstring strPath = CToolsDllApp::GetPath(m_appSettings.m_strDestinationPath.c_str());
	TRACE(_T("GetPath() = %s\n"), strPath.c_str());
	m_appSettings.m_strDestinationPath = strPath;
	m_appSettings.SaveConfig();

	InitDestinationBranch();
}

/// <summary>
/// OnButtonResetConfig
/// </summary>
void CWin10LoginPicCollectorDlg::OnButtonResetConfig()
{
	TRACE(_T("OnButtonResetConfig was pressed\n"));

	CAppSettings::ResetIniFile();
	InitSourceBranch();
	MessageBoxW((LPCWSTR)L"Ini-File reseted", (LPCWSTR)L"", MB_ICONINFORMATION | MB_OK);
}

/// <summary>
/// OnButtonEditIni
/// </summary>
void CWin10LoginPicCollectorDlg::OnButtonEditIni()
{
	TRACE(_T("OnButtonEditIni was pressed\n"));

	LoadIniFileInNotpad();
}

/// <summary>
/// OnButtonAbout
/// </summary>
void CWin10LoginPicCollectorDlg::OnButtonAbout()
{
	TRACE(_T("OnButtonAbout was pressed\n"));

	// Status output
	CString strStatus;
	strStatus.Format(L"About Win10LoginPicCollectorDlg");
	m_StatusBar.SetPaneText(0, strStatus);

	CAboutBox aboutBox;
	aboutBox.InitParam();
	aboutBox.DoModal();
}

#pragma endregion

//------------------------------/
//-       Button Events        -/
//------------------------------/
#pragma region My Button Events

/// <summary>
/// OnClickedButtonReloadDestination
/// </summary>
void CWin10LoginPicCollectorDlg::OnClickedButtonReloadDestination()
{
	InitDestinationBranch();
}

/// <summary>
/// OnClickedButtonReloadSource
/// </summary>
void CWin10LoginPicCollectorDlg::OnClickedButtonReloadSource()
{
	InitSourceBranch();
}

/// <summary>
/// OnClickedButtonCopy
/// </summary>
void CWin10LoginPicCollectorDlg::OnClickedButtonCopy()
{
	TRACE(_T("OnClickedButtonCopy was pressed\n"));
	CString strSPath = m_appSettings.m_strSourcePath.c_str();
	CString strDPath = m_appSettings.m_strDestinationPath.c_str();
	// Append a "\" at the end if we have no
	if (strSPath[strSPath.GetLength() - 1] == L'\\') strSPath += L""; else strSPath += L"\\";
	if (strSPath[strDPath.GetLength() - 1] == L'\\') strDPath += L""; else strDPath += L"\\";
	CToolsDllApp::CopyFiles(strSPath, strDPath);
	wstring log = L"Copy all files from " + strSPath + L" to " + strDPath;
	spdlog::info(log.c_str());
	InitDestinationBranch();
}

#pragma endregion

//------------------------------/
//-       Other Events         -/
//------------------------------/
#pragma region My Other Events

/// <summary>
/// PreTranslateMessage
/// prevent MFC dialog closing 
/// on Enter or Escape keys
/// </summary>
/// <param name="pMsg"></param>
/// <returns></returns>
BOOL CWin10LoginPicCollectorDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->hwnd == this->m_hWnd && pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;                // Do not process further
		}
	}
	return CWnd::PreTranslateMessage(pMsg);
}

/// <summary>
/// OnPaint
/// </summary>
void CWin10LoginPicCollectorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

/// <summary>
/// OnTimer
/// </summary>
/// <param name="nIDEvent"></param>
void CWin10LoginPicCollectorDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 100)
	{
		CTime time;
		time = CTime::GetCurrentTime();
		m_StatusBar.SetPaneText(1, time.Format("%H:%M:%S"));
	}
	CDialogEx::OnTimer(nIDEvent);
}

/// <summary>
/// OnToolTipText
/// </summary>
/// <param name=""></param>
/// <param name="pNMHDR"></param>
/// <param name="pResult"></param>
/// <returns></returns>
BOOL CWin10LoginPicCollectorDlg::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

	// if there is a top level routing frame then let it handle the message
	if (GetRoutingFrame() != NULL) return FALSE;

	// to be thorough we will need to handle UNICODE versions of the message also !!
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	TCHAR szFullText[2048];
	CString strTipText;
	UINT nID = pNMHDR->idFrom;

	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom is actually the HWND of the tool 
		nID = ::GetDlgCtrlID((HWND)nID);
	}

	// nID is the ToolTip Buton Id like 
	// ID_BUTTON_EXIT (32772) in ouer case
	if (nID != 0) // will be zero on a separator
	{
		AfxLoadString(nID, szFullText);
		strTipText = szFullText;

		// see: https://docs.microsoft.com/en-us/windows/win32/api/commctrl/ns-commctrl-nmttdispinfow
		ASSERT(strTipText.GetLength() < 80);

		if (pNMHDR->code == TTN_NEEDTEXTA)
		{
			_wcstombsz(pTTTA->szText, strTipText, sizeof(pTTTA->szText));
		}
		else
		{
			lstrcpyn(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
		}

		*pResult = 0;

		// bring the tooltip window above other popup windows
		::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
			SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE | SWP_NOOWNERZORDER);

		return TRUE;
	}

	return FALSE;
}

/// <summary>
/// OnNMClickListctrlDestination
/// </summary>
/// <param name="pNMHDR"></param>
/// <param name="pResult"></param>
void CWin10LoginPicCollectorDlg::OnNMClickListctrlDestination(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// Zero based index of CListCtrl
	int itemClicked = pNMItemActivate->iItem;
	// Buffer for the LVITEM.pszText
	wchar_t retText[MAX_PATH];
	ZeroMemory(retText, sizeof(wchar_t));

	LVITEM itemToGet;
	ZeroMemory(&itemToGet, sizeof(LVITEM));

	// Set the flag to LVIF_TEXT
	itemToGet.mask = LVIF_TEXT;
	// Specifies the required item
	itemToGet.iItem = itemClicked;
	// Assigns the buffer to pszText member variable
	itemToGet.pszText = retText;
	// IMPORTED set also cchTextMax
	itemToGet.cchTextMax = MAX_PATH;
	// Call GetItem(...) call
	BOOL rc = m_ctrlDestination.GetItem(&itemToGet);

	// Cuild file path name
	wstring fileName(itemToGet.pszText);
	wstring path = m_appSettings.m_strDestinationPath;
	wstring filePath = path + L"\\" + fileName;

	// Output of our file path name
	TRACE(_T("Click on ListCtrl Item %s\n"), filePath.c_str());
	// Status output
	CString strStatus;
	strStatus.Format(L"Click on ListCtrl Item %s\n", filePath.c_str());
	m_StatusBar.SetPaneText(0, strStatus);

	// Action on file
	filePath;

	*pResult = 0;
}

/// <summary>
/// OnNMDblclkListctrlDestination
/// </summary>
/// <param name="pNMHDR"></param>
/// <param name="pResult"></param>
void CWin10LoginPicCollectorDlg::OnNMDblclkListctrlDestination(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// Zero based index of CListCtrl
	int itemClicked = pNMItemActivate->iItem;

	// Status output
	CString strStatus;
	strStatus.Format(L"DBClick on ListCtrl Item %i\n", itemClicked);
	m_StatusBar.SetPaneText(0, strStatus);

	*pResult = 0;
}

/// <summary>
/// OnClose
/// </summary>
void CWin10LoginPicCollectorDlg::OnClose()
{
	// Shut down GDI+
	GdiplusShutdown(m_gdiplusToken);
	SaveAppSettings();

	CDialogEx::OnClose();
}

#pragma endregion

//------------------------------/
//-       Other Functions      -/
//------------------------------/
#pragma region My Other Functions

/// <summary>
/// SaveAppSettings
/// </summary>
void CWin10LoginPicCollectorDlg::SaveAppSettings()
{
	RECT rect;
	::GetWindowRect(GetSafeHwnd(), &rect);
	m_appSettings.m_lLeft = rect.left;
	m_appSettings.m_lTop = rect.top;
	m_appSettings.m_lRight = rect.right;
	m_appSettings.m_lBottom = rect.bottom;
	m_appSettings.SaveConfig();
	spdlog::info("END Win10LoginPicCollector");
}

/// <summary>
/// LoadIniFileInNotpad
/// </summary>
void CWin10LoginPicCollectorDlg::LoadIniFileInNotpad()
{
	TCHAR       szHelp[MAX_PATH];
	::GetModuleFileName(::GetModuleHandle(NULL), szHelp, MAX_PATH);
	CString strHelp = szHelp;
	CString strIniPathFile = strHelp.Left(strHelp.GetLength() - 3) + _T("ini");
	CString strComand;
	strComand.Format(L"notepad.exe");
	ShellExecute(m_hWnd, L"open", strComand, strIniPathFile, NULL, SW_SHOWNORMAL);

	// Status output
	CString strStatus;
	strStatus.Format(L"Show and edit the Ini File %s", strHelp);
	m_StatusBar.SetPaneText(0, strStatus);
}

/// <summary>
/// InitSourceBranch
/// </summary>
void CWin10LoginPicCollectorDlg::InitSourceBranch()
{
	CString strPath(m_appSettings.m_strSourcePath.c_str());
	// Append a "\" at the end if we have no
	if (strPath[strPath.GetLength()-1] == L'\\') strPath += L"*.*"; else strPath += L"\\*.*";

	// Replace the %USERNAME% with the real username if containing
	if (strPath.Find(L"%USERNAME%") != -1)
		strPath.Replace(L"%USERNAME%", CToolsDllApp::GetMyUserName().c_str());

	m_iNumFiles = CToolsDllApp::NumberOfFilesIn(strPath) - 2;
	CString strHelp(m_appSettings.m_strSourcePath.c_str());
	m_strSourcePath = strHelp;

	vector<wstring> fileNames = CToolsDllApp::GetAllFilesInDir(strPath);
	LoadListBox(fileNames);
	m_strCoutFilesSource.Format(L"%i",m_iNumFiles);
	UpdateData(false);

	// Status output
	CString strStatus;
	strStatus.Format(L"%i Files in %s", m_iNumFiles, strPath);
	m_StatusBar.SetPaneText(0, strStatus);
}

/// <summary>
/// InitDestinationBranch
/// </summary>
void CWin10LoginPicCollectorDlg::InitDestinationBranch()
{
	CString strPath(m_appSettings.m_strDestinationPath.c_str());
	// Append a "\" at the end if we have no
	if (strPath[strPath.GetLength() - 1] == L'\\') strPath += L"*.*"; else strPath += L"\\*.*";

	// Check if Destination path exsits
	if (!CToolsDllApp::FileExsist(m_appSettings.m_strDestinationPath.c_str()))
	{
		CString message;
		message.Format(L"The Destination Path:\n%s\ndose not exsist.\nDo you want to create it?",m_appSettings.m_strDestinationPath.c_str());
		// if not create it
		if (MessageBox(message, L"Windows 10 Pic Collectorg", MB_YESNO | MB_ICONQUESTION) == IDYES)
			CToolsDllApp::CreateDir(m_appSettings.m_strDestinationPath.c_str());
		else
			return;
	}

	vector<wstring> fileNames = CToolsDllApp::GetAllFilesInDir(strPath);
	LoadListCtrl(fileNames);
	int count = CToolsDllApp::NumberOfFilesIn(strPath) - 2;
	m_strCoutFilesDestination.Format(L"%i",count);
	UpdateData(false);

	// Status output
	CString strStatus;
	strStatus.Format(L"%i Files in %s", count, strPath);
	m_StatusBar.SetPaneText(0, strStatus);
}

/// <summary>
/// LoadListBox
/// </summary>
/// <param name="list"></param>
void CWin10LoginPicCollectorDlg::LoadListBox(vector<wstring> list)
{
	m_listBox.ResetContent();
	
	wstring stdstr = L"";
	for (size_t i = 0; i < list.size(); i++) 
	{
		stdstr = list.at(i);
		CString cstr(stdstr.c_str());
		m_listBox.AddString(cstr);
	}
}

/// <summary>
/// LoadListCtrl
/// This function loads the images with their properties 
/// and adds reduced images in the Image-List assigned to 
/// the ListCtr.
/// </summary>
/// <param name="list"></param>
void CWin10LoginPicCollectorDlg::LoadListCtrl(vector<wstring> list)
{
	wstring stdstr;
	COLORREF rgbTransparentColor = 0xFFFFFF;

	// Delete old items if any
	m_ctrlDestination.DeleteAllItems();
	// Delete the ImageList if we have one and initialise it new
	if (m_pImageListThumb) delete(m_pImageListThumb);
	m_pImageListThumb = new CImageList();
	// Create the ImageList with 100*90 icons and 32 bpp color depth
	m_pImageListThumb->Create(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, ILC_COLOR32, 0, 1);
	// Link the ImageList to the Control
	m_ctrlDestination.SetImageList(m_pImageListThumb, LVSIL_NORMAL);
	m_pImageListThumb->SetImageCount(list.size());
	m_ctrlDestination.SetRedraw(FALSE);

	// Iterate through all files found, load the 
	// image and paste it into the LstCtrl
	for (size_t i = 0; i < list.size(); i++)
	{
		HBITMAP hbmReturn = NULL;
		Bitmap* pbmPhoto = NULL;
		CBitmap bmp1;

		// Insert the item to the CListCtrl
		m_ctrlDestination.InsertItem(i, list.at(i).c_str(), i);
        stdstr = m_appSettings.m_strDestinationPath + L"\\" + list.at(i);
		Bitmap image(stdstr.c_str());

		int sourceWidth = image.GetWidth();
		int sourceHeight = image.GetHeight();

		int destX = 0;
		int destY = 0;

		float nPercent = 0;
		float nPercentW = ((float)THUMBNAIL_WIDTH / (float)sourceWidth);;
		float nPercentH = ((float)THUMBNAIL_HEIGHT / (float)sourceHeight);

		if (nPercentH < nPercentW)
		{
			nPercent = nPercentH;
			destX = (int)((THUMBNAIL_WIDTH - (sourceWidth * nPercent)) / 2);
		}
		else
		{
			nPercent = nPercentW;
			destY = (int)((THUMBNAIL_HEIGHT - (sourceHeight * nPercent)) / 2);
		}

		int destWidth = (int)(sourceWidth * nPercent);
		int destHeight = (int)(sourceHeight * nPercent);

		// Force the compiler to take global new (also necessary for the delete)
		pbmPhoto = ::new Bitmap(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, PixelFormat24bppRGB);
		pbmPhoto->SetResolution(image.GetHorizontalResolution(), image.GetVerticalResolution());

		Graphics* grPhoto = Graphics::FromImage(pbmPhoto);
		Color colorW(255, 255, 255, 255);
		grPhoto->Clear(colorW);
		grPhoto->SetInterpolationMode(InterpolationModeHighQualityBicubic);
		grPhoto->DrawImage(&image, Rect(destX, destY, destWidth, destHeight));

		pbmPhoto->GetHBITMAP(colorW, &hbmReturn);

		bmp1.Attach(hbmReturn);
		m_pImageListThumb->Replace(i, &bmp1, NULL);

		delete grPhoto;
		::delete pbmPhoto;  // see ::new above
		bmp1.Detach();
		DeleteObject(hbmReturn);
	}

	m_ctrlDestination.SetRedraw(TRUE);
	m_ctrlDestination.Invalidate();
}

/// <summary>
/// GetAndPrintLastErrorTxt
/// Print the Last Error (if any) 
/// as text in the Output
/// </summary>
/// <param name="strFuncName"></param>
void CWin10LoginPicCollectorDlg::GetAndPrintLastErrorTxt(wstring strFuncName)
{
	DWORD dwerr = GetLastError(); if (!dwerr) return;
	// Get the last Error in text
	wchar_t buf[256];
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, dwerr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
	TRACE(_T("Failed on %s LastError %u %s\n"), strFuncName.c_str(), dwerr, buf);
}

#pragma endregion