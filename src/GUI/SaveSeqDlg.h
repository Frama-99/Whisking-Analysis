#if !defined(AFX_SAVESEQDLG_H__AF1DE762_FDBD_4629_B17D_4021D5DBD33B__INCLUDED_)
#define AFX_SAVESEQDLG_H__AF1DE762_FDBD_4629_B17D_4021D5DBD33B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SaveSeqDlg.h : header file
//

#include <Hermes.h> //the hermes api header

/////////////////////////////////////////////////////////////////////////////
// CSaveSeqDlg dialog

class CSaveSeqDlg : public CFileDialog
{
	DECLARE_DYNAMIC(CSaveSeqDlg)

public:
	CSaveSeqDlg(eHCompression& compression, LPCTSTR filename = NULL, CWnd* parentWnd = NULL);

private:
		 eHCompression* Compression;
protected:
	// Dialog Data
	//{{AFX_DATA(CSaveSeqDlg)
	CComboBox	SeqTypeList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSaveSeqDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSaveSeqDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeSeqTypelist();
	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVESEQDLG_H__AF1DE762_FDBD_4629_B17D_4021D5DBD33B__INCLUDED_)
