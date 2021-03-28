
// Win10LoginPicCollectorDlg.h : header file
//

#pragma once

#include "..\ToolsDll\AppSettings.h"
#include "..\ToolsDll\ToolsDll.h"

// CWin10LoginPicCollectorDlg dialog
class CWin10LoginPicCollectorDlg : public CDialogEx
{
// Construction
public:
	CWin10LoginPicCollectorDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WIN10LOGINPICCOLLECTOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CToolBar m_ctrlToolBar1;
	CStatusBar m_StatusBar;
	CAppSettings m_appSettings;
	CString m_strVersion;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

	void LoadIniFileInNotpad();
public:
	afx_msg void OnButtonExit();
	afx_msg void OnButtonSelectSourcePath();
	afx_msg void OnButtonEditIni();
	afx_msg void OnButtonAbout();
private:
	int m_iNumFiles;
	CListBox m_listBox;
	CListCtrl m_ctrlDestination;
	CImageList *m_pImageListThumb;

	void InitSourceBranch();
	void InitDestinationBranch();
	void SaveAppSettings();
	void LoadListBox(std::vector<std::wstring> list);
	void LoadListCtrl(std::vector<std::wstring> list);
	void GetAndPrintLastErrorTxt(std::wstring strFuncName);
	void InsertItem(CString strItem, int iImage);

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_strSourcePath;
	CString m_strDestinationPath;
	afx_msg void OnButtonSelectDestinationPath();
	afx_msg void OnButtonResetConfig();
	afx_msg void OnClickedButtonCopy();
};
