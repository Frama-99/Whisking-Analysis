#if !defined(AFX_DEMOGRAB_H__24A6A35A_04A3_4D76_8AA9_94A994C97B06__INCLUDED_)
#define AFX_DEMOGRAB_H__24A6A35A_04A3_4D76_8AA9_94A994C97B06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"

class CDemoGrabApp : public CWinApp
{
public:
	CDemoGrabApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoGrabApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CDemoGrabApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMOGRAB_H__24A6A35A_04A3_4D76_8AA9_94A994C97B06__INCLUDED_)
