#include "stdafx.h"
#include "DemoGrab.h"
#include "DescriptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDescriptionDlg::CDescriptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDescriptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDescriptionDlg)
	Description = _T("");
	//}}AFX_DATA_INIT
}
//-----------------------------------------------------------------------------
void CDescriptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDescriptionDlg)
	DDX_Text(pDX, IDC_DESCRIPTION_EDIT, Description);
	DDV_MaxChars(pDX, Description, 500);
	//}}AFX_DATA_MAP
}
//-----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CDescriptionDlg, CDialog)
	//{{AFX_MSG_MAP(CDescriptionDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-----------------------------------------------------------------------------
void CDescriptionDlg::OnOK()
{
	UpdateData(TRUE);

	CDialog::OnOK();
}
