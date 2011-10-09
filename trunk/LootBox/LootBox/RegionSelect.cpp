// RegionSelect.cpp : implementation file
//

#include "stdafx.h"
#include "LootBox.h"
#include "resource.h"

#include "FFXIHelper.h"
#include "RegionSelect.h"


// RegionSelect dialog

IMPLEMENT_DYNAMIC(RegionSelect, CDialog)

RegionSelect::RegionSelect(int Regions, CWnd* pParent) : CDialog(RegionSelect::IDD, pParent)
{
	m_Regions = Regions;
	m_Region = 0;
}

RegionSelect::~RegionSelect()
{
}

void RegionSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL RegionSelect::OnInitDialog()
{
	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_REGION_COMBO);
	int ItemIndex = 0;

	if ((m_Regions & FFXI_REGION_FLAG_JP) == FFXI_REGION_FLAG_JP)
	{
		ItemIndex = pCombo->AddString(_T("Japan"));
		m_Region = FFXI_REGION_JP;
		pCombo->SetItemData(ItemIndex, m_Region);
	}
	if ((m_Regions & FFXI_REGION_FLAG_US) == FFXI_REGION_FLAG_US)
	{
		ItemIndex = pCombo->AddString(_T("America"));
		m_Region = FFXI_REGION_US;
		pCombo->SetItemData(ItemIndex, m_Region);
	}
	if ((m_Regions & FFXI_REGION_FLAG_EU) == FFXI_REGION_FLAG_EU)
	{
		ItemIndex = pCombo->AddString(_T("Europe"));
		m_Region = FFXI_REGION_EU;
		pCombo->SetItemData(ItemIndex, m_Region);
	}

	pCombo->SetCurSel(ItemIndex);

	return TRUE;
}

int RegionSelect::GetSelectedRegion()
{
	return m_Region;
}
BEGIN_MESSAGE_MAP(RegionSelect, CDialog)
	ON_CBN_SELCHANGE(IDC_REGION_COMBO, &RegionSelect::OnChangeRegionCombo)
END_MESSAGE_MAP()


// RegionSelect message handlers

void RegionSelect::OnChangeRegionCombo()
{
	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_REGION_COMBO);
	int Selected = pCombo->GetCurSel();

	m_Region = (int)pCombo->GetItemData(Selected);
}
