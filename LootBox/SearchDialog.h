#pragma once

#ifndef MSG_SEARCH_CLOSE
	#define MSG_SEARCH_CLOSE (WM_APP + 0xc)
#endif

class SearchDialog : public CDialog
{
	// Construction
public:
	SearchDialog(FFXiHelper *pHelper, CWnd* pParent = NULL);   // standard constructor
	SearchParams* GetSearchParams() { return m_pParams; }
	void GetBitMask(const CListCtrl *pList, DWORD &Field);

	// Dialog Data
	enum { IDD = IDD_SEARCH_DIALOG };

protected:

	FFXiHelper *m_pHelper;

	CACEdit	m_SearchEdit;

	DWORD m_JobsBitMask;
	DWORD m_SlotBitMask;
	DWORD m_RacesBitMask;
	DWORD m_SkillsBitMask;
	int m_MinLevel;
	int m_MaxLevel;
	int m_MinDelay;
	int m_MaxDelay;
	int m_MinDmg;
	int m_MaxDmg;
	int m_MinDef;
	int m_MaxDef;
	bool m_Rare;
	bool m_Exclusive;
	bool m_Stackable;

	SearchParams *m_pParams;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnDestroy();
	
	DECLARE_MESSAGE_MAP()
};