#pragma once


// NewProjectDlg dialog

class NewProjectDlg : public CDialog
{
	DECLARE_DYNAMIC(NewProjectDlg)

public:
	NewProjectDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~NewProjectDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEW_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_ProjectName;
	CString m_NewProjectPath;
	int m_RecordDest;
	afx_msg void OnBrowseButtonClicked();
};
