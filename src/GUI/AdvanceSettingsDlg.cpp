#include "stdafx.h"
#include "DemoGrab.h"
#include "AdvanceSettingsDlg.h"


// CGigEVisonTest dialog

IMPLEMENT_DYNAMIC(CAdvanceSettingsDlg, CDialog)

CAdvanceSettingsDlg::CAdvanceSettingsDlg(HGrabModule* mdoule,CWnd* pParent /*=NULL*/)
	: CDialog(CAdvanceSettingsDlg::IDD, pParent)
{
    GrabMdoule = mdoule;
}

CAdvanceSettingsDlg::~CAdvanceSettingsDlg()
{

}

void CAdvanceSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_FRAME, Frame);
}


BEGIN_MESSAGE_MAP(CAdvanceSettingsDlg, CDialog)
    ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CAdvanceSettingsDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    CRect rc;

	Frame.GetWindowRect(rc);
	ScreenToClient(rc);
	Frame.ShowWindow(SW_HIDE);
	
    GrabMdoule->CreateAdvanceSettingsWindow(GetSafeHwnd());  
    GrabMdoule->ReSizeAdvanceSettingsWindow(rc.left,rc.top,rc.Width(),rc.Height());
    GrabMdoule->ShowAdvanceSettingsWindow(true); 
    
    return TRUE; 
}

void CAdvanceSettingsDlg::OnDestroy()
{
    GrabMdoule->CloseAdvanceSettingsWindow(); 
    CDialog::OnDestroy();
}
