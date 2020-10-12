#if !defined(AFX_MAINFRM_H__DB0EBA4E_AB05_42A5_AF89_364259825A00__INCLUDED_)
#define AFX_MAINFRM_H__DB0EBA4E_AB05_42A5_AF89_364259825A00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChildView.h"

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

	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileCreatenewproject();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__DB0EBA4E_AB05_42A5_AF89_364259825A00__INCLUDED_)
