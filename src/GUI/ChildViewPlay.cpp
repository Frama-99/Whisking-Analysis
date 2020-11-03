#include "stdafx.h"
#include "DemoGrab.h"
#include "ChildViewPlay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CChildViewPlay::CChildViewPlay()
{
    CurrentState = STOP;
    LastImage = NULL;
    StillImage = HImage::CreateHImage();
    Loop = false;
    SequenceMode = true;
}
//-----------------------------------------------------------------------------
CChildViewPlay::~CChildViewPlay()
{
    HImage::DeleteHImage(StillImage);

    if (Sequence.IsPlaying())
        Sequence.Stop();

    Sequence.Close();
}
//-----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CChildViewPlay, CWnd)
    //{{AFX_MSG_MAP(CChildViewPlay)
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-----------------------------------------------------------------------------
BOOL CChildViewPlay::PreCreateWindow(CREATESTRUCT& cs)
{
    if (!CWnd::PreCreateWindow(cs))
        return FALSE;

    cs.dwExStyle |= WS_EX_CLIENTEDGE;
    cs.style &= ~WS_BORDER;
    cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
        ::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW + 1), NULL);

    return TRUE;
}
//-----------------------------------------------------------------------------
bool CChildViewPlay::LoadSequence()
{
    CFileDialog fd(TRUE, _T("seq"), NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
        _T("Norpix Sequence File (*.seq)|*.seq|Movie Clip (*.avi)|*.avi||"), 0, 0, 0);

    //Show the dialog box
    if (fd.DoModal() != IDOK)
        return false; //user clicked [Cancel]

    CString path = fd.GetPathName();

    if (path.Find(_T(".seq")) != -1)
    {
        if (!Sequence.Open(path))
            return false;

        Sequence.SetCallbacks(this, 0, this);
        Sequence.Read(StillImage, 1);
        LastImage = StillImage;
        Loop = false;
        SequenceMode = true;
        return true;
    }

    else if (path.Find(_T(".avi")) != -1)
    {
        if (!AviFile.OpenAviFile(path))
            return false;

        OutputDebugString(_T("Open AVI success"));
        AviFile.SetCallbacks(this, 0, NULL);
        LastImage = StillImage;
        SequenceMode = false;
        Loop = false;
        return true;
    }

    return false;
}
//-----------------------------------------------------------------------------
void CChildViewPlay::ToggleLoop()
{
    Loop = !Loop;
}
//-----------------------------------------------------------------------------
bool CChildViewPlay::IsLooping()
{
    return Loop;
}
//-----------------------------------------------------------------------------
void CChildViewPlay::OnImageRead(HImage* image, unsigned int userData, void* userPtr)
{
    //We received an image from the sequencer "Play" function
    LastImage = image;
    Invalidate(false); //will trigger an OnPaint (do not directly call OnPaint)
}
//-----------------------------------------------------------------------------
void CChildViewPlay::OnEndOfSequence(bool& loop, unsigned int userData, void* userPtr)
{
    loop = Loop;
}
//-----------------------------------------------------------------------------
void CChildViewPlay::OnPlaybackStopped(unsigned int userData, void* userPtr)
{
    CurrentState = STOP;
}
//-----------------------------------------------------------------------------
void CChildViewPlay::OnAviImageRead(HImage* image, unsigned int index,
    unsigned int userData, void* userPtr)
{
    //We received an image from the AVI File "Play" function
    LastImage = image;
    Invalidate(false);
}
//-----------------------------------------------------------------------------
void CChildViewPlay::OnAviEndPlayback(unsigned int userData, void* userPtr)
{
    CurrentState = STOP;
}
//-----------------------------------------------------------------------------
void CChildViewPlay::SetPlayState(ePlayState state)
{
    if (state == CurrentState)
        return;

    if (SequenceMode)
    {
        if (state == PLAY)
            Sequence.Play();

        else if (state == STOP)
            Sequence.Stop();

        else if (state == PLAYB30)
        {
            Sequence.SetPlaybackSpeed(-30.0);
            Sequence.PlayFixedRate();
        }

        else if (state == PLAYB15)
        {
            Sequence.SetPlaybackSpeed(-15.0);
            Sequence.PlayFixedRate();
        }

        else if (state == PLAY0)
        {
            Sequence.SetPlaybackSpeed(0.0);
            Sequence.PlayFixedRate();
        }

        else if (state == PLAYF15)
        {
            Sequence.SetPlaybackSpeed(15.0);
            Sequence.PlayFixedRate();
        }

        else if (state == PLAYF30)
        {
            Sequence.SetPlaybackSpeed(30.0);
            Sequence.PlayFixedRate();
        }
    }

    else
    {//AVI mode
        if (state == PLAY)
            AviFile.Play();

        else if (state == STOP)
            AviFile.Stop();
    }

    CurrentState = state;
}
//-----------------------------------------------------------------------------
ePlayState CChildViewPlay::GetPlayState()
{
    return CurrentState;
}
//-----------------------------------------------------------------------------
void CChildViewPlay::GoToPreviousFrame()
{
    if (SequenceMode)
    {
        if (Sequence.IsPlaying())
            return;

        //Show the previous frame
        Sequence.SetPlaybackPosition(Sequence.GetPlaybackPosition() - 1);
        Sequence.Read(StillImage, Sequence.GetPlaybackPosition());
    }

    else
    {
        if (AviFile.IsPlaying())
            return;

        AviFile.ReadImage(StillImage, AviFile.GetCurrentPosition() - 1);
    }

    LastImage = StillImage;
    Invalidate(false);
}
//-----------------------------------------------------------------------------
void CChildViewPlay::GoToNextFrame()
{
    if (SequenceMode)
    {
        if (Sequence.IsPlaying())
            return;

        //Show the next frame
        Sequence.SetPlaybackPosition(Sequence.GetPlaybackPosition() + 1);
        Sequence.Read(StillImage, Sequence.GetPlaybackPosition());
    }

    else
    {
        if (AviFile.IsPlaying())
            return;

        AviFile.ReadImage(StillImage, AviFile.GetCurrentPosition() + 1);
    }

    LastImage = StillImage;
    Invalidate(false);
}
//-----------------------------------------------------------------------------
void CChildViewPlay::EditSequenceDescription()
{
    if (!SequenceMode)
        return;

#ifdef UNICODE
    BYTE data[512];
    _tcscpy_s((wchar_t*)data, 255, _T(""));

    //We won't handle MBCS strings or binary data
    if (Sequence.GetDescriptionFormat() == H_UNICODE_FORMAT)
        Sequence.GetDescription(data); //Read the current description

    //Show the box
    CDescriptionDlg dlg;
    dlg.Description = (wchar_t*)data;

    if (dlg.DoModal() == IDOK)
    {//Update the description
        _tcscpy_s((wchar_t*)data, 255, dlg.Description);

        Sequence.SetDescriptionFormat(H_UNICODE_FORMAT);
        Sequence.SetDescription(data);
    }
#else
    BYTE data[512];
    strcpy((char*)data, "");

    //We won't handle UNICODE strings or binary data
    if (Sequence.GetDescriptionFormat() == H_MBCS_FORMAT)
        Sequence.GetDescription(data); //Read the current description

    //Show the box
    CDescriptionDlg dlg;
    dlg.Description = (char*)data;

    if (dlg.DoModal() == IDOK)
    {//Update the description
        strcpy((char*)data, dlg.Description);

        Sequence.SetDescriptionFormat(H_MBCS_FORMAT);
        Sequence.SetDescription(data);
    }
#endif
}
//-----------------------------------------------------------------------------
void CChildViewPlay::UpdatePosition()
{
    unsigned long current;
    unsigned long total;

    if (SequenceMode)
    {
        current = Sequence.GetPlaybackPosition();
        total = Sequence.GetAllocatedFrames();
    }

    else
    {
        current = AviFile.GetCurrentPosition();
        total = AviFile.GetFrameCount();
    }

#ifdef UNICODE
    TCHAR text[50];
    _stprintf_s(text, _T("PlayDemo (%u/%u)"), current, total);
#else
    char text[50];
    sprintf(text, "PlayDemo (%u/%u)", current, total);
#endif

    GetParent()->SetWindowText(text);
}
//-----------------------------------------------------------------------------
void CChildViewPlay::OnPaint()
{
    UpdatePosition();

    CPaintDC dc(this); // device context for painting

    if (LastImage != NULL)
        ImageViewer.ShowImage(dc.m_hDC, LastImage, 0, 0);
}
//-----------------------------------------------------------------------------