#if !defined(AFX_ChildViewPlay_H__4F8CF2B7_BCA4_4D61_92E7_EE65214D3F4D__INCLUDED_)
#define AFX_ChildViewPlay_H__4F8CF2B7_BCA4_4D61_92E7_EE65214D3F4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Hermes.h> //the hermes api header
#include "DescriptionDlg.h"

enum ePlayState
{
    PLAY,
    PLAYB30,
    PLAYB15,
    PLAY0,
    PLAYF15,
    PLAYF30,
    STOP
};

class CChildViewPlay :
    public CWnd,
    public HSequencerCallbacks,
    public HAviFileCallbacks
{
private:
    HSequence Sequence;
    HAviFile AviFile;

    bool SequenceMode;

    HViewer ImageViewer;
    HImage* LastImage;
    HImage* StillImage;

    ePlayState CurrentState;
    bool Loop;

    void UpdatePosition();

public:
    CChildViewPlay();

    bool LoadSequence();
    //void TogglePlay();

    void ToggleLoop();
    bool IsLooping();

    void SetPlayState(ePlayState state);
    ePlayState GetPlayState();

    void GoToPreviousFrame();
    void GoToNextFrame();

    void EditSequenceDescription();

    //Sequence Callbacks
    virtual void OnImageRead(HImage* image, unsigned int userData, void* userPtr);
    virtual void OnEndOfSequence(bool& loop, unsigned int userData, void* userPtr);
    virtual void OnPlaybackStopped(unsigned int userData, void* userPtr);


    //AVI Callbacks
    virtual void OnAviImageRead(HImage* image, unsigned int index, unsigned int userData, void* userPtr);
    virtual void OnAviEndPlayback(unsigned int userData, void* userPtr);


    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CChildViewPlayPlay)
protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    //}}AFX_VIRTUAL

public:
    virtual ~CChildViewPlay();

protected:
    //{{AFX_MSG(CChildViewPlayPlay)
    afx_msg void OnPaint();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ChildViewPlay_H__4F8CF2B7_BCA4_4D61_92E7_EE65214D3F4D__INCLUDED_)
