#if !defined(AFX_CHILDVIEW_H__0557C3F2_EABB_4D76_960C_48EB167657FB__INCLUDED_)
#define AFX_CHILDVIEW_H__0557C3F2_EABB_4D76_960C_48EB167657FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Hermes.h" //the hermes api header
#include "GrabberListDlg.h"

class CChildView : public CWnd, public HGrabCallbacks, public CAudioRecorderCallback
{
private:
    HGrabModule GrabModule; //The grabber module
    HImage* LastImage;      //The lastest image received by the grabber
    HViewer ImageViewer;    //Used to display HImage 
    bool Live;

    HSequence Sequence;
    bool SequenceReady;

	HGrabAudioModule* GrabAudioModule;
	HAudioRecorder AudioRecorder;
    HAudioFile AudioFile;
	bool CaptureAudio;      //True if the audio will be recorded. Otherwise, false

public:
	CChildView();
	virtual ~CChildView();
    //Project related
    void NewProject();

    //Grabber related
    bool LoadGrabber();
    void StartLive();
    void StopLive();
    bool IsLive();
    void ShowPropertyPages();
    void ShowImageInfo();

    void GrabberSettings();
    void GrabberAdjustments();

    //Sequencer related
    void NewSequence();
    void CloseSequence();
    bool IsSequenceReady();

	//Audio related
	bool IsAudioSupported();       //Return true if the driver supports audio capability. Otherwise, return false
	bool IsAudioCaptureEnabled();  //Return true if the audio is recorded. Otherwise, return false
	void ToggleAudioCapture();

    //HGrabCallbacks overriden function
    void OnImageReceived(HImage* image, unsigned int userData, void* userPtr);


	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CChildView)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnGrabberAdvanceSettings();
    afx_msg void OnGrabberAdvanceSettingsModaless();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__0557C3F2_EABB_4D76_960C_48EB167657FB__INCLUDED_)
