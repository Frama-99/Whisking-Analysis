// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "DemoGrab.h"
#include "ChildView.h"
#include "AdvanceSettingsDlg.h"
#include "SaveSeqDlg.h"
#include "NewProjectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CChildView::CChildView()
{
    LastImage = NULL;
    Live = false;
    SequenceReady = false;
    GrabAudioModule = NULL;
	CaptureAudio = false;
}
//-----------------------------------------------------------------------------
CChildView::~CChildView()
{
    //If a grabber was successfully loaded, we must unload it as we quit
    if(GrabModule.GetCurrentGrabber() != H_UNKNOWN)
    {
        CloseSequence();
        StopLive();
        GrabModule.Unload();
    }
}
//-----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CChildView,CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
    ON_COMMAND(ID_GRABBER_ADVANCESETTINGS, &CChildView::OnGrabberAdvanceSettings)
    ON_COMMAND(ID_GRABBER_ADVANCESETTINGSMODALESS, &CChildView::OnGrabberAdvanceSettingsModaless)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------------
BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}


//=============================================================================
//GRABBER RELATED
//=============================================================================
bool CChildView::LoadGrabber()
{
    CGrabberListDlg dlg;
    dlg.SetGrabModule(&GrabModule);
    
    //Show the "Grabber Selection" Dialog
    if(dlg.DoModal() != IDOK)
        return false;

	//Set the callbacks
    GrabModule.SetCallbacks(this, 0, this);

    //Audio stuff
    GrabAudioModule = GrabModule.GetGrabAudioModule();

	//Prepare audio
	if(GrabAudioModule)
	{
		AudioRecorder.SetCallback(this);
		GrabAudioModule->EnableAudioParams(true);
		GrabAudioModule->SetAudioStreamEnabled(true);
		GrabAudioModule->SetAudioBuffer(48);
		AudioRecorder.SetAudioDriver(GrabAudioModule);	
	}
    return true;
}
//-----------------------------------------------------------------------------
void CChildView::StartLive()
{
    if(Live)
        return; //already in live
    
    //This part prevent the live feed from starting automatically,
    //this is usefull if you need to adjust some settings (such as configuration
    //files) before starting to stream.
    BYTE keybState[256];
    GetKeyboardState(keybState);
    if(keybState[VK_SCROLL] == 1)
    {
        MessageBox(_T("DemoGrab won't go in live while 'scroll lock' is on."), 
            _T("SCROLL LOCK MODE"), MB_OK);
        return;
    }
	
    //Tell the grabber to start sending us images
    GrabModule.StartStreaming();

    Live = true;
}
//-----------------------------------------------------------------------------
void CChildView::StopLive()
{
    if(Live)
    {
        GrabModule.StopStreaming();
        LastImage = NULL;
        Live = false;
    }
}
//-----------------------------------------------------------------------------
bool CChildView::IsLive()
{
    return Live;
}
//-----------------------------------------------------------------------------
void CChildView::OnImageReceived(HImage* image, unsigned int userData, void* userPtr)
{
    //An image captured by the grabber was just received, display it
    LastImage = image;

    //If a sequence is open, save the image into it
    if(SequenceReady)
        Sequence.Write(image);

    Invalidate(false); //will trigger an OnPaint (do not directly call OnPaint)
}
//-----------------------------------------------------------------------------
void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

    if(LastImage != NULL)
        ImageViewer.ShowImage(dc.m_hDC, LastImage, 0, 0);
}
//-----------------------------------------------------------------------------
void CChildView::ShowPropertyPages()
{
    //Some property pages aren't shown if you are in live. (such as the one 
    //allowing to set a new image size)  Stop the live then select the
    //"Property Pages" menu item to see them.
	GrabModule.ShowProperties(AfxGetMainWnd()->GetSafeHwnd());
}
//-----------------------------------------------------------------------------
void CChildView::ShowImageInfo()
{
    eHImageFormat format = GrabModule.GetImageFormat();
    CString type;

    switch(format)
    {
        case H_IMAGE_UNKNOWN:       type = _T("Format Unknown");break;
        case H_IMAGE_MONO:          type = _T("Monochrome image");break;
        case H_IMAGE_MONO_BAYER:    type = _T("Monochrome Bayer image");break;
        case H_IMAGE_BGR:           type = _T("BGR packed image");break;
        case H_IMAGE_BGRx:          type = _T("BGRx packed image");break;
        case H_IMAGE_RGB:           type = _T("RGB packed image");break;
        case H_IMAGE_PLANAR:        type = _T("BGR planar image");break;
		case H_IMAGE_YUV422:        type = _T("YUV422 packed image");break;
		case H_IMAGE_UVY422:        type = _T("UVY422 packed image");break;
		case H_IMAGE_UVY411:        type = _T("UVY411 packed image");break;
		case H_IMAGE_UVY444:        type = _T("UVY444 packed image");break;
        default :                   type = _T("? - unsupported format");
    }
    
    CString info;
    
    info.Format(_T("Image Format : %s\nImage Width : %u \
    \nImage Height : %u\nBit Depth : %u\nTrue Bit Depth : %u\nImage Size : %u bytes"), 
    type, GrabModule.GetImageWidth(), GrabModule.GetImageHeight(),
    GrabModule.GetImageBitDepth(), GrabModule.GetImageBitDepthReal(), 
    GrabModule.GetImageSizeBytes());

    AfxMessageBox(info);
}
//-----------------------------------------------------------------------------
void CChildView::GrabberSettings()
{
    CString msg = _T("GRABBER SETTINGS\n"); 

    //Enumerate every available settings
    long settingsCount = 0;
    
    GrabModule.GetSettingsCount(&settingsCount);

    //Enumerate every settings
    for(long index = 0; index < settingsCount; index++)
    {
        TCHAR featureName[100];
        long featureID;
        GrabModule.GetSettingsCaps(index, featureName, &featureID);

        msg += _T("\n- ");
        msg += featureName;
        msg += _T(" -\n");

        //Enumerate every possible value
        long valuesCount;
        GrabModule.GetValuesCount(featureID, &valuesCount);
        
        long currentValue;
        GrabModule.GetCurrentValues(featureID, &currentValue);

        for(long i = 0; i < valuesCount; i++)
        {
            TCHAR valueName[100];
            long valueID;

            GrabModule.GetValuesCaps(featureID, i, valueName, &valueID);
            msg += valueName;
            
            if(valueID == currentValue)
                msg += _T(" <-\n");
            else
                msg += _T("\n");
        }
    } 

	TCHAR settingName[100];
	TCHAR valueName[100];
	CString message;
	long settingID, valueID, lSettingsCount, lValueCount;
	int i, j;

	// Enumerate settings
	GrabModule.GetSettingsCount(&lSettingsCount);
	
    for(i = 0; i < lSettingsCount; i++)
    {
		GrabModule.GetSettingsCaps(i, settingName, &settingID);
		message.Format(_T("\t\t[%d] '%s', ID: %d"), i, settingName, settingID);

		OutputDebugString(message);

		long currentValue;
		GrabModule.GetCurrentValues(settingID, &currentValue);

		GrabModule.GetValuesCount(settingID, &lValueCount);

		for(j = 0; j < lValueCount; j++) 
        {
			GrabModule.GetValuesCaps(settingID, j, valueName, &valueID);
			
			if(valueID == currentValue)
				message.Format(_T("   =>[%d][%d] '%s', ID: %d"), i, j, valueName, valueID);
			else
				message.Format(_T("\t\t\t  [%d][%d] '%s', ID: %d"), i, j, valueName, valueID);

			OutputDebugString(message);
		}
	}
}
//-----------------------------------------------------------------------------
void CChildView::GrabberAdjustments()
{
    CString msg = _T("GRABBER ADJUSTMENTS\n\n");
    CString temp;

    //Enumerate every available settings
    long adjCount = 0;
    
    GrabModule.GetAdjustmentCount(&adjCount);

    //Enumerate every settings
    for(long index = 0; index < adjCount; index++)
    {
        TCHAR featureName[100];
        long featureID;
        long min, max;
        double dMin, dMax;
        BOOL integer, logSlider, manual, automatic, oneShot;

        GrabModule.GetAdjustmentCaps(index, &featureID, featureName, &min, &max,
            &dMin, &dMax, &integer, &logSlider, &manual, &automatic, &oneShot);
        
        if(integer)
        {
            //Get the current value
            long current;
            GrabModule.GetIntegerAdjustment(featureID, &current);

            temp.Format(_T("%d - Feature Name : %s (ID:%d)\nRange [%d,%d], Current : %d\n%s, %s, %s, %s\n\n"), 
                index, featureName, featureID, min, max, current,
                logSlider ? _T("Log slider") : _T("Standard slider"),
                manual ? _T("Support manual") : _T("Don't support manual"),
                automatic ? _T("Support automatic") : _T("Don't support automatic"),
                oneShot ? _T("Support One Shot") : _T("Don't support One Shot"));
        }

        else
        {
            //Get the current value
            double current;
            GrabModule.GetDoubleAdjustment(featureID, &current);

            temp.Format(_T("%d - Feature Name : %s (ID:%d)\nRange double[%.02f,%.02f], Current : %.02f\n%s, %s, %s, %s\n\n"), 
                index, featureName, featureID, dMin, dMax, current,
                logSlider ? _T("Log slider") : _T("Standard slider"),
                manual ? _T("Support manual") : _T("Don't support manual"),
                automatic ? _T("Support automatic") : _T("Don't support automatic"),
                oneShot ? _T("Support One Shot") : _T("Don't support One Shot"));
        }

        msg += temp;
    }

    //AfxMessageBox(msg);
	OutputDebugString(msg);
} 


//=============================================================================
//SEQUENCER RELATED
//=============================================================================
void CChildView::NewSequence()
{
    //Setup the File dialog
	eHCompression compression = H_COMPRESSION_NONE;
	CSaveSeqDlg fd(compression);
    
    //Show the dialog box
    if(fd.DoModal() != IDOK)
	    return; //user clicked [Cancel]

    if(Sequence.Create(fd.GetPathName(), compression, 0, 80))
	   SequenceReady = true;
	
    if(GrabAudioModule && IsAudioCaptureEnabled())
    {
        //Create the audio file
	    CString audioFileName = fd.GetPathName().Left(fd.GetPathName().GetLength() - 3) + "aud";
	    AudioFile.Create(H_AUDIO_FILE_AUD_PCM, audioFileName);
	    AudioRecorder.SetAudioFile(&AudioFile);
	    AudioRecorder.StartRecording();
    }
}
//-----------------------------------------------------------------------------
void CChildView::CloseSequence()
{
    if(SequenceReady)
    {
        SequenceReady = false;
        Sequence.Close();

		if(GrabAudioModule)
        {
            AudioRecorder.StopRecording();
		    AudioFile.Close();
        }
    }
}
//-----------------------------------------------------------------------------
bool CChildView::IsSequenceReady()
{
    return SequenceReady;
}
//-----------------------------------------------------------------------------
bool CChildView::IsAudioSupported()
{
    return GrabAudioModule != NULL;
}
//-----------------------------------------------------------------------------
bool CChildView::IsAudioCaptureEnabled()
{
    return CaptureAudio;
}
//-----------------------------------------------------------------------------
void CChildView::ToggleAudioCapture()
{
    CaptureAudio = ! CaptureAudio;
}
//-----------------------------------------------------------------------------
void CChildView::OnGrabberAdvanceSettings()
{
    GrabModule.DoModalAdvanceSettingsWindow();
}
//-----------------------------------------------------------------------------
void CChildView::OnGrabberAdvanceSettingsModaless()
{
    CAdvanceSettingsDlg dlg(&GrabModule);
    dlg.DoModal();
}

//=============================================================================
//PROJECT RELATED
//=============================================================================
void CChildView::NewProject()
{
    NewProjectDlg dlg;
    dlg.DoModal();
}