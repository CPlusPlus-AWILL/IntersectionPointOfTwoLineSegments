
// Segment2D.h : main header file for the Segment2D application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CSegment2DApp:
// See Segment2D.cpp for the implementation of this class
//

class CSegment2DApp : public CWinApp
{
public:
	CSegment2DApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSegment2DApp theApp;
