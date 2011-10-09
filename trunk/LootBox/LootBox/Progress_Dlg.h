#if !defined(_PROGRESS_DLG_H_INCLUDED_)
#define _PROGRESS_DLG_H_INCLUDED_

#pragma once

/* \defgroup DialogScreen CDialog based screen
 */

/*! \brief Progression bar used when opening / importing a project, importing a dictionary...
 * \image html CProgress_Dlg.png
 * \ingroup MFC DialogScreen
 */
class CProgress_Dlg : public CDialog
{
// Construction
public:
	CProgress_Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProgress_Dlg)
	enum { IDD = IDD_PROGRESS };
	CProgressCtrl	m_Progress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgress_Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProgress_Dlg)
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_PROGRESS_DLG_H_INCLUDED_)
