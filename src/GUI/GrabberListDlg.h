#if !defined(AFX_GRABBERLISTDLG_H__183AB83B_B688_46B5_86AF_ED1C16EBF8E8__INCLUDED_)
#define AFX_GRABBERLISTDLG_H__183AB83B_B688_46B5_86AF_ED1C16EBF8E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Hermes.h> //the hermes api header

class CGrabberListDlg : public CDialog
{
private:
    HGrabModule* GrabModule;

public:
	CGrabberListDlg(CWnd* pParent = NULL);   // standard constructor
    void SetGrabModule(HGrabModule* module);

	//{{AFX_DATA(CGrabberListDlg)
	enum { IDD = IDD_GRABBER_DLG };
	CListBox	GrabberList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrabberListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	// Generated message map functions
	//{{AFX_MSG(CGrabberListDlg)
	afx_msg void OnGrabberOk();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRABBERLISTDLG_H__183AB83B_B688_46B5_86AF_ED1C16EBF8E8__INCLUDED_)
