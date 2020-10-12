#include "stdafx.h"
#include "DemoGrab.h"
#include "GrabberListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CGrabberListDlg::CGrabberListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGrabberListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGrabberListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

    GrabModule = NULL;
}
//-----------------------------------------------------------------------------
void CGrabberListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGrabberListDlg)
	DDX_Control(pDX, IDC_GRABBER_LIST, GrabberList);
	//}}AFX_DATA_MAP
}
//-----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CGrabberListDlg, CDialog)
	//{{AFX_MSG_MAP(CGrabberListDlg)
	ON_BN_CLICKED(IDC_GRABBER_OK, OnGrabberOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-----------------------------------------------------------------------------
void CGrabberListDlg::SetGrabModule(HGrabModule* module)
{
    GrabModule = module;
}
//-----------------------------------------------------------------------------
BOOL CGrabberListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    //Query the grab module to know how much grabbers are availables
    int driverCount = GrabModule->GetDriverCount();
    
    //Fill the ListBox with all the available cards
    eHSupportedModule driverID;

    for(int index = 0; index < driverCount; index++)
        GrabberList.AddString(GrabModule->GetDriverInfo(index, &driverID));

	//Set a default selection
    GrabberList.SetCurSel(0);

    return TRUE;
}
//-----------------------------------------------------------------------------
void CGrabberListDlg::OnGrabberOk() 
{
    //Retrieve the identifier of the selected grabber
    eHSupportedModule driverID;

    //Note: Make sure the Listbox "sort" option is unchecked else
    //the indexes won't be in the same order.
    
    if(GrabModule->GetDriverInfo(GrabberList.GetCurSel(), &driverID) == NULL)
    {
        OnCancel();
        return;
    }

    //Try to load the selected grabber
    if(!GrabModule->Load(driverID, _T("DemoGrabber")))
    {
        //retrieve the error code
        eHErrorCode error = GrabModule->GetLastError();

        //Display the error message
        AfxMessageBox(HErrorManager::GetErrorString(error));

        //Grabber could not be loaded
        OnCancel();

        return;
    }

    OnOK();
}
