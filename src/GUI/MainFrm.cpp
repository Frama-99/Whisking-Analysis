#include "stdafx.h"
#include "DemoGrab.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)
//-----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_GRABBER_LIVE, OnGrabberLive)
	ON_UPDATE_COMMAND_UI(ID_GRABBER_LIVE, OnUpdateGrabberLive)
	ON_COMMAND(ID_SEQUENCER_NEWSEQUENCE, OnSequencerNewsequence)
	ON_COMMAND(ID_SEQUENCER_CLOSESEQUENCE, OnSequencerClosesequence)
	ON_UPDATE_COMMAND_UI(ID_SEQUENCER_NEWSEQUENCE, OnUpdateSequencerNewsequence)
	ON_UPDATE_COMMAND_UI(ID_SEQUENCER_CLOSESEQUENCE, OnUpdateSequencerClosesequence)
	ON_COMMAND(ID_GRABBER_PROPERTYPAGES, OnGrabberPropertypages)
	ON_COMMAND(ID_GRABBER_VIEWIMAGEINFORMATION, OnGrabberViewImageInformation)
	ON_COMMAND(ID_GRABBER_SETTINGS, OnGrabberSettings)
	ON_COMMAND(ID_GRABBER_ADJUSTMENTS, OnGrabberAdjustments)
	ON_COMMAND(ID_SEQUENCER_CAPTUREAUDIO, &CMainFrame::OnSequencerCaptureaudio)
	ON_UPDATE_COMMAND_UI(ID_SEQUENCER_CAPTUREAUDIO, &CMainFrame::OnUpdateSequencerCaptureaudio)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_CREATENEWPROJECT, &CMainFrame::OnFileCreatenewproject)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------------
static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
//-----------------------------------------------------------------------------
CMainFrame::CMainFrame()
{

}
//-----------------------------------------------------------------------------
CMainFrame::~CMainFrame()
{

}
//-----------------------------------------------------------------------------
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// create a view to occupy the client area of the frame
	if (!ChildView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

    //Ask the user which driver to load
    if(!ChildView.LoadGrabber())
        return -1; //failed to load a grabber

    ChildView.StartLive();

	return 0;
}
//-----------------------------------------------------------------------------
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}
//-----------------------------------------------------------------------------
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG
//-----------------------------------------------------------------------------
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	ChildView.SetFocus();
}
//-----------------------------------------------------------------------------
BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (ChildView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
//-----------------------------------------------------------------------------
void CMainFrame::OnGrabberLive() 
{
    if(ChildView.IsLive())
        ChildView.StopLive(); //stop live feed
    else
        ChildView.StartLive(); //start live feed
}
//-----------------------------------------------------------------------------
void CMainFrame::OnUpdateGrabberLive(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(ChildView.IsLive() ? 1 : 0);
}
//-----------------------------------------------------------------------------
void CMainFrame::OnSequencerNewsequence() 
{
    ChildView.NewSequence();
}
//-----------------------------------------------------------------------------
void CMainFrame::OnUpdateSequencerNewsequence(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(ChildView.IsSequenceReady() ? 0 : 1);
}
//-----------------------------------------------------------------------------
void CMainFrame::OnSequencerClosesequence() 
{
    ChildView.CloseSequence();
}
//-----------------------------------------------------------------------------
void CMainFrame::OnUpdateSequencerClosesequence(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(ChildView.IsSequenceReady() ? 1 : 0);
}
//-----------------------------------------------------------------------------
void CMainFrame::OnSequencerCaptureaudio()
{
	ChildView.ToggleAudioCapture();
}
//-----------------------------------------------------------------------------
void CMainFrame::OnUpdateSequencerCaptureaudio(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(ChildView.IsAudioSupported() && !ChildView.IsSequenceReady());
	pCmdUI->SetCheck(ChildView.IsAudioCaptureEnabled() ? 1 : 0);
}
//-----------------------------------------------------------------------------
void CMainFrame::OnGrabberPropertypages() 
{
    ChildView.ShowPropertyPages();
}
//-----------------------------------------------------------------------------
void CMainFrame::OnGrabberViewImageInformation() 
{
    ChildView.ShowImageInfo();
}
//-----------------------------------------------------------------------------
void CMainFrame::OnGrabberSettings() 
{
    ChildView.GrabberSettings();
}
//-----------------------------------------------------------------------------
void CMainFrame::OnGrabberAdjustments() 
{
    ChildView.GrabberAdjustments();
}
//-----------------------------------------------------------------------------
void CMainFrame::OnClose() 
{
	ChildView.CloseSequence();
    ChildView.StopLive();
	CFrameWnd::OnClose();
}


void CMainFrame::OnFileCreatenewproject()
{
	ChildView.NewProject();
}
