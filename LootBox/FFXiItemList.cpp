#include "stdafx.h"

#include "FFXIHelper.h"
#include "FFXiItemList.h"

bool FFXiItemList::m_SortAsc = true;
int FFXiItemList::m_SortedColumn = -1;

int FFXiItemList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	EnableToolTips();
	m_CompactList = false;

	return 0;
}

void FFXiItemList::OnColumnSort(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW *pLV = (NMLISTVIEW*)pNMHDR;
	int PrevColumn = m_SortedColumn;

	m_SortedColumn = pLV->iItem;

	if (m_SortedColumn == PrevColumn)
		m_SortAsc = !m_SortAsc;
	else
		m_SortAsc = true;

	SortItems(ColumnSortFunc, pLV->iItem);

	*pResult = 0;
}

int CALLBACK FFXiItemList::ColumnSortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int nRetVal;

	InventoryItem *pData1;
	InventoryItem *pData2;

	if (m_SortAsc)
	{
		pData1 = (InventoryItem*)lParam1;
		pData2 = (InventoryItem*)lParam2;
	}
	else
	{
		pData1 = (InventoryItem*)lParam2;
		pData2 = (InventoryItem*)lParam1;
	}

	switch(lParamSort)
	{
		default:
		case INVENTORY_LIST_COL_NAME:
			nRetVal = _tcscmp(pData1->ItemName, pData2->ItemName);
		break;

		case INVENTORY_LIST_COL_LOCATION:
			nRetVal = _tcscmp(pData1->LocationInfo.Location, pData2->LocationInfo.Location);
		break;

		case INVENTORY_LIST_COL_ATTR:
			nRetVal = _tcscmp(pData1->Attr, pData2->Attr);
		break;

		case INVENTORY_LIST_COL_DESCRIPTION:
			nRetVal = _tcscmp(pData1->ItemDescription, pData2->ItemDescription);
		break;
		case INVENTORY_LIST_COL_SLOT:
			nRetVal = _tcscmp(pData1->Slot, pData2->Slot);
		break;
		case INVENTORY_LIST_COL_RACES:
			nRetVal = _tcscmp(pData1->Races, pData2->Races);
		break;
		case INVENTORY_LIST_COL_LEVEL:
		{
			int Level1 = 0, Level2 = 0;

			if (pData1->ItemHdr.Type == ITEM_OBJECT_TYPE_ARMOR)
				Level1 = pData1->ArmorInfo.Level;
			else if (pData1->ItemHdr.Type == ITEM_OBJECT_TYPE_WEAPON)
				Level1 = pData1->WeaponInfo.Level;

			if (pData2->ItemHdr.Type == ITEM_OBJECT_TYPE_ARMOR)
				Level2 = pData2->ArmorInfo.Level;
			else if (pData2->ItemHdr.Type == ITEM_OBJECT_TYPE_WEAPON)
				Level2 = pData2->WeaponInfo.Level;

			// make sure empty values stay at the bottom of the list
			if (Level1 == 0 && m_SortAsc)
				nRetVal = 1; // force Level1 > Level2
			// make sure empty values stay at the bottom of the list
			else if (Level2 == 0 && m_SortAsc)
				nRetVal = -1; // force Level1 < Level2
			else if (Level1 == Level2)
				nRetVal = 0;
			else if (Level2 > Level1)
				nRetVal = -1;
			else
				nRetVal = 1;
		}
		break;
		case INVENTORY_LIST_COL_JOBS:
			nRetVal = _tcscmp(pData1->Jobs, pData2->Jobs);
		break;
		case INVENTORY_LIST_COL_REMARKS:
			nRetVal = _tcscmp(pData1->Remarks, pData2->Remarks);
		break;
	}

	return nRetVal;
}


BOOL FFXiItemList::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	TOOLTIPTEXT* pTTTW = (TOOLTIPTEXT*)pNMHDR;
	CPoint pt(GetMessagePos());
	int nRow, nCol;

	ScreenToClient(&pt);
	CellHitTest(pt, nRow, nCol);

	pTTTW->lpszText = GetToolTipText(nRow, nCol);
	pTTTW->hinst = 0;

	return TRUE;
}

void FFXiItemList::CellHitTest(const CPoint& pt, int& nRow, int& nCol) const
{
	LVHITTESTINFO lvhti = {0};

	lvhti.pt = pt;
	nRow = -1;
	nCol = -1;

	nRow = ListView_SubItemHitTest(m_hWnd, &lvhti);
	nCol = lvhti.iSubItem;

	if ((lvhti.flags & LVHT_ONITEMLABEL) != LVHT_ONITEMLABEL)
		nRow = -1;
}

LPTSTR FFXiItemList::GetToolTipText(int nRow, int nCol)
{
	int ItemCount = GetItemCount();

	if (nRow >= 0 && nRow < ItemCount)
	{
		InventoryItem *pItem = (InventoryItem*)GetItemData(nRow);

		if (pItem != NULL)
		{
			switch(nCol)
			{
				default:
				case 0:
					return pItem->ItemToolTip.GetBuffer();
				break;
				case INVENTORY_LIST_COL_ATTR:
					return pItem->Attr.GetBuffer();
				break;
				case INVENTORY_LIST_COL_DESCRIPTION:
					return pItem->ItemDescription.GetBuffer();
				break;
				case INVENTORY_LIST_COL_SLOT:
					return pItem->Slot.GetBuffer();
				break;
				case INVENTORY_LIST_COL_RACES:
					return pItem->Races.GetBuffer();
				break;
				case INVENTORY_LIST_COL_LEVEL:
					return pItem->Level.GetBuffer();
				break;
				case INVENTORY_LIST_COL_JOBS:
					return pItem->Jobs.GetBuffer();
				break;
				case INVENTORY_LIST_COL_REMARKS:
					return pItem->Remarks.GetBuffer();
				break;
			}
		}
	}

	return NULL;
}

bool FFXiItemList::ShowToolTip(const CPoint& pt) const
{
	// Lookup up the cell
	int nRow, nCol;

	CellHitTest(pt, nRow, nCol);

	return (nRow >= 0 && nCol >= 0);
}

void FFXiItemList::GetItemAt(int Index, int SubItem, int &Icon, CString &Text)
{
	LVITEM Item;

	Item.mask = LVIF_TEXT|LVIF_IMAGE ;
	Item.iItem = Index ;
	Item.iSubItem = SubItem;
	Item.pszText = Text.GetBuffer();

	GetItem(&Item);

	Icon = Item.iImage;
	Text.ReleaseBuffer();
}

void FFXiItemList::SetItemAt(int Index, int SubItem, int Icon, CString &Text)
{
	LVITEM Item;

	Item.mask = LVIF_TEXT | LVIF_IMAGE ;
	Item.iItem = Index ;
	Item.iImage = Icon;
	Item.iSubItem = SubItem;
	Item.pszText = Text.GetBuffer();

	SetItem(&Item);
}

void FFXiItemList::PreSubclassWindow()
{
	// Focus rectangle is not painted properly without double-buffering
#if (_WIN32_WINNT >= 0x501)
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_DOUBLEBUFFER);
#endif
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_SUBITEMIMAGES);
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_GRIDLINES);
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_LABELTIP);

	GetToolTips()->SetMaxTipWidth(512);

	CListCtrl::PreSubclassWindow();
}

bool FFXiItemList::AddItem(InventoryItem *pItem, int &ItemIndex)
{
	if (pItem != NULL)
	{
		ItemIndex = InsertItem(ItemIndex, pItem->ItemName, pItem->LocationInfo.ImageIndex);
		SetItemData(ItemIndex, (LPARAM)pItem);

		if (pItem->Attr.IsEmpty() == false)
			SetItemAt(ItemIndex, INVENTORY_LIST_COL_ATTR, -1, pItem->Attr);

		if (pItem->LocationInfo.Location.IsEmpty() == false)
			SetItemAt(ItemIndex, INVENTORY_LIST_COL_LOCATION, -1, pItem->LocationInfo.Location);

		if (pItem->ItemDescription.IsEmpty() == false)
			SetItemAt(ItemIndex, INVENTORY_LIST_COL_DESCRIPTION, -1, pItem->ItemDescription);

		if (pItem->Slot.IsEmpty() == false)
			SetItemAt(ItemIndex, INVENTORY_LIST_COL_SLOT, -1, pItem->Slot);

		if (pItem->Races.IsEmpty() == false)
			SetItemAt(ItemIndex, INVENTORY_LIST_COL_RACES, -1, pItem->Races);

		if (pItem->Level.IsEmpty() == false)
			SetItemAt(ItemIndex, INVENTORY_LIST_COL_LEVEL, -1, pItem->Level);

		if (pItem->Jobs.IsEmpty() == false)
			SetItemAt(ItemIndex, INVENTORY_LIST_COL_JOBS, -1, pItem->Jobs);

		if (pItem->Remarks.IsEmpty() == false)
			SetItemAt(ItemIndex, INVENTORY_LIST_COL_REMARKS, -1, pItem->Remarks);

		ItemIndex++;

		return true;
	}

	return false;
}

void FFXiItemList::UpdateItemData(InventoryItem *pItem, int ItemIndex, int SubIndex)
{
	if (pItem != NULL)
	{
		SetItemData(ItemIndex, (LPARAM)pItem);

		if (ItemIndex >= 0 && ItemIndex < GetItemCount() && SubIndex >= 0 && SubIndex < INVENTORY_LIST_COL_COUNT)
		{
			switch(SubIndex)
			{
				default:
				case 0:
					SetItemText(ItemIndex, SubIndex, pItem->ItemName);
				break;
				case INVENTORY_LIST_COL_ATTR:
					SetItemText(ItemIndex, SubIndex, pItem->Attr);
				break;
				case INVENTORY_LIST_COL_DESCRIPTION:
					SetItemText(ItemIndex, SubIndex, pItem->ItemDescription);
				break;
				case INVENTORY_LIST_COL_SLOT:
					SetItemText(ItemIndex, SubIndex, pItem->Slot);
				break;
				case INVENTORY_LIST_COL_RACES:
					SetItemText(ItemIndex, SubIndex, pItem->Races);
				break;
				case INVENTORY_LIST_COL_LEVEL:
					SetItemText(ItemIndex, SubIndex, pItem->Level);
				break;
				case INVENTORY_LIST_COL_JOBS:
					SetItemText(ItemIndex, SubIndex, pItem->Jobs);
				break;
				case INVENTORY_LIST_COL_REMARKS:
					SetItemText(ItemIndex, SubIndex, pItem->Remarks);
				break;
			}
		}
	}
}

void FFXiItemList::UpdateItemText(const TCHAR* pText, int ItemIndex, int SubIndex)
{
	if (pText != NULL && ItemIndex >= 0 && ItemIndex < GetItemCount() && SubIndex >= 0 && SubIndex < INVENTORY_LIST_COL_COUNT)
	{
		SetItemText(ItemIndex, SubIndex, pText);
	}
}

void FFXiItemList::OnPaint()
{
	if (m_BlockRedraw == false)
	{
		CListCtrl::OnPaint();
	}
}