#if !defined(AFX_DESCRIPTIONDLG_H__4DD98821_820F_4C6F_A82F_349F0AC9A35B__INCLUDED_)
#define AFX_DESCRIPTIONDLG_H__4DD98821_820F_4C6F_A82F_349F0AC9A35B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDescriptionDlg : public CDialog
{
public:
	CDescriptionDlg(CWnd* pParent = NULL);   // standard constructor

//MFC STUFF
	//{{AFX_DATA(CDescriptionDlg)
	enum { IDD = IDD_DESCRIPTION };
	CString	Description;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDescriptionDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDescriptionDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESCRIPTIONDLG_H__4DD98821_820F_4C6F_A82F_349F0AC9A35B__INCLUDED_)
