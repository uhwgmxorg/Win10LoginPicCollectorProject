// AboutBox.cpp: Implementation file
//

#include "pch.h"
#include "AboutBox.h"
#include "afxdialogex.h"
#include "resource.h"


// AboutBox-Dialog

IMPLEMENT_DYNAMIC(CAboutBox, CDialogEx)

CAboutBox::CAboutBox(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ABOUTBOX, pParent)
{
	m_strVersion = "1.0.0.4";
}

CAboutBox::~CAboutBox()
{
}


BOOL CAboutBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  add additional initialization here.

	CWnd *label = GetDlgItem(IDC_VERSIONTEXT);
	label->SetWindowText(m_strVersion);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // AUSNAHME: OCX-Eigenschaftenseite muss FALSE zurückgeben.
}

void CAboutBox::InitParam()
{

}

void CAboutBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutBox, CDialogEx)
END_MESSAGE_MAP()


// AboutBox message handler