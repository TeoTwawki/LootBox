#ifndef __TOOLTIP_LIST_CLASS__
#define __TOOLTIP_LIST_CLASS__

enum InventoryListColumns
{
	INVENTORY_LIST_COL_NAME = 0,
	INVENTORY_LIST_COL_LOCATION,
	INVENTORY_LIST_COL_ATTR,
	INVENTORY_LIST_COL_DESCRIPTION,
	INVENTORY_LIST_COL_SLOT,
	INVENTORY_LIST_COL_RACES,
	INVENTORY_LIST_COL_LEVEL,
	INVENTORY_LIST_COL_JOBS,
	INVENTORY_LIST_COL_REMARKS,
	INVENTORY_LIST_COL_COUNT
};

class FFXiItemList : public CListCtrl
{
	// Construction
public:
	FFXiItemList(){}
	virtual ~FFXiItemList(){}

	bool ShowToolTip(const CPoint& pt) const;
	bool IsCompact() { return m_CompactList; }
	LPTSTR GetToolTipText(int nRow, int nCol);
	bool AddItem(InventoryItem *pItem, int &ItemIndex);
	BOOL OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	void CellHitTest(const CPoint& pt, int& nRow, int& nCol) const;
	void UpdateItemText(const TCHAR* pText, int ItemIndex, int SubIndex = 0);
	void UpdateItemData(InventoryItem *pItem, int ItemIndex, int SubIndex = 0);
	void BlockRedraw(bool BlockRedraw = true) { m_BlockRedraw = BlockRedraw; }
	void SetCompactList(LONG CompactList = 0) { m_CompactList = (CompactList != 0); }

	static int CALLBACK ColumnSortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

protected:
	bool m_CompactList;
	bool m_BlockRedraw;

	static bool m_SortAsc;
	static int m_SortedColumn;

	void GetItemAt(int Index, int SubItem, int &Icon, CString &Text);
	void SetItemAt(int Index, int SubItem, int Icon, CString &Text);

	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnColumnSort(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()
};

#endif//__TOOLTIP_LIST_CLASS__