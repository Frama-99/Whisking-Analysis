// SaveSeqDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demograb.h"
#include "SaveSeqDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaveSeqDlg

IMPLEMENT_DYNAMIC(CSaveSeqDlg, CFileDialog)

CSaveSeqDlg::CSaveSeqDlg(eHCompression& compression,LPCTSTR filename, CWnd* parentWnd)
	:CFileDialog(FALSE, _T(".seq"), filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
      _T("Norpix Sequence File (*.seq)|*.seq||"), parentWnd, 0, 0)
{
	SetTemplate(0, IDD_SEQTYPE_DLG);
	Compression = &compression;
}


BEGIN_MESSAGE_MAP(CSaveSeqDlg, CFileDialog)
	//{{AFX_MSG_MAP(CSaveSeqDlg)
	ON_CBN_SELCHANGE(IDC_HERMES_SEQUNECE_TYPE, OnSelchangeSeqTypelist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CSaveSeqDlg::DoDataExchange(CDataExchange* pDX)
{
    CFileDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveSeqDlg)
	DDX_Control(pDX, IDC_HERMES_SEQUNECE_TYPE, SeqTypeList);
	//}}AFX_DATA_MAP
}

BOOL CSaveSeqDlg::OnInitDialog() 
{
	CFileDialog::OnInitDialog();
	
	SeqTypeList.AddString (_T("RAW"));
	SeqTypeList.AddString (_T("JPEG"));
	SeqTypeList.AddString (_T("RLE"));
	SeqTypeList.AddString (_T("Huffman"));
	SeqTypeList.AddString (_T("LZ"));
    SeqTypeList.AddString (_T("H264"));
	SeqTypeList.SetCurSel (0);
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSaveSeqDlg::OnSelchangeSeqTypelist()
{
	long index = SeqTypeList.GetCurSel ();
	switch(index)
	{
		default:
		case 0:{*Compression = H_COMPRESSION_NONE;break;}
		case 1:{*Compression = H_COMPRESSION_JPEG;break;}
		case 2:{*Compression = H_COMPRESSION_RLE;break;}
		case 3:{*Compression = H_COMPRESSION_HUFFMAN;break;}
		case 4:{*Compression = H_COMPRESSION_LZ;break;}
        case 5:{*Compression = H_COMPRESSION_H264;break;}
	}
}