#if !defined(AFX_MAINFRM_H__DB0EBA4E_AB05_42A5_AF89_364259825A00__INCLUDED_)
#define AFX_MAINFRM_H__DB0EBA4E_AB05_42A5_AF89_364259825A00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChildView.h"
#include "ChildViewPlay.h"

class CMainFrame : public CFrameWnd
{
public:
	CMainFrame();

protected: 
	DECLARE_DYNAMIC(CMainFrame)

public:

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

	virtual ~CMainFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CChildView  ChildView;
	CChildViewPlay ChildViewPlay;

	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	
	// Grab
	afx_msg void OnGrabberLive();
	afx_msg void OnUpdateGrabberLive(CCmdUI* pCmdUI);
	afx_msg void OnSequencerNewsequence();
	afx_msg void OnSequencerClosesequence();
	afx_msg void OnUpdateSequencerNewsequence(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSequencerClosesequence(CCmdUI* pCmdUI);
	afx_msg void OnGrabberPropertypages();
	afx_msg void OnGrabberViewImageInformation();
	afx_msg void OnGrabberSettings();
	afx_msg void OnGrabberAdjustments();
	afx_msg void OnSequencerCaptureaudio();
	afx_msg void OnUpdateSequencerCaptureaudio(CCmdUI *pCmdUI);
	afx_msg void OnClose();

	// Play
	afx_msg void OnSequencerGoToPreviousFrame();
	afx_msg void OnSequencerGoToNextFrame();
	afx_msg void OnSequencerPlay();
	afx_msg void OnUpdateSequencerPlay(CCmdUI* pCmdUI);
	afx_msg void OnSequencerStop();
	afx_msg void OnSequencerLoop();
	afx_msg void OnUpdateSequencerLoop(CCmdUI* pCmdUI);
	afx_msg void OnSequencerPlay15();
	afx_msg void OnUpdateSequencerPlay15(CCmdUI* pCmdUI);
	afx_msg void OnSequencerPlay30();
	afx_msg void OnUpdateSequencerPlay30(CCmdUI* pCmdUI);
	afx_msg void OnSequencerPlayb15();
	afx_msg void OnUpdateSequencerPlayb15(CCmdUI* pCmdUI);
	afx_msg void OnSequencerPlayb30();
	afx_msg void OnUpdateSequencerPlayb30(CCmdUI* pCmdUI);
	afx_msg void OnSequencerPlaypause();
	afx_msg void OnUpdateSequencerPlaypause(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSequencerGoToNextFrame(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSequencerGoToPreviousFrame(CCmdUI* pCmdUI);
	afx_msg void OnSequencerDescription();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileCreatenewproject();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__DB0EBA4E_AB05_42A5_AF89_364259825A00__INCLUDED_)
