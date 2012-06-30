// SearchDialog.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"

#include "ACEdit.h"
#include "FFXIHelper.h"
#include "SearchHandler.h"
#include "SearchDialog.h"

#include <iostream>
#include <sstream>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// SearchDialog dialog
BEGIN_MESSAGE_MAP(SearchDialog, CDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

SearchDialog::SearchDialog(FFXiHelper *pHelper, CWnd* pParent): CDialog(SearchDialog::IDD, pParent)
{
	m_pParams = NULL;
	m_pHelper = pHelper;
}


BOOL SearchDialog::OnInitDialog()
{
	const FFXiStringAssoc *pStringTable = FFXiHelper::JobsStringTable;
	CListCtrl *pList = (CListCtrl*)GetDlgItem(IDC_LIST_JOBS);
	int ItemIndex = 0;

	while(pStringTable->Key != -1)
	{   
		ItemIndex = pList->InsertItem(ItemIndex, pStringTable->pString);
		pList->SetItemData(ItemIndex, (LPARAM)pStringTable->Key);
		ItemIndex++;

		pStringTable++;   
	}

	// fill the slot list
	pList = (CListCtrl*)GetDlgItem(IDC_LIST_SLOTS); 
	pStringTable = FFXiHelper::SlotStringTable;
	ItemIndex = 0;

	while(pStringTable->Key != -1)
	{   
		if (pStringTable->Key != FFXI_SLOT_FLAG_R_EAR && pStringTable->Key != FFXI_SLOT_FLAG_R_RING)
		{
			ItemIndex = pList->InsertItem(ItemIndex, pStringTable->pString);
			pList->SetItemData(ItemIndex, (LPARAM)pStringTable->Key);
			ItemIndex++;
		}

		pStringTable++;   
	}

	// fill the skill list
	pList = (CListCtrl*)GetDlgItem(IDC_LIST_SKILLS); 
	pStringTable = FFXiHelper::SkillStringTable;
	ItemIndex = 0;

	while(pStringTable->Key != -1)
	{   
		if (pStringTable->Key != FFXI_SKILL_TYPE_BST_JUG &&
			(pStringTable->Key < FFXI_SKILL_TYPE_DIVINE_MAGIC ||
			 pStringTable->Key > FFXI_SKILL_TYPE_SINGING))
		{
			ItemIndex = pList->InsertItem(ItemIndex, pStringTable->pString);
			pList->SetItemData(ItemIndex, (LPARAM)pStringTable->Key);
			ItemIndex++;
		}

		pStringTable++;   
	}

	// fill the races list
	pList = (CListCtrl*)GetDlgItem(IDC_LIST_RACES); 
	pStringTable = FFXiHelper::RacesStringTable;
	ItemIndex = 0;

	ItemIndex = pList->InsertItem(ItemIndex, _T("All \u2642"));
	pList->SetItemData(ItemIndex++, (LPARAM)FFXI_RACE_FLAG_MALE);

	ItemIndex = pList->InsertItem(ItemIndex, _T("All \u2640"));
	pList->SetItemData(ItemIndex++, (LPARAM)FFXI_RACE_FLAG_FEMALE);

	while(pStringTable->Key != -1)
	{   
		ItemIndex = pList->InsertItem(ItemIndex, pStringTable->pString);
		pList->SetItemData(ItemIndex, (LPARAM)pStringTable->Key);
		ItemIndex++;

		pStringTable++;   
	}

	UpdateData(FALSE);

	if (IsWindow(m_SearchEdit))
	{
		m_SearchEdit.SetMode(_MODE_CURSOR_O_LIST_ | _MODE_FIND_ALL_);
	}

	GetDlgItem(IDC_SEARCH_TERM)->SetFocus();

	return FALSE;
}

void SearchDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SEARCH_TERM, m_SearchEdit);
}

