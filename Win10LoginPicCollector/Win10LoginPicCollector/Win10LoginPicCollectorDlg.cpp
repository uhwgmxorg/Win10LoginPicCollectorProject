
// Win10LoginPicCollectorDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "afxdialogex.h"

#include "..\AboutBoxDll\AboutBox.h"

#include "Win10LoginPicCollector.h"
#include "Win10LoginPicCollectorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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
}

BEGIN_MESSAGE_MAP(CWin10LoginPicCollectorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_BUTTON_EXIT, &CWin10LoginPicCollectorDlg::OnButtonExit)
	ON_COMMAND(ID_BUTTON_SELECT_PATH, &CWin10LoginPicCollectorDlg::OnButtonSelectSourcePath)
	ON_COMMAND(ID_BUTTON_EDIT_INI, &CWin10LoginPicCollectorDlg::OnButtonEditIni)
	ON_COMMAND(ID_BUTTON_ABOUT, &CWin10LoginPicCollectorDlg::OnButtonAbout)
	ON_WM_CLOSE()
	ON_COMMAND(ID_BUTTON_SELECT_DESTINATION_PATH, &CWin10LoginPicCollectorDlg::OnButtonSelectDestinationPath)
	ON_COMMAND(ID_BUTTON_RESET_CONFIG, &CWin10LoginPicCollectorDlg::OnButtonResetConfig)
	ON_BN_CLICKED(ID_BUTTON_COPY, &CWin10LoginPicCollectorDlg::OnClickedButtonCopy)
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
	m_strVersion = L"1.0.0.0";
	SetWindowText(_T("Window 10 Login Picture Collector Version " + m_strVersion));

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
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
#pragma endregion

	// Load the App settings
	m_appSettings.LoadConfig();

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

	std::wstring strPath = CAppSettings::GetPath();
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

	std::wstring strPath = CAppSettings::GetPath();
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
/// OnClickedButtonCopy
/// </summary>
void CWin10LoginPicCollectorDlg::OnClickedButtonCopy()
{
	TRACE(_T("OnClickedButtonCopy was pressed\n"));

	m_appSettings.CopyFile((m_appSettings.m_strSourcePath).c_str(), m_appSettings.m_strDestinationPath.c_str());
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
/// OnClose
/// </summary>
void CWin10LoginPicCollectorDlg::OnClose()
{
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
}

/// <summary>
/// InitSourceBranch
/// </summary>
void CWin10LoginPicCollectorDlg::InitSourceBranch()
{
	CString strPath(m_appSettings.m_strSourcePath.c_str());
	strPath += L"\\*.*";

	// Replace the %USERNAME% with the real username if containing
	if (strPath.Find(L"%USERNAME%") != -1)
		strPath.Replace(L"%USERNAME%", CAppSettings::GetMyUserName().c_str());

	m_iNumFiles = CAppSettings::NumberOfFilesIn(strPath);
	CString strHelp(m_appSettings.m_strSourcePath.c_str());
	m_strSourcePath = strHelp;

	std::vector<std::wstring> fileNames = CAppSettings::GetAllFilesInDir(strPath);
	LoadListBox(fileNames);
	UpdateData(false);
}

/// <summary>
/// InitDestinationBranch
/// </summary>
void CWin10LoginPicCollectorDlg::InitDestinationBranch()
{
	CString strPath(m_appSettings.m_strDestinationPath.c_str());
	strPath += L"\\*.*";
	CString strHelp(m_appSettings.m_strDestinationPath.c_str());
	m_strDestinationPath = strHelp;

	std::vector<std::wstring> fileNames = CAppSettings::GetAllFilesInDir(strPath);
	LoadListCtrl(fileNames);
	UpdateData(false);
}

/// <summary>
/// LoadListBox
/// </summary>
/// <param name="list"></param>
void CWin10LoginPicCollectorDlg::LoadListBox(std::vector<std::wstring> list)
{
	m_listBox.ResetContent();
	
	std::wstring stdstr = L"";
	for (size_t i = 0; i < list.size(); i++) 
	{
		stdstr = list.at(i);
		CString cstr(stdstr.c_str());
		m_listBox.AddString(cstr);
	}
}

/// <summary>
/// LoadListCtrl
/// </summary>
/// <param name="list"></param>
void CWin10LoginPicCollectorDlg::LoadListCtrl(std::vector<std::wstring> list)
{
	m_ctrlDestination.DeleteAllItems();

	std::wstring stdstr = L"";
	for (size_t i = 0; i < list.size(); i++)
	{
		stdstr = list.at(i);
		CString cstr(stdstr.c_str());
		InsertItem(cstr);
	}
}
void CWin10LoginPicCollectorDlg::InsertItem(CString strItem)
{
	LVITEM lvi;

	lvi.mask = LVIF_TEXT;
	lvi.iItem = 0;
	lvi.iSubItem = 0;
	lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	lvi.iImage = NULL;
	m_ctrlDestination.InsertItem(&lvi);
}

#pragma endregion
