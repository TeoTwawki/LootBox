// LootBox.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CLootBoxApp:
// See LootBox.cpp for the implementation of this class
//

class CLootBoxApp : public CWinApp
{
#ifdef GDIPLUS_IMAGE_RESIZING
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR  gdiplusToken;
#endif

public:
	CLootBoxApp();

// Overrides
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CLootBoxApp LootBoxApp;