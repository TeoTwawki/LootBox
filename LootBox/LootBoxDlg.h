// LootBoxDlg.h : header file
//
#ifndef __LOOTBOX_DLG_CLASS__
#define __LOOTBOX_DLG_CLASS__

// CLootBoxDlg dialog
class CLootBoxDlg : public ETSLayoutDialog
{
// Construction
public:
	CLootBoxDlg(CWnd* pParent = NULL);	// standard constructor

	BOOL InitDialog();
	BOOL InitLayout();
	BOOL DefaultConfig();
	void LoadGlobalMap(bool Update = false);
	void UpdateStatus();
	void SetCompactListMenu(LONG CompactList);
	BOOL RefreshList(const ItemArray *pItemList);
	void SetLanguageMenu(int Language, bool Check = true);
	static HBITMAP GetItemIcon(FFXiIconInfo *pIconInfo, CDC *pDC, int Width, int Height, int ItemID = -1);

// Dialog Data
	enum { IDD = IDD_LOOTBOX_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
	HICON m_hIcon;
	int m_Region;
	int m_Language;
	bool m_InitDone;
	bool m_CompactList;
	BYTE *m_pFileData;
	HACCEL m_hAcceleratorTable;
	CMenu *m_pMainMenu;
	CMenu *m_pPopMenu;
	CSimpleIni *m_pIni;	
	FFXiHelper *m_pHelper;
	CString m_CurrentFile;
	CImageList *m_pIconList;
	CImageList *m_pItemIconList;
	CString m_FFXiInstallPath;
	FFXiItemList m_InventoryList;
	CArray<CString, LPCTSTR> m_CharacterIDs;
	CArray<CString, LPCTSTR> m_CharacterNames;
	CArray<CString, LPCTSTR> m_InventoryFiles;
	CArray<CString, LPCTSTR> m_InventoryNames;
	CMap<int, int, SearchData*, SearchData*> m_SearchTabs;
	int m_SelectedChar, m_SelectedTab, m_CharactersCount, m_ItemsCount;

	CharacterMap m_GlobalMap;
	CProgress_Dlg m_ProgressDlg;
	SearchDialog *m_pSearchDlg;

	void RemoveItemIcons();
	void DeleteGlobalMap();
	int GlobalMapCount();
	void GetSearchResults(SearchData *pParams);
	ItemArray* GetItemMap(int SelectedCharIndex, int SelectedTabIndex);
	void SetItemMapAt(int SelectedCharIndex, int SelectedTabIndex, ItemArray *pItemList);

#ifdef _DEBUG
	void DumpGlobalMap();
#endif

	bool IsValidChar() { return (m_SelectedChar >= 0 && m_SelectedChar < m_CharacterIDs.GetCount()); }
	bool IsValidChar(int SelectedChar) { return (SelectedChar >= 0 && SelectedChar < m_CharacterIDs.GetCount()); }
	bool IsInventoryTab() { return (m_SelectedTab >= 0 && m_SelectedTab < m_InventoryFiles.GetCount()); }
	bool IsInventoryTab(int SelectedTag) { return (SelectedTag >= 0 && SelectedTag < m_InventoryFiles.GetCount()); }
	LRESULT OnSearchClose(WPARAM wParam, LPARAM lParam);
	// Generated message map functions
	virtual void OnOK(){}
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG *pMsg);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	afx_msg void OnMenuChange(CCmdUI* pCmdUI);
	afx_msg void OnSearch(CCmdUI* pCmdUI);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnToolTipNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListItemClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEndItemEdit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKeyDownListItem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnInventoryColumnResize(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnInventoryTabsChange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCharactersListChange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRightClickInventoryTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnInventoryColumnAutosize(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemDoubleClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRefreshClose();
};


#endif//__LOOTBOX_DLG_CLASS__