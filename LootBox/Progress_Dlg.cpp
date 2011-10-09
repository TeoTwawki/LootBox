// Progress_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "Progress_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgress_Dlg dialog


CProgress_Dlg::CProgress_Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgress_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgress_Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CProgress_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgress_Dlg)
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgress_Dlg, CDialog)
	//{{AFX_MSG_MAP(CProgress_Dlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgress_Dlg message handlers

void CProgress_Dlg::OnCancel(void)
{
	// Avoid cancelling the progress bar
	//CDialog::OnCancel();
}
