#include "stdafx.h"
#include "resource.h"

#include "SimpleIni.h"
#include "ExportDlg.h"
#include "DefaultConfig.h"

/////////////////////////////////////////////////////////////////////////////
// ExportDialog dialog
BEGIN_MESSAGE_MAP(ExportDialog, CDialog)
END_MESSAGE_MAP()

ExportDialog::ExportDialog(FFXiHelper *pHelper, const CArray<CString, LPCTSTR> &Characters,
						   CSimpleIni *pIni_in, CWnd* pParent)
	: CDialog(ExportDialog::IDD, pParent), m_pHelper(pHelper), m_BitMask(0UL),
	  m_Characters(Characters), m_pIni(pIni_in), m_ColumnCount(0), m_ExportedCharsCount(0) {}

BOOL ExportDialog::OnInitDialog()
{
	CListCtrl *pList = static_cast<CListCtrl*>(GetDlgItem(IDC_EXPORT_LIST));
	int CharCount = m_Characters.GetCount();
	CButton *pButton;
	long Value;

	pList->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES);
	pList->InsertColumn(0, _T("Character"), LVCFMT_LEFT, 372);

	for (int CharIndex = 0; CharIndex < CharCount; ++CharIndex)
		pList->InsertItem(CharIndex, m_Characters[CharIndex]);

	Value = m_pIni->GetLongValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_NAME_KEY);
	pButton = static_cast<CButton*>(GetDlgItem(IDC_EXPORT_NAME));
	pButton->SetCheck((Value != 0L) ? BST_CHECKED : BST_UNCHECKED);

	Value = m_pIni->GetLongValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_ATTR_KEY);
	pButton = static_cast<CButton*>(GetDlgItem(IDC_EXPORT_ATTR));
	pButton->SetCheck((Value != 0L) ? BST_CHECKED : BST_UNCHECKED);

	Value = m_pIni->GetLongValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_DESC_KEY);
	pButton = static_cast<CButton*>(GetDlgItem(IDC_EXPORT_DESC));
	pButton->SetCheck((Value != 0L) ? BST_CHECKED : BST_UNCHECKED);

	Value = m_pIni->GetLongValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_TYPE_KEY);
	pButton = static_cast<CButton*>(GetDlgItem(IDC_EXPORT_TYPE));
	pButton->SetCheck((Value != 0L) ? BST_CHECKED : BST_UNCHECKED);

	Value = m_pIni->GetLongValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_RACES_KEY);
	pButton = static_cast<CButton*>(GetDlgItem(IDC_EXPORT_RACES));
	pButton->SetCheck((Value != 0L) ? BST_CHECKED : BST_UNCHECKED);

	Value = m_pIni->GetLongValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_LEVEL_KEY);
	pButton = static_cast<CButton*>(GetDlgItem(IDC_EXPORT_LEVEL));
	pButton->SetCheck((Value != 0L) ? BST_CHECKED : BST_UNCHECKED);

	Value = m_pIni->GetLongValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_JOBS_KEY);
	pButton = static_cast<CButton*>(GetDlgItem(IDC_EXPORT_JOBS));
	pButton->SetCheck((Value != 0L) ? BST_CHECKED : BST_UNCHECKED);

	Value = m_pIni->GetLongValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_REMARKS_KEY);
	pButton = static_cast<CButton*>(GetDlgItem(IDC_EXPORT_REMARKS));
	pButton->SetCheck((Value != 0L) ? BST_CHECKED : BST_UNCHECKED);

	return CDialog::OnInitDialog();
}

void ExportDialog::OnOK()
{
	CListCtrl *pList = static_cast<CListCtrl*>(GetDlgItem(IDC_EXPORT_LIST));
	int ListItemCount = pList->GetItemCount();
	CButton *pButton;	
	bool Checked;

	m_ExportedChars.RemoveAll();

	for (int ItemIndex = 0; ItemIndex < ListItemCount; ++ItemIndex)
	{
		Checked = (pList->GetCheck(ItemIndex) == BST_CHECKED);
		m_ExportedChars.InsertAt(ItemIndex, Checked);

		if (Checked)
			++m_ExportedCharsCount;
	}

	ASSERT(m_Characters.GetCount() == m_ExportedChars.GetCount());

	pButton = static_cast<CButton*>(GetDlgItem(IDC_EXPORT_NAME));
	Checked = (pButton->GetCheck() == BST_CHECKED);
	m_pIni->SetLongValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_NAME_KEY,
						 Checked ? 1L, ++m_ColumnCount, m_BitMask |= EXPORT_NAME : 0L);

	pButton = static_cast<CButton*>(GetDlgItem(IDC_EXPORT_ATTR));
	Checked = (pButton->GetCheck() == BST_CHECKED);
	m_pIni->SetLongValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_ATTR_KEY,
						 Checked ? 1L, ++m_ColumnCount, m_BitMask |= EXPORT_ATTR : 0L);

	pButton = static_cast<CButton*>(GetDlgItem(IDC_EXPORT_DESC));
	Checked = (pButton->GetCheck() == BST_CHECKED);
	m_pIni->SetLongValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_DESC_KEY,
						 Checked ? 1L, ++m_ColumnCount, m_BitMask |= EXPORT_DESC : 0L);

	pButton = static_cast<CButton*>(GetDlgItem(IDC_EXPORT_TYPE));
	Checked = (pButton->GetCheck() == BST_CHECKED);
	m_pIni->SetLongValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_TYPE_KEY,
						 Checked ? 1L, ++m_ColumnCount, m_BitMask |= EXPORT_TYPE : 0L);

	pButton = static_cast<CButton*>(GetDlgItem(IDC_EXPORT_RACES));
	Checked = (pButton->GetCheck() == BST_CHECKED);
	m_pIni->SetLongValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_RACES_KEY,
						 Checked ? 1L, ++m_ColumnCount, m_BitMask |= EXPORT_RACES : 0L);

	pButton = static_cast<CButton*>(GetDlgItem(IDC_EXPORT_LEVEL));
	Checked = (pButton->GetCheck() == BST_CHECKED);
	m_pIni->SetLongValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_LEVEL_KEY,
						 Checked ? 1L, ++m_ColumnCount, m_BitMask |= EXPORT_LEVEL : 0L);

	pButton = static_cast<CButton*>(GetDlgItem(IDC_EXPORT_JOBS));
	Checked = (pButton->GetCheck() == BST_CHECKED);
	m_pIni->SetLongValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_JOBS_KEY,
						 Checked ? 1L, ++m_ColumnCount, m_BitMask |= EXPORT_JOBS : 0L);

	pButton = static_cast<CButton*>(GetDlgItem(IDC_EXPORT_REMARKS));
	Checked = (pButton->GetCheck() == BST_CHECKED);
	m_pIni->SetLongValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_REMARKS_KEY,
						 Checked ? 1L, ++m_ColumnCount, m_BitMask |= EXPORT_RMKS : 0L);

	if (m_ColumnCount > 0 && m_ExportedCharsCount > 0 && m_BitMask != 0UL)
		CDialog::OnOK();
	else
		AfxMessageBox(_T("Nothing to export"), MB_OK | MB_ICONEXCLAMATION);
}