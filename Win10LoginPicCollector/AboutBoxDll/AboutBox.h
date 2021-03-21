#pragma once


// AboutBox dialog

class AFX_EXT_CLASS CAboutBox : public CDialogEx
{
	DECLARE_DYNAMIC(CAboutBox)

public:
	CAboutBox(CWnd* pParent = nullptr);   // Standard constructor
	virtual ~CAboutBox();
	virtual BOOL OnInitDialog();
	void InitParam();

// Dialog box data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CString m_strVersion;
};
