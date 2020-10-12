// NewProjectDlg.cpp : implementation file
//

// #include "pch.h"
#include "stdafx.h"
#include "DemoGrab.h"
#include "NewProjectDlg.h"
#include "afxdialogex.h"


// NewProjectDlg dialog

IMPLEMENT_DYNAMIC(NewProjectDlg, CDialog)

NewProjectDlg::NewProjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_NEW_DIALOG, pParent)
	, m_ProjectName(_T(""))
	, m_NewProjectPath(_T(""))
	, m_RecordDest(0)
{

}

NewProjectDlg::~NewProjectDlg()
{
}

void NewProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NEW_PROJ_NAME, m_ProjectName);
	DDX_Text(pDX, IDC_NEW_PROJ_LOCATION, m_NewProjectPath);
	DDX_Radio(pDX, IDC_NEW_PROJ_RAM, m_RecordDest);
}


BEGIN_MESSAGE_MAP(NewProjectDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &NewProjectDlg::OnBrowseButtonClicked)
END_MESSAGE_MAP()


// NewProjectDlg message handlers


void NewProjectDlg::OnBrowseButtonClicked()
{
	CFolderPickerDialog m_dlg;
	m_dlg.m_ofn.lpstrTitle = _T("Choose the directory where the project will be created...");
	m_dlg.m_ofn.lpstrInitialDir = _T("C:\\");
	if (m_dlg.DoModal() == IDOK) {
		m_NewProjectPath = m_dlg.GetPathName();   // Use this to get the selected folder name 
										  // after the dialog has closed

		// May need to add a '\' for usage in GUI and for later file saving, 
		// as there is no '\' on the returned name
		m_NewProjectPath += _T("\\");
		UpdateData(FALSE);   // To show updated folder in GUI

		// Debug
		TRACE("\n%S", m_NewProjectPath);
	}
}
