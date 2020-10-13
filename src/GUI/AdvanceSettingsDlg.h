#pragma once
#include "afxwin.h"

#include <Hermes.h> //the hermes api header
// CGigEVisonTest dialog

class CAdvanceSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CAdvanceSettingsDlg)

public:
	CAdvanceSettingsDlg(HGrabModule* mdoule,CWnd* pParent = NULL);   // standard constructor
	virtual ~CAdvanceSettingsDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };
private:
    HGrabModule* GrabMdoule;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CStatic Frame;
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
};
