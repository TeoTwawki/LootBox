// LootBox.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#include "ACEdit.h"
#include "SimpleIni.h"
#include "ETSLayout.h"

#include "FFXIHelper.h"
#include "SearchHandler.h"

using namespace ETSLayout;

#include "LootBox.h"
#include "FFXiItemList.h"
#include "SearchDialog.h"
#include "Progress_Dlg.h"
#include "LootBoxDlg.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif


// CLootBoxApp

BEGIN_MESSAGE_MAP(CLootBoxApp, CWinApp)
	//ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CLootBoxApp construction

CLootBoxApp::CLootBoxApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CLootBoxApp object

CLootBoxApp LootBoxApp;


// CLootBoxApp initialization

BOOL CLootBoxApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Standard initialization

	CLootBoxDlg MainDialog;

#ifdef GDIPLUS_IMAGE_RESIZING
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
#endif

	m_pMainWnd = &MainDialog;
	MainDialog.DoModal();

#ifdef GDIPLUS_IMAGE_RESIZING
	Gdiplus::GdiplusShutdown(gdiplusToken);
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