void SearchDialog::OnOK()
{
	CListCtrl *pList = (CListCtrl*)GetDlgItem(IDC_LIST_JOBS);
	CButton *pButton = (CButton*)GetDlgItem(IDC_CHECK_RARE);
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_MIN_LEVEL);
	CString EditValue;
	
	m_pParams = new SearchParams;
	SecureZeroMemory(m_pParams, sizeof(SearchParams));

	// fill the job bit mask
	GetBitMask(pList, m_pParams->JobsBitMask);
	// fill the slot bit mask
	pList = (CListCtrl*)GetDlgItem(IDC_LIST_SLOTS);
	GetBitMask(pList, m_pParams->SlotBitMask);
	// fill the races bit mask
	pList = (CListCtrl*)GetDlgItem(IDC_LIST_RACES);
	GetBitMask(pList, m_pParams->RacesBitMask);
	// fill the skill bit mask 
	pList = (CListCtrl*)GetDlgItem(IDC_LIST_SKILLS);
	GetBitMask(pList, m_pParams->Skill);

	// get the rare flag
	m_pParams->Rare = (pButton->GetState() == 1);
	// get the exclusive flag
	pButton = (CButton*)GetDlgItem(IDC_CHECK_EX);
	m_pParams->Exclusive = (pButton->GetState() == 1);

	// get the min level value
	pEdit->GetWindowText(EditValue);
	m_pParams->MinLevel = _ttoi(EditValue);
	// get the max level value
	pEdit = (CEdit*)GetDlgItem(IDC_MAX_LEVEL);
	pEdit->GetWindowText(EditValue);
	m_pParams->MaxLevel = _ttoi(EditValue);

	// get the min delay value
	pEdit = (CEdit*)GetDlgItem(IDC_MIN_DELAY);
	pEdit->GetWindowText(EditValue);
	m_pParams->MinDelay = _ttoi(EditValue);
	// get the max delay value
	pEdit = (CEdit*)GetDlgItem(IDC_MAX_DELAY);
	pEdit->GetWindowText(EditValue);
	m_pParams->MaxDelay = _ttoi(EditValue);

	// get the min damage value
	pEdit = (CEdit*)GetDlgItem(IDC_MIN_DMG);
	pEdit->GetWindowText(EditValue);
	m_pParams->MinDmg = _ttoi(EditValue);
	// get the max damage value
	pEdit = (CEdit*)GetDlgItem(IDC_MAX_DMG);
	pEdit->GetWindowText(EditValue);
	m_pParams->MaxDmg = _ttoi(EditValue);

	// get the min defense value
	pEdit = (CEdit*)GetDlgItem(IDC_MIN_DEF);
	pEdit->GetWindowText(EditValue);
	m_pParams->MinDef = _ttoi(EditValue);
	// get the max defense value
	pEdit = (CEdit*)GetDlgItem(IDC_MAX_DEF);
	pEdit->GetWindowText(EditValue);
	m_pParams->MaxDef = _ttoi(EditValue);

	// get the search term
	pEdit = (CEdit*)GetDlgItem(IDC_SEARCH_TERM);
	pEdit->GetWindowText(EditValue);

	if (EditValue.IsEmpty() == false)
		m_pParams->pSearchTerm = _tcsdup(EditValue.MakeLower());

	// notify the main window the user validated the search
	GetParent()->PostMessage(MSG_SEARCH_CLOSE);

	CDialog::OnOK();
}

void SearchDialog::GetBitMask(const CListCtrl *pList, DWORD &Field)
{
	int ListIndex = -1;
	DWORD Flag = 0;

	if (pList != NULL)
	{
		do
		{
			ListIndex = pList->GetNextItem(ListIndex, LVNI_SELECTED);
			if (ListIndex != -1)
			{
				Flag = (DWORD)pList->GetItemData(ListIndex);
				Field |= Flag;
			}

		}
		while (ListIndex != -1);
	}
}

void SearchDialog::OnDestroy()
{
	m_pParams = NULL;
	m_pHelper = NULL;
}