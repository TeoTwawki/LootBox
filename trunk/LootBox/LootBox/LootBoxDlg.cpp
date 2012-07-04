// LootBoxDlg.cpp : implementation file
//

#include "stdafx.h"

#include "ACEdit.h"
#include "SimpleIni.h"
#include "ETSLayout.h"

#include "FFXIHelper.h"
#include "SearchHandler.h"
#include "DefaultConfig.h"

using namespace ETSLayout;

#include "LootBox.h"
#include "FFXiItemList.h"
#include "Progress_Dlg.h"
#include "SearchDialog.h"
#include "LootBoxDlg.h"
#include "RegionSelect.h"
#include "ExportDlg.h"

#include "CsvWriter.h"

#include <shlobj.h>

#ifdef GDIPLUS_IMAGE_RESIZING
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
#endif

BEGIN_MESSAGE_MAP(CLootBoxDlg, ETSLayoutDialog)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_MESSAGE(MSG_SEARCH_CLOSE, OnSearchClose)
	ON_COMMAND_RANGE(ID_LANGUAGE_JAPANESE, ID_VIEW_COMPACTLISTING, OnOptionsChange)
	ON_COMMAND(ID_FILE_EXPORT, OnExport)
	ON_COMMAND(ID_FILE_SEARCH, OnSearch)
	ON_NOTIFY(HDN_ITEMCLICK, IDC_CHAR_LIST, &CLootBoxDlg::OnListItemClick)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_CHAR_LIST, &CLootBoxDlg::OnEndItemEdit)
	ON_NOTIFY(LVN_KEYDOWN, IDC_CHAR_LIST, &CLootBoxDlg::OnKeyDownListItem)
	ON_NOTIFY(HDN_ENDTRACK, IDC_INVENTORY_LIST, &CLootBoxDlg::OnInventoryColumnResize)
	ON_NOTIFY(TCN_SELCHANGE, IDC_INVENTORY_TABS, &CLootBoxDlg::OnInventoryTabsChange)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CHAR_LIST, &CLootBoxDlg::OnCharactersListChange)
	ON_NOTIFY(NM_RCLICK, IDC_INVENTORY_TABS, &CLootBoxDlg::OnRightClickInventoryTab)
	ON_NOTIFY(HDN_DIVIDERDBLCLICK, IDC_INVENTORY_LIST, &CLootBoxDlg::OnInventoryColumnAutosize)
	ON_NOTIFY(NM_DBLCLK, IDC_INVENTORY_LIST, &CLootBoxDlg::OnItemDoubleClick)
	ON_COMMAND(ID_REFRESH_CLOSE, &CLootBoxDlg::OnRefreshClose)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(FFXiItemList, CListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY(HDN_ITEMCLICK, 0, OnColumnSort)
END_MESSAGE_MAP()

// CLootBoxDlg dialog
CLootBoxDlg::CLootBoxDlg(CWnd* pParent)
	: ETSLayoutDialog(CLootBoxDlg::IDD, pParent)
{
	m_ItemsCount = m_CharactersCount = m_SelectedChar = m_SelectedTab = 0;
	m_pFileData = (BYTE*)malloc(DATA_SIZE_INVENTORY+1);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pIni = new CSimpleIni(true, false, false);
	m_Language = INI_FILE_LANGUAGE_VALUE;
	m_pItemIconList = m_pIconList = NULL;
	m_pPopMenu = m_pMainMenu = NULL;
	m_hAcceleratorTable = NULL;
	m_pIni->SetSpaces(false);
	m_CompactList = false;
	m_pSearchDlg = NULL;
	m_InitDone = false;
	m_pHelper = NULL;
}

void CLootBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_INVENTORY_LIST, m_InventoryList);
	ETSLayoutDialog::DoDataExchange(pDX);
}

BOOL CLootBoxDlg::OnInitDialog()
{
	ETSLayoutDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	m_pMainMenu = new CMenu();
	m_pMainMenu->LoadMenu(IDR_MAIN_MENU);
	SetMenu(m_pMainMenu);

	m_pPopMenu = new CMenu();
	m_pPopMenu->LoadMenu(IDR_POP_INVTAB);

	// set the dialog layout
	if (InitLayout())
	{
		// load the configuration file
		if (DefaultConfig())
		{
			m_hAcceleratorTable = LoadAccelerators(NULL, MAKEINTRESOURCE(IDR_ACCELERATOR_SEARCH));
			InitDialog();

			m_InitDone = true;
		}
	}

	return FALSE;  // return TRUE  unless you set the focus to a control
}

BOOL CLootBoxDlg::InitDialog()
{
	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_INVENTORY_TABS);
	FFXiItemList *pList = (FFXiItemList*)GetDlgItem(IDC_CHAR_LIST);
	CLootBoxApp *pApp = (CLootBoxApp*)AfxGetApp();
	const TCHAR *pFFXiPath = NULL;
	const TBYTE *pValue, *pKey;

	// load the configuration file
	if (DefaultConfig())
	{
		m_pHelper = new FFXiHelper(m_Region);

		// retrieve the installation path from the configuration file
		pFFXiPath = m_pIni->GetValue(INI_FILE_CONFIG_SECTION, INI_FILE_GAME_DIRECTORY);
		SetLanguageMenu(m_Language);

		if (pFFXiPath != NULL)
		{
			m_FFXiInstallPath = pFFXiPath;
		}
		else
		{
			// retrieve the FFXI installation path from the registry
			m_FFXiInstallPath = m_pHelper->GetInstallPath(m_Region);
			// reading the registry failed: pick a directory manually
			if (m_FFXiInstallPath.IsEmpty())
			{
				LPCITEMIDLIST pSelectedPIDL;
				BROWSEINFO BrowseInfo;
				LPTSTR pPathBuffer;

				AfxMessageBox(_T("Failed to read the Final Fantasy XI installation folder from the registry.\nPlease choose the Final Fantasy XI path manually."), MB_ICONSTOP);
				SecureZeroMemory(&BrowseInfo, sizeof(BrowseInfo));

				BrowseInfo.hwndOwner = GetSafeHwnd();
				BrowseInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NONEWFOLDERBUTTON | BIF_USENEWUI;
				BrowseInfo.lpszTitle = _T("Please select the Final Fantasy XI installation folder");

				// select the directory manually
				pSelectedPIDL = SHBrowseForFolder(&BrowseInfo);
				pPathBuffer = m_FFXiInstallPath.GetBuffer(MAX_PATH);
				SHGetPathFromIDList(pSelectedPIDL, pPathBuffer);
				m_FFXiInstallPath.ReleaseBuffer();
			}
		}

		if (m_FFXiInstallPath.IsEmpty() == false)
		{
			CSimpleIni::TNamesDepend::const_reverse_iterator iKey;
			CString UserDataPath, CurrentUserID;
			CSimpleIni::TNamesDepend Keys;
			LONG Width, Height, X, Y;
			HICON hPolIco, hLootBox;
			int ImageIndex = 0;
			CFileFind Finder;
			UINT ItemIndex;

			m_FFXiInstallPath.TrimRight('\\');

			m_pIconList = new CImageList();

			m_pIni->SetValue(INI_FILE_CONFIG_SECTION, INI_FILE_GAME_DIRECTORY, m_FFXiInstallPath);
			m_pHelper->SetInstallPath(m_FFXiInstallPath);

			Width = m_pIni->GetLongValue(INI_FILE_CONFIG_SECTION, INI_FILE_WINDOW_WIDTH_KEY, INI_FILE_WINDOW_WIDTH_VALUE);
			Height = m_pIni->GetLongValue(INI_FILE_CONFIG_SECTION, INI_FILE_WINDOW_HEIGHT_KEY, INI_FILE_WINDOW_HEIGHT_VALUE);

			if (Width < INI_FILE_WINDOW_WIDTH_VALUE)
				Width = INI_FILE_WINDOW_WIDTH_VALUE;

			if (Height < INI_FILE_WINDOW_HEIGHT_VALUE)
				Height = INI_FILE_WINDOW_HEIGHT_VALUE;

			// center the dialog
			X = (LONG)(GetSystemMetrics(SM_CXSCREEN) - Width) / 2;
			Y = (LONG)(GetSystemMetrics(SM_CYSCREEN) - Height) / 2;
			MoveWindow(X, Y, Width, Height);

			// search string to enumerate the user IDs
			UserDataPath.Format(_T("%s\\%s\\*.*"), m_FFXiInstallPath, FFXI_PATH_USER_DATA);

			// add the PlayOnline icon to the image list
			if (m_pIconList->Create(16, 16, ILC_COLOR32 | ILC_MASK, 4, 1))
			{
				m_pIconList->Add(m_hIcon);
				hPolIco = pApp->LoadIcon(IDI_PLAYONLINE);
				m_pIconList->Add(hPolIco);
				hLootBox = pApp->LoadIcon(IDI_LOOTBOX);
				m_pIconList->Add(hLootBox);
				hLootBox = pApp->LoadIcon(IDI_SEARCH);
				m_pIconList->Add(hLootBox);
				pList->SetImageList(m_pIconList, LVSIL_SMALL);
			}

			// enumerate the inventory tabs
			if (m_pIni->GetAllKeys(INI_FILE_INVENTORY_SECTION, Keys))
			{
				LPTSTR pBuffer;
				TCITEM TabItem;
				CSize Size;

				Keys.sort(CSimpleIni::Entry::LoadOrder());
				pTabCtrl->SetImageList(m_pIconList);

				for (iKey = Keys.rbegin(); iKey != Keys.rend(); ++iKey)
				{
					pKey = iKey->pItem;

					pValue = m_pIni->GetValue(INI_FILE_INVENTORY_SECTION, pKey);

					if (pValue != NULL)
					{
						// add the tab
						ItemIndex = pTabCtrl->InsertItem(0, pValue, 2);
						// save the filename
						m_InventoryFiles.InsertAt(ItemIndex, pKey);
						m_InventoryNames.InsertAt(ItemIndex, pValue);
						// retrieve the buffer
						pBuffer = m_InventoryFiles.GetAt(ItemIndex).GetBuffer();
						// set the pointer as the item data
						TabItem.mask = TCIF_PARAM;
						TabItem.lParam = (DWORD_PTR)pBuffer;
						pTabCtrl->SetItem(ItemIndex, &TabItem);
					}
				}
			}

			// enumerate the user IDs
			if (Finder.FindFile(UserDataPath))
			{
				BOOL MoreData = TRUE;

				while(MoreData)
				{
					MoreData = Finder.FindNextFile();
					// look only for folders
					if (Finder.IsDirectory() && Finder.IsDots() == FALSE)
					{
						LPTSTR pBuffer;

						CurrentUserID = Finder.GetFileName();
						pValue = m_pIni->GetValue(INI_FILE_CHARACTERS_SECTION, CurrentUserID);

						if (pValue == NULL || _tcslen(pValue) == 0)
						{
							m_pIni->SetValue(INI_FILE_CHARACTERS_SECTION, CurrentUserID, _T(""));
							pValue = CurrentUserID.GetBuffer();
						}
						else if (pValue[0] == '@')
							continue;

						// add the user ID to the list
						ItemIndex = pList->InsertItem(0, pValue, 1);
						// retrieve the buffer and set the pointer as the item data
						m_CharacterIDs.InsertAt(ItemIndex, CurrentUserID);
						m_CharacterNames.InsertAt(ItemIndex, pValue);
						pBuffer = m_CharacterIDs.GetAt(ItemIndex).GetBuffer();
						pList->SetItemData(ItemIndex, (DWORD_PTR)pBuffer);
					}
				}

				Finder.Close();
			}

			//LoadGlobalMap();

			m_CharactersCount = (UINT)m_CharacterIDs.GetSize();
			m_SelectedChar = m_pIni->GetLongValue(INI_FILE_CONFIG_SECTION, INI_FILE_LAST_CHARACTER_KEY, 0);

			if (m_SelectedChar < 0)
				m_SelectedChar = 0;
			else if (m_CharactersCount > 0 && m_SelectedChar >= m_CharactersCount)
				m_SelectedChar = m_CharactersCount - 1;

			pList->SetFocus();
			pList->SetItemState(m_SelectedChar, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

			m_SelectedTab = m_pIni->GetLongValue(INI_FILE_CONFIG_SECTION, INI_FILE_LAST_TAB_KEY);
			pTabCtrl->SetCurSel(m_SelectedTab);

			pList = (FFXiItemList*)GetDlgItem(IDC_INVENTORY_LIST);

			m_pItemIconList = new CImageList();
			m_pItemIconList->Create(LIST_ICON_SIZE, LIST_ICON_SIZE, ILC_COLOR32 | ILC_MASK, 0, 1);
			pList->SetImageList(m_pItemIconList, LVSIL_SMALL);
			m_CompactList = (m_pIni->GetLongValue(INI_FILE_CONFIG_SECTION, INI_FILE_COMPACT_LIST_KEY, 0) != 0);
			pList->SetCompactList(m_CompactList);
			SetCompactListMenu(m_CompactList);

			// enumerate the columns
			if (pList && m_pIni->GetAllKeys(INI_FILE_COLUMNS_SECTION, Keys))
			{
				Keys.sort(CSimpleIni::Entry::LoadOrder());

				for (iKey = Keys.rbegin(); iKey != Keys.rend(); ++iKey)
				{
					pKey = iKey->pItem;

					Width = m_pIni->GetLongValue(INI_FILE_COLUMNS_SECTION, pKey);

					if (pValue != NULL)
						pList->InsertColumn(0, pKey, LVCFMT_LEFT, Width);
				}

				pList->InsertColumn(INVENTORY_LIST_COL_LOCATION, _T("Location"), LVCFMT_LEFT, 0);

				pList->BringWindowToTop();
			}

			UpdateStatus();
		}

		return TRUE;
	}
	else
		AfxMessageBox(_T("Failed to load the configuration file."), MB_ICONSTOP);

	return FALSE;
}

void CLootBoxDlg::LoadGlobalMap(bool Update)
{
	int FileCount, CharCount, ProgressBarStep;
	ItemLocationInfo LocationInfo;
	InventoryMap *pInvMap;
	LPCTSTR pChar, pFile;
	ItemArray *pItemMap;
	CString InvFile;

	FileCount = (int)m_InventoryFiles.GetCount();
	CharCount = (int)m_CharacterIDs.GetCount();
	ProgressBarStep = CharCount * FileCount;

	// Init progress bar
	m_ProgressDlg.Create(IDD_PROGRESS, this);
	m_ProgressDlg.m_Progress.SetRange(0, ProgressBarStep);
	m_ProgressDlg.m_Progress.SetStep(1);

	for (int CharIndex = 0; CharIndex < CharCount; CharIndex++)
	{
		pInvMap = NULL;

		pChar = m_CharacterNames.GetAt(CharIndex);
		m_GlobalMap.Lookup(CharIndex, pInvMap);

		if (pInvMap == NULL)
		{
			pInvMap = new InventoryMap;
			// add the ID to the global map
			m_GlobalMap.SetAt(CharIndex, pInvMap);
		}

		// add the inventory files for the current character
		for (int FileIndex = 0; FileIndex < FileCount; FileIndex++)
		{
			pItemMap = NULL;

			pFile = m_InventoryNames.GetAt(FileIndex);
			pInvMap->Lookup(FileIndex, pItemMap);

			if (pItemMap == NULL || Update)
			{
				if (pItemMap == NULL)
					pItemMap = new ItemArray();

				pChar = m_CharacterIDs.GetAt(CharIndex);
				pFile = m_InventoryFiles.GetAt(FileIndex);
				InvFile.Format(_T("%s\\USER\\%s\\%s"), m_FFXiInstallPath, pChar, pFile);

				pChar = m_CharacterNames.GetAt(CharIndex);
				pFile = m_InventoryNames.GetAt(FileIndex);
				LocationInfo.Location.Format(_T("%s: %s"), pChar, pFile);
				LocationInfo.Character = CharIndex;
				LocationInfo.InvTab = FileIndex;
				LocationInfo.ListIndex = 0;
				LocationInfo.ImageIndex = 0;

				m_pHelper->ParseInventoryFile(InvFile, LocationInfo, pItemMap, m_Language, Update);

				pInvMap->SetAt(FileIndex, pItemMap);
			}

			m_ProgressDlg.m_Progress.StepIt();
		}
	}

	m_ProgressDlg.DestroyWindow();
}

BOOL CLootBoxDlg::DefaultConfig()
{
	const CSimpleIni::TKeyVal *pSectionData;
	const TBYTE *pValue;
	LONG Value = 0;

	// check if the configuration file doesn't exist
	if (GetFileAttributes(INI_FILE_FILENAME) == MAXDWORD)
	{
		// create an empty file if it doesn't
		FILE *IniFile;

		_tfopen_s(&IniFile, INI_FILE_FILENAME, _T("wb"));

		if (IniFile != NULL)
			fclose(IniFile);
		else
			return FALSE;
	}

	if (m_pIni->LoadFile(INI_FILE_FILENAME) >= 0)
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// check if the Export section exits
		pSectionData = m_pIni->GetSection(INI_FILE_EXPORT_SECTION);
		// create it if it doesn't
		if (pSectionData == NULL)
			m_pIni->SetValue(INI_FILE_EXPORT_SECTION, NULL, NULL);

		// check if the Name key exits
		pValue = m_pIni->GetValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_NAME_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetLongValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_NAME_KEY, 1L);

		// check if the Attribute key exits
		pValue = m_pIni->GetValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_ATTR_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetLongValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_ATTR_KEY, 1L);

		// check if the Description key exits
		pValue = m_pIni->GetValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_DESC_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetLongValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_DESC_KEY, 1L);

		// check if the Type key exits
		pValue = m_pIni->GetValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_TYPE_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetLongValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_TYPE_KEY, 1L);

		// check if the Races key exits
		pValue = m_pIni->GetValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_RACES_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetLongValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_RACES_KEY, 1L);

		// check if the Level key exits
		pValue = m_pIni->GetValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_LEVEL_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetLongValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_LEVEL_KEY, 1L);

		// check if the Jobs key exits
		pValue = m_pIni->GetValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_JOBS_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetLongValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_JOBS_KEY, 1L);

		// check if the Remarks key exits
		pValue = m_pIni->GetValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_REMARKS_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetLongValue(INI_FILE_EXPORT_SECTION, INI_FILE_COL_REMARKS_KEY, 1L);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// check if the Characters section exits
		pSectionData = m_pIni->GetSection(INI_FILE_CHARACTERS_SECTION);
		// create it if it doesn't
		if (pSectionData == NULL)
			m_pIni->SetValue(INI_FILE_CHARACTERS_SECTION, NULL, NULL);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// check if the Inventory section exits
		pSectionData = m_pIni->GetSection(INI_FILE_INVENTORY_SECTION);
		// create it if it doesn't
		if (pSectionData == NULL)
			m_pIni->SetValue(INI_FILE_INVENTORY_SECTION, NULL, NULL);

		// check if the Inventory key exits
		pValue = m_pIni->GetValue(INI_FILE_INVENTORY_SECTION, INI_FILE_INVENTORY_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetValue(INI_FILE_INVENTORY_SECTION, INI_FILE_INVENTORY_KEY, INI_FILE_INVENTORY_VALUE);

		// check if the Mog Safe key exits
		pValue = m_pIni->GetValue(INI_FILE_INVENTORY_SECTION, INI_FILE_MOG_SAFE_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetValue(INI_FILE_INVENTORY_SECTION, INI_FILE_MOG_SAFE_KEY, INI_FILE_MOG_SAFE_VALUE);

		// check if the Storage key exits
		pValue = m_pIni->GetValue(INI_FILE_INVENTORY_SECTION, INI_FILE_STORAGE_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetValue(INI_FILE_INVENTORY_SECTION, INI_FILE_STORAGE_KEY, INI_FILE_STORAGE_VALUE);

		// check if the Mog Locker key exits
		pValue = m_pIni->GetValue(INI_FILE_INVENTORY_SECTION, INI_FILE_MOG_LOCKER_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetValue(INI_FILE_INVENTORY_SECTION, INI_FILE_MOG_LOCKER_KEY, INI_FILE_MOG_LOCKER_VALUE);

		// check if the Mog Satchel key exits
		pValue = m_pIni->GetValue(INI_FILE_INVENTORY_SECTION, INI_FILE_MOG_SATCHEL_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetValue(INI_FILE_INVENTORY_SECTION, INI_FILE_MOG_SATCHEL_KEY, INI_FILE_MOG_SATCHEL_VALUE);

		// check if the Mog Sack key exits
		pValue = m_pIni->GetValue(INI_FILE_INVENTORY_SECTION, INI_FILE_MOG_SACK_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetValue(INI_FILE_INVENTORY_SECTION, INI_FILE_MOG_SACK_KEY, INI_FILE_MOG_SACK_VALUE);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// check if the Columns section exits
		pSectionData = m_pIni->GetSection(INI_FILE_COLUMNS_SECTION);
		// create it if it doesn't
		if (pSectionData == NULL)
			m_pIni->SetValue(INI_FILE_COLUMNS_SECTION, NULL, NULL);

		// check if the Name key exits
		pValue = m_pIni->GetValue(INI_FILE_COLUMNS_SECTION, INI_FILE_COL_NAME_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetValue(INI_FILE_COLUMNS_SECTION, INI_FILE_COL_NAME_KEY, INI_FILE_COL_NAME_VALUE);

		// check if the Attr key exits
		pValue = m_pIni->GetValue(INI_FILE_COLUMNS_SECTION, INI_FILE_COL_ATTR_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetValue(INI_FILE_COLUMNS_SECTION, INI_FILE_COL_ATTR_KEY, INI_FILE_COL_ATTR_VALUE);

		// check if the Description key exits
		pValue = m_pIni->GetValue(INI_FILE_COLUMNS_SECTION, INI_FILE_COL_DESC_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetValue(INI_FILE_COLUMNS_SECTION, INI_FILE_COL_DESC_KEY, INI_FILE_COL_DESC_VALUE);

		// check if the Type key exits
		pValue = m_pIni->GetValue(INI_FILE_COLUMNS_SECTION, INI_FILE_COL_TYPE_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetValue(INI_FILE_COLUMNS_SECTION, INI_FILE_COL_TYPE_KEY, INI_FILE_COL_TYPE_VALUE);

		// check if the Races key exits
		pValue = m_pIni->GetValue(INI_FILE_COLUMNS_SECTION, INI_FILE_COL_RACES_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetValue(INI_FILE_COLUMNS_SECTION, INI_FILE_COL_RACES_KEY, INI_FILE_COL_RACES_VALUE);

		// check if the Level key exits
		pValue = m_pIni->GetValue(INI_FILE_COLUMNS_SECTION, INI_FILE_COL_LEVEL_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetValue(INI_FILE_COLUMNS_SECTION, INI_FILE_COL_LEVEL_KEY, INI_FILE_COL_LEVEL_VALUE);

		// check if the Jobs key exits
		pValue = m_pIni->GetValue(INI_FILE_COLUMNS_SECTION, INI_FILE_COL_JOBS_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetValue(INI_FILE_COLUMNS_SECTION, INI_FILE_COL_JOBS_KEY, INI_FILE_COL_JOBS_VALUE);

		// check if the Remarks key exits
		pValue = m_pIni->GetValue(INI_FILE_COLUMNS_SECTION, INI_FILE_COL_REMARKS_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetValue(INI_FILE_COLUMNS_SECTION, INI_FILE_COL_REMARKS_KEY, INI_FILE_COL_REMARKS_VALUE);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// check if the Config section exits
		pSectionData = m_pIni->GetSection(INI_FILE_CONFIG_SECTION);
		// create it if it doesn't
		if (pSectionData == NULL)
			m_pIni->SetValue(INI_FILE_CONFIG_SECTION, NULL, NULL);

		// check if the Language key exits
		m_Region = m_pIni->GetLongValue(INI_FILE_CONFIG_SECTION, INI_FILE_GAME_REGION_KEY, -1);
		// create it if it doesn't
		if (m_Region == -1)
		{
			int RegionCount, Regions = 0;

			m_Region = INI_FILE_GAME_REGION_VALUE;
			RegionCount = m_pHelper->DetectGameRegion(Regions);

			if (RegionCount > 1)
			{
				RegionSelect RegionSelectDlg(Regions);

				if (RegionSelectDlg.DoModal() == IDOK)
					m_Region = RegionSelectDlg.GetSelectedRegion();
			}

			m_pIni->SetLongValue(INI_FILE_CONFIG_SECTION, INI_FILE_GAME_REGION_KEY, m_Region, INI_FILE_GAME_REGION_COMMENT);
		}

		// check if the Region key exits
		m_Language = m_pIni->GetLongValue(INI_FILE_CONFIG_SECTION, INI_FILE_LANGUAGE_KEY, -1);
		// create it if it doesn't
		if (m_Language == -1)
		{
			m_pIni->SetLongValue(INI_FILE_CONFIG_SECTION, INI_FILE_LANGUAGE_KEY, INI_FILE_LANGUAGE_VALUE, INI_FILE_LANGUAGE_COMMENT);
			m_Language = INI_FILE_LANGUAGE_VALUE;
		}

		// check if the Width key exits
		pValue = m_pIni->GetValue(INI_FILE_CONFIG_SECTION, INI_FILE_WINDOW_WIDTH_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetLongValue(INI_FILE_CONFIG_SECTION, INI_FILE_WINDOW_WIDTH_KEY, INI_FILE_WINDOW_WIDTH_VALUE);

		// check if the Height key exits
		pValue = m_pIni->GetValue(INI_FILE_CONFIG_SECTION, INI_FILE_WINDOW_HEIGHT_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetLongValue(INI_FILE_CONFIG_SECTION, INI_FILE_WINDOW_HEIGHT_KEY, INI_FILE_WINDOW_HEIGHT_VALUE);

		// check if the SelectedChar key exits
		pValue = m_pIni->GetValue(INI_FILE_CONFIG_SECTION, INI_FILE_LAST_CHARACTER_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetLongValue(INI_FILE_CONFIG_SECTION, INI_FILE_LAST_CHARACTER_KEY, INI_FILE_LAST_CHARACTER_VALUE);

		// check if the SelectedTab key exits
		pValue = m_pIni->GetValue(INI_FILE_CONFIG_SECTION, INI_FILE_LAST_TAB_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetLongValue(INI_FILE_CONFIG_SECTION, INI_FILE_LAST_TAB_KEY, INI_FILE_LAST_TAB_VALUE);
		// check if the SelectedTab key exits
		pValue = m_pIni->GetValue(INI_FILE_CONFIG_SECTION, INI_FILE_COMPACT_LIST_KEY);
		// create it if it doesn't
		if (pValue == NULL)
			m_pIni->SetLongValue(INI_FILE_CONFIG_SECTION, INI_FILE_COMPACT_LIST_KEY, INI_FILE_COMPACT_LIST_VALUE);

		m_pIni->SaveFile(_T("loot.ini"));

		return TRUE;
	}

	return FALSE;
}

BOOL CLootBoxDlg::InitLayout()
{
	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_INVENTORY_TABS);
	CPane TopLeftPane, TopRightPane, BottomPane, TopPane;

	CreateRoot(VERTICAL, 2, 8);

	TopLeftPane = new Pane(this, VERTICAL);
	TopLeftPane->addItem(IDC_CHARACTERS_LABEL, NORESIZE | ALIGN_LEFT);
	TopLeftPane->addItem(IDC_CHAR_LIST, ABSOLUTE_HORZ);

	TopRightPane = new PaneTab(pTabCtrl, this, VERTICAL);
	TopRightPane->addItem(IDC_INVENTORY_LIST);

	TopPane = new Pane(this, HORIZONTAL);

	TopPane->addPane(TopLeftPane, NORESIZE | ALIGN_LEFT, 0);
	TopPane->addPane(TopRightPane, GREEDY, 0);

	BottomPane = new Pane(this, HORIZONTAL);
	BottomPane->addItem(IDC_STATUS_BAR, ABSOLUTE_VERT | ALIGN_LEFT);
	BottomPane->addItem(IDC_ITEM_COUNT, GREEDY | ALIGN_RIGHT);

	m_RootPane->addPane(TopPane, GREEDY, 0);
	m_RootPane->addPane(BottomPane, ABSOLUTE_VERT, 0);

	UpdateLayout();

	return TRUE;
}

// prevent the dialog from closing when the user presses ESC
BOOL CLootBoxDlg::PreTranslateMessage(MSG* pMsg)
{
	if (m_hAcceleratorTable != NULL && TranslateAccelerator(m_hWnd, m_hAcceleratorTable, pMsg))
	{
		return TRUE;
	}

	switch (pMsg->message)
	{
		case WM_KEYDOWN:
		{
			switch(pMsg->wParam)
			{
				case VK_ESCAPE:
				case VK_CANCEL:
					::DispatchMessage(pMsg);

					return TRUE;
				break;
			}
		}
		break;

		case WM_MBUTTONDOWN:
		{
			CTabCtrl *pInvTab = (CTabCtrl*)GetDlgItem(IDC_INVENTORY_TABS);

			if (pInvTab)
			{
				CPoint MousePos = pMsg->pt;
				TCHITTESTINFO HitInfo;
				int TabIndex = -1;

				SecureZeroMemory(&HitInfo, sizeof(HitInfo));

				pInvTab->ScreenToClient(&MousePos);

				HitInfo.pt = MousePos;
				TabIndex = pInvTab->HitTest(&HitInfo);

				if (TabIndex >= m_InventoryFiles.GetCount())
				{
					CTabCtrl *pTabList = (CTabCtrl*)GetDlgItem(IDC_INVENTORY_TABS);
					SearchData *pData;

					pInvTab->DeleteItem(TabIndex);

					m_SearchTabs.Lookup(TabIndex, pData);

					if (pData)
					{
						if (pData->pParams != NULL)
						{
							if (pData->pParams->pSearchTerm != NULL)
							{
								delete pData->pParams->pSearchTerm;
								pData->pParams->pSearchTerm = NULL;
							}

							delete pData->pParams;
							pData->pParams = NULL;
						}

						delete pData;
						pData = NULL;
					}

					m_SearchTabs.RemoveKey(TabIndex);

					m_SelectedTab = TabIndex - 1;
					pTabList->SetCurSel(m_SelectedTab);

					UpdateStatus();
				}
			}
		}
		break;
	}

	return ETSLayoutDialog::PreTranslateMessage(pMsg);
}

// cleanup on dialog destruction
void CLootBoxDlg::OnDestroy()
{
	FFXiItemList* pInvList = (FFXiItemList*)GetDlgItem(IDC_INVENTORY_LIST);
	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_INVENTORY_TABS);
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_CHAR_LIST);
	LONG Width, Height, SelectedChar, SelectedTab;
	int CharCount, FileCount, TabID;
	SearchData *pData = NULL;
	POSITION MapPos;
	RECT WindowRect;
	
	GetWindowRect(&WindowRect);
	Width = WindowRect.right - WindowRect.left;
	Height = WindowRect.bottom - WindowRect.top;
	SelectedChar = pList->GetNextItem(-1, LVNI_SELECTED);
	SelectedTab = pTabCtrl->GetCurSel();
	CharCount = (int)m_CharacterIDs.GetCount();
	FileCount = (int)m_InventoryFiles.GetCount();

	if (SelectedChar < 0)
		SelectedChar = 0;
	if (SelectedChar >= CharCount)
		SelectedTab = CharCount - 1;

	if (SelectedTab < 0)
		SelectedTab = 0;
	if (SelectedTab >= FileCount && FileCount > 0)
		SelectedTab = FileCount - 1;
	

	m_pIni->SetLongValue(INI_FILE_CONFIG_SECTION, INI_FILE_WINDOW_WIDTH_KEY, Width);
	m_pIni->SetLongValue(INI_FILE_CONFIG_SECTION, INI_FILE_WINDOW_HEIGHT_KEY, Height);
	m_pIni->SetLongValue(INI_FILE_CONFIG_SECTION, INI_FILE_LAST_TAB_KEY, SelectedTab);
	m_pIni->SetLongValue(INI_FILE_CONFIG_SECTION, INI_FILE_LAST_CHARACTER_KEY, SelectedChar);

	pInvList->DeleteAllItems();
	DeleteGlobalMap();

	MapPos = m_SearchTabs.GetStartPosition();

	while (MapPos != NULL)
	{
		m_SearchTabs.GetNextAssoc(MapPos, TabID, pData);

		if (pData != NULL)
		{
			if (pData->pParams != NULL)
			{
				if (pData->pParams->pSearchTerm != NULL)
				{
					delete pData->pParams->pSearchTerm;
					pData->pParams->pSearchTerm = NULL;
				}

				delete pData->pParams;
				pData->pParams = NULL;
			}			

			delete pData;
			pData = NULL;
		}
	}

	if (m_pFileData != NULL)
	{
		free(m_pFileData);
		m_pFileData = NULL;
	}

	if (m_pIconList != NULL)
	{
		delete(m_pIconList);
		m_pIconList = NULL;
	}

	if (m_pItemIconList != NULL)
	{
		delete(m_pItemIconList);
		m_pItemIconList = NULL;
	}

	if (m_pMainMenu != NULL)
	{
		delete m_pMainMenu;
		m_pMainMenu = NULL;
	}

	if (m_pPopMenu != NULL)
	{
		delete m_pPopMenu;
		m_pPopMenu = NULL;
	}

	if (m_pIni != NULL)
	{
		m_pIni->SaveFile(_T("loot.ini"));

		delete(m_pIni);
		m_pIni = NULL;
	}

	if (m_pSearchDlg != NULL)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}

	if (m_pHelper != NULL)
	{
		delete(m_pHelper);
		m_pHelper = NULL;
	}

	ETSLayoutDialog::OnDestroy();
}

int CLootBoxDlg::GlobalMapCount()
{
	int CharID, FileID, Result = 0;
	POSITION GlobalPos, InvPos;
	InventoryMap *pInvMap;
	ItemArray *pItemArr;

	GlobalPos = m_GlobalMap.GetStartPosition();
	pItemArr = NULL;
	pInvMap = NULL;

	while (GlobalPos != NULL)
	{
		m_GlobalMap.GetNextAssoc(GlobalPos, CharID, pInvMap);

		if (pInvMap != NULL)
		{
			InvPos = pInvMap->GetStartPosition();

			while (InvPos != NULL)
			{
				pInvMap->GetNextAssoc(InvPos, FileID, pItemArr);

				if (pItemArr != NULL)
					Result += (int)pItemArr->GetCount();
			}
		}
	}

	return Result;
}

void CLootBoxDlg::DeleteGlobalMap()
{
	POSITION GlobalPos, InvPos, ItemPos;
	int CharID, FileID, ItemID;
	CString InvKey, CharKey;
	InventoryMap *pInvMap;
	InventoryItem *pItem;
	ItemArray *pItemArr;

	GlobalPos = m_GlobalMap.GetStartPosition();
	pItemArr = NULL;
	pInvMap = NULL;

	while (GlobalPos != NULL)
	{
		m_GlobalMap.GetNextAssoc(GlobalPos, CharID, pInvMap);

		if (pInvMap != NULL)
		{
			InvPos = pInvMap->GetStartPosition();

			while (InvPos != NULL)
			{
				pInvMap->GetNextAssoc(InvPos, FileID, pItemArr);

				if (pItemArr != NULL)
				{
					ItemPos = pItemArr->GetStartPosition();

					while(ItemPos != NULL)
					{
						pItemArr->GetNextAssoc(ItemPos, ItemID, pItem);

						if (pItem != NULL)
						{
							if (pItem->hBitmap != NULL)
								DeleteObject(pItem->hBitmap);

							if (pItem != NULL)
							{
								delete(pItem);
								pItem = NULL;
							}
						}
					}

					delete pItemArr;
					pItemArr = NULL;
				}
			}

			delete pInvMap;
			pInvMap = NULL;
		}
	}

	m_GlobalMap.RemoveAll();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLootBoxDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		ETSLayoutDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLootBoxDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// rename a list item on left click
void CLootBoxDlg::OnListItemClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	CListCtrl *pList = (CListCtrl*)GetDlgItem(IDC_CHAR_LIST);
	LPNMHEADER phdr = (LPNMHEADER)pNMHDR;

	// edit the item
	pList->EditLabel(phdr->iItem);

	*pResult = 0;
}

//void CLootBoxDlg::OnInvListItemClick(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	FFXiItemList *pInvList = (FFXiItemList*)GetDlgItem(IDC_INVENTORY_LIST);
//	LPNMHEADER phdr = (LPNMHEADER)pNMHDR;
//
//	TRACE(_T("Item data %0x\n"), pInvList->GetItemData(phdr->iItem));
//
//	*pResult = 0;
//}

// process key presses on list items
void CLootBoxDlg::OnKeyDownListItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	CListCtrl *pList = (CListCtrl*)GetDlgItem(IDC_CHAR_LIST);
	LPNMLVKEYDOWN pLVKeyDow = (LPNMLVKEYDOWN)pNMHDR;

	// rename the current item when F2 is pressed
	if (pLVKeyDow->wVKey == VK_F2)
	{
		// retrieve the selected item
		int ItemIndex = pList->GetSelectionMark();
		// edit the item
		pList->EditLabel(ItemIndex);
	}

	*pResult = 0;
}


// commit the changes when the edit mode ends
void CLootBoxDlg::OnEndItemEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
	CListCtrl *pList = (CListCtrl*)GetDlgItem(IDC_CHAR_LIST);
	NMLVDISPINFO *pDispInfo = (NMLVDISPINFO*)pNMHDR;
	LPTSTR ItemData = (LPTSTR)pList->GetItemData(pDispInfo->item.iItem);
	
	// if the text is NULL, the user canceled: do nothing
	if (pDispInfo->item.pszText != NULL)
	{
		// if the text is empty
		if (_tcslen(pDispInfo->item.pszText) == 0)
		{
			// write an empty value in the INI file
			m_pIni->SetValue(INI_FILE_CHARACTERS_SECTION, ItemData, _T(""));
			// reset the item to the default value
			pList->SetItem(pDispInfo->item.iItem, pDispInfo->item.iSubItem, LVIF_TEXT, ItemData, 0, 0, 0, 0);
		}
		else
		{
			// update the INI file with the new value
			m_pIni->SetValue(INI_FILE_CHARACTERS_SECTION, ItemData, pDispInfo->item.pszText);
			// update the item
			pList->SetItem(pDispInfo->item.iItem, pDispInfo->item.iSubItem, LVIF_TEXT, pDispInfo->item.pszText, 0, 0, 0, 0);
		}
	}	

	*pResult = 0;
}
void CLootBoxDlg::OnInventoryColumnResize(NMHDR *pNMHDR, LRESULT *pResult)
{
	FFXiItemList *pList = (FFXiItemList*)GetDlgItem(IDC_INVENTORY_LIST);
	NMHEADER* phdr = (NMHEADER*)pNMHDR;
	TCHAR pHeader[16];

	// if this item mask indicates a change in width, then cxy holds that changed value
	if (phdr->pitem->mask & HDI_WIDTH)
	{
		LVCOLUMN Column;

		Column.mask = LVCF_TEXT;
		Column.cchTextMax = 16;
		Column.pszText = pHeader;
		pList->GetColumn(phdr->iItem, &Column);

		// update the INI file with the new value
		m_pIni->SetLongValue(INI_FILE_COLUMNS_SECTION, Column.pszText, phdr->pitem->cxy);
	}

	*pResult = 0;
}

void CLootBoxDlg::OnInventoryColumnAutosize(NMHDR *pNMHDR, LRESULT *pResult)
{
	FFXiItemList *pList = (FFXiItemList*)GetDlgItem(IDC_INVENTORY_LIST);
	NMHEADER* phdr = (NMHEADER*)pNMHDR;
	TCHAR pHeader[16];

	if (phdr->iItem)
	{
		LVCOLUMN Column;

		Column.mask = LVCF_WIDTH | LVCF_TEXT;
		Column.cchTextMax = 16;
		Column.pszText = pHeader;
		pList->GetColumn(phdr->iItem, &Column);
		// update the INI file with the new value
		m_pIni->SetLongValue(INI_FILE_COLUMNS_SECTION, Column.pszText, Column.cx);
	}

	*pResult = 0;
}

void CLootBoxDlg::OnInventoryTabsChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	FFXiItemList *pList = (FFXiItemList*)GetDlgItem(IDC_INVENTORY_LIST);
	CTabCtrl *pTabList = (CTabCtrl*)GetDlgItem(IDC_INVENTORY_TABS);

	if (pTabList)
	{
		m_SelectedTab = pTabList->GetCurSel();

		if (IsInventoryTab())
			pList->SetColumnWidth(INVENTORY_LIST_COL_LOCATION, 0);
		else
			pList->SetColumnWidth(INVENTORY_LIST_COL_LOCATION, 150);

		UpdateStatus();
	}

	*pResult = 0;
}

void CLootBoxDlg::OnItemDoubleClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	FFXiItemList *pList = (FFXiItemList*)GetDlgItem(IDC_INVENTORY_LIST);
	CTabCtrl *pTabList = (CTabCtrl*)GetDlgItem(IDC_INVENTORY_TABS);
	CListCtrl *pCharList = (CListCtrl*)GetDlgItem(IDC_CHAR_LIST);
	NMHEADER* phdr = (NMHEADER*)pNMHDR;
	InventoryItem *pItem;
	
	if (pList && pTabList && pCharList && IsInventoryTab() == false)
	{
		pItem = (InventoryItem*)pList->GetItemData(phdr->iItem);

		if (pItem)
		{
			int EnsureIndex, ItemIndex = 0;

			m_SelectedTab = pItem->LocationInfo.InvTab;
			m_SelectedChar = pItem->LocationInfo.Character;
			ItemIndex = pItem->LocationInfo.ListIndex;

			if (m_SelectedChar != m_CharacterIDs.GetCount() - 1)
				EnsureIndex = m_SelectedChar + 1;
			else
				EnsureIndex = m_SelectedChar;

			pList->SetColumnWidth(INVENTORY_LIST_COL_LOCATION, 0);

			pCharList->SetItemState(m_SelectedChar, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			pCharList->EnsureVisible(EnsureIndex, FALSE);
			pTabList->SetCurSel(m_SelectedTab);

			UpdateStatus();

			if (ItemIndex != pList->GetItemCount() - 1)
				EnsureIndex = ItemIndex + 1;
			else
				EnsureIndex = ItemIndex;

			pList->SetItemState(ItemIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			pList->EnsureVisible(EnsureIndex, FALSE);
		}
	}

	*pResult = 0;
}

void CLootBoxDlg::OnRightClickInventoryTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMenu *pMenu = m_pPopMenu->GetSubMenu(0);
	CPoint MousePos;

	GetCursorPos(&MousePos);

	if (IsInventoryTab() == false)
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, MousePos.x,	MousePos.y, this);

	*pResult = 0;
}

void CLootBoxDlg::OnCharactersListChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (m_InitDone && (pNMListView->uChanged & LVIF_STATE) && (pNMListView->uNewState & LVNI_SELECTED))
	{
		m_SelectedChar = pNMListView->iItem;

		if (IsInventoryTab())
			UpdateStatus();
	}

	*pResult = 0;
}

void CLootBoxDlg::UpdateStatus()
{
	CStatic *pStatus = (CStatic*)GetDlgItem(IDC_STATUS_BAR);

	if (pStatus && IsValidChar())
	{
		ItemArray *pItemList = NULL;

		if (IsInventoryTab())
		{
			m_CurrentFile.Format(_T("%s\\USER\\%s\\%s"), m_FFXiInstallPath,
								 m_CharacterIDs.GetAt(m_SelectedChar),
								 m_InventoryFiles.GetAt(m_SelectedTab));
		}
		else
		{
			m_CurrentFile = _T("");
		}

		pStatus->SetWindowText(m_CurrentFile);

		pItemList = GetItemMap(m_SelectedChar, m_SelectedTab);

		if (pItemList == NULL)
		{
			ItemLocationInfo Location;

			pItemList = new ItemArray;

			Location.ImageIndex = Location.ListIndex = 0;
			Location.Location.Format(_T("%s: %s"), m_CharacterNames.GetAt(m_SelectedChar),
												   m_InventoryNames.GetAt(m_SelectedTab));
			Location.Character = m_SelectedChar;
			Location.InvTab = m_SelectedTab;

			m_pHelper->ParseInventoryFile(m_CurrentFile, Location, pItemList, m_Language);

			SetItemMapAt(m_SelectedChar, m_SelectedTab, pItemList);
		}

		RefreshList(pItemList);
	}
}

void CLootBoxDlg::SetItemMapAt(int SelectedCharIndex, int SelectedTabIndex, ItemArray *pItemList)
{
	InventoryMap *pInvMap = NULL;

	if (IsInventoryTab(SelectedTabIndex) && IsValidChar(SelectedCharIndex))
	{
		m_GlobalMap.Lookup(SelectedCharIndex, pInvMap);

		if (pInvMap == NULL)
		{
			pInvMap = new InventoryMap;
			m_GlobalMap.SetAt(SelectedCharIndex, pInvMap);
		}

		pInvMap->SetAt(SelectedTabIndex, pItemList);
	}
}

ItemArray* CLootBoxDlg::GetItemMap(int SelectedCharIndex, int SelectedTabIndex)
{
	InventoryMap *pInvMap = NULL;
	ItemArray *pItemList = NULL;

	if (IsInventoryTab(SelectedTabIndex) && IsValidChar(SelectedCharIndex))
	{
		m_GlobalMap.Lookup(SelectedCharIndex, pInvMap);

		if (pInvMap != NULL)
		{
			pInvMap->Lookup(SelectedTabIndex, pItemList);

			return pItemList;
		}
	}
	else
	{
		SearchData *pData = NULL;

		m_SearchTabs.Lookup(m_SelectedTab, pData);

		if (pData)
		{
			if (pData->Done == false)
				GetSearchResults(pData);

			return &pData->Items;
		}
	}

	return NULL;
}

BOOL CLootBoxDlg::RefreshList(const ItemArray *pItemList)
{
	FFXiItemList* pList = (FFXiItemList*)GetDlgItem(IDC_INVENTORY_LIST);
	int ImageIndex = 0, ItemIndex = 0, ImageCount;
	CString ItemCountStr;
	CBitmap Bitmap;
	CFile InvFile;
	int ItemID;

	if (pList)
	{
		pList->DeleteAllItems();
		pList->BlockRedraw();

		ImageCount = m_pItemIconList->GetImageCount();
		m_ItemsCount = 0;

		if (pItemList != NULL)
		{
			InventoryItem *pItem;
			POSITION ItemPos;
	
			ItemPos = pItemList->GetStartPosition();

			while (ItemPos != NULL)
			{
				pItemList->GetNextAssoc(ItemPos, ItemID, pItem);

				if (pItem != NULL)
				{
					if (pItem->hBitmap == NULL) 
						pItem->hBitmap = GetItemIcon(&pItem->IconInfo, GetDC(), LIST_ICON_SIZE, LIST_ICON_SIZE);

					if (pItem->hBitmap != NULL && Bitmap.Attach(pItem->hBitmap))
					{
						if (ItemIndex < ImageCount)
						{
							m_pItemIconList->Replace(ItemIndex, &Bitmap, RGB(0,0,0));
							ImageIndex = ItemIndex;
						}
						else
							ImageIndex = m_pItemIconList->Add(&Bitmap, RGB(0,0,0));

						Bitmap.Detach();
					}

					pItem->LocationInfo.ImageIndex = ImageIndex;
					m_ItemsCount++;

					if (m_CompactList && pItem->RefCount > 1)
					{
						CString ItemText;

						pList->AddItem(pItem, ItemIndex);
						// update the name of the item
						ItemText.Format(_T("%s (%d)"), pItem->ItemName, pItem->RefCount);
						pList->UpdateItemText(ItemText, ItemIndex - 1, 0);
						pItem->ItemToolTip.Format(_T("%d %s"), pItem->RefCount, pItem->LogName2);
					}
					else
					{
						pItem->ItemToolTip = pItem->LogName;

						for (int i = 0; i < pItem->RefCount; i++)
							pList->AddItem(pItem, ItemIndex);
					}
				}

				++ItemIndex;
			}
		}

		for (int i = m_ItemsCount; i < m_pItemIconList->GetImageCount();)
			m_pItemIconList->Remove(i);

		ItemCountStr.Format(_T("%d item(s)"), m_ItemsCount);
		pList->BlockRedraw(false);

		GetDlgItem(IDC_ITEM_COUNT)->SetWindowText(ItemCountStr);

		return TRUE;
	}

	return FALSE;
}

void CLootBoxDlg::SetLanguageMenu(int Language, bool Check)
{
	MENUITEMINFO MenuInfo;
	CMenu *pLangMenu;

	pLangMenu = m_pMainMenu->GetSubMenu(1)->GetSubMenu(0);

	if (pLangMenu)
	{
		SecureZeroMemory(&MenuInfo, sizeof(MENUITEMINFO));
		MenuInfo.fMask = MIIM_STATE;
		MenuInfo.fState = Check ? MFS_CHECKED : MFS_UNCHECKED;
		MenuInfo.cbSize = sizeof(MENUITEMINFO);

		pLangMenu->SetMenuItemInfo(Language - 1, &MenuInfo, TRUE);
	}
}

void CLootBoxDlg::SetCompactListMenu(LONG CompactList)
{
	MENUITEMINFO MenuInfo;
	CMenu *pCompactListMenu;

	pCompactListMenu = m_pMainMenu->GetSubMenu(2);

	if (pCompactListMenu)
	{
		SecureZeroMemory(&MenuInfo, sizeof(MENUITEMINFO));
		MenuInfo.fMask = MIIM_STATE;
		MenuInfo.fState = (CompactList != 0) ? MFS_CHECKED : MFS_UNCHECKED;
		MenuInfo.cbSize = sizeof(MENUITEMINFO);

		pCompactListMenu->SetMenuItemInfo(0, &MenuInfo, TRUE);
	}
}

afx_msg void CLootBoxDlg::OnSearch()
{
	if (m_pSearchDlg == NULL)
		m_pSearchDlg = new SearchDialog(m_pHelper, this);

	if (m_pSearchDlg->m_hWnd == NULL)
		m_pSearchDlg->Create(IDD_SEARCH_DIALOG, this);
	else
		m_pSearchDlg->ShowWindow(SW_SHOW);

	m_pSearchDlg->CenterWindow();
}

afx_msg void CLootBoxDlg::OnExport()
{
	ExportDialog Dialog(m_pHelper, m_CharacterNames, m_pIni, this);
	
	if (Dialog.DoModal() == IDOK)
	{
		int ExportChars = Dialog.GetExportedCharsCount();
		int ColumnCount = Dialog.GetColumnCount() + 2;
		DWORD_PTR BitMask = Dialog.GetBitMask();

		if (ColumnCount > 0 && ExportChars > 0 && BitMask != 0UL)
		{
			CFileDialog SaveDialog(FALSE, _T("*.csv"), _T("export.csv"),
								   OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_NOCHANGEDIR 
								   | OFN_PATHMUSTEXIST | OFN_ENABLESIZING,
								   _T("(*.csv) Excel 2000/XP||"), this);

			if (SaveDialog.DoModal() == IDOK)
			{
				const CArray<bool, bool> &ExportedChars = Dialog.GetExportedChars();
				int FileCount = m_InventoryFiles.GetCount();
				int CharCount = m_CharacterNames.GetCount();		

				CsvWriter<CFile> Exporter;
				InventoryMap *pInvMap;
				InventoryItem *pItem;
				ItemArray *pItemMap;
				CString Filename;
				POSITION ItemPos;
				int ItemID;

				LoadGlobalMap();

				Filename = SaveDialog.GetPathName();
				Exporter.CreateFile(Filename, ColumnCount);

				Exporter.AddColumn(_T("Character"))
						.AddColumn(_T("Location"));

				if ((BitMask & EXPORT_NAME) == EXPORT_NAME)
					Exporter.AddColumn(INI_FILE_COL_NAME_KEY);

				if ((BitMask & EXPORT_ATTR) == EXPORT_ATTR)
					Exporter.AddColumn(INI_FILE_COL_ATTR_KEY);

				if ((BitMask & EXPORT_DESC) == EXPORT_DESC)
					Exporter.AddColumn(INI_FILE_COL_DESC_KEY);

				if ((BitMask & EXPORT_TYPE) == EXPORT_TYPE)
					Exporter.AddColumn(INI_FILE_COL_TYPE_KEY);

				if ((BitMask & EXPORT_RACES) == EXPORT_RACES)
					Exporter.AddColumn(INI_FILE_COL_RACES_KEY);

				if ((BitMask & EXPORT_LEVEL) == EXPORT_LEVEL)
					Exporter.AddColumn(INI_FILE_COL_LEVEL_KEY);

				if ((BitMask & EXPORT_JOBS) == EXPORT_JOBS)
					Exporter.AddColumn(INI_FILE_COL_JOBS_KEY);

				if ((BitMask & EXPORT_RMKS) == EXPORT_RMKS)
					Exporter.AddColumn(INI_FILE_COL_REMARKS_KEY);

				for (int CharIndex = 0; CharIndex < CharCount; ++CharIndex)
				{
					if (ExportedChars[CharIndex] == false)
						continue;

					pInvMap = NULL;

					if (m_GlobalMap.Lookup(CharIndex, pInvMap) && pInvMap != NULL)
					{
						// add the inventory files for the current character
						for (int FileIndex = 0; FileIndex < FileCount; ++FileIndex)
						{
							pItemMap = NULL;

							if (pInvMap->Lookup(FileIndex, pItemMap) && pItemMap != NULL)
							{
								ItemPos = pItemMap->GetStartPosition();

								while (ItemPos != NULL)
								{
									pItemMap->GetNextAssoc(ItemPos, ItemID, pItem);

									Exporter.AddColumn(m_CharacterNames[CharIndex])
											.AddColumn(m_InventoryNames[FileIndex]);

									if ((BitMask & EXPORT_NAME) == EXPORT_NAME)
										Exporter.AddColumn(pItem->ItemName);

									if ((BitMask & EXPORT_ATTR) == EXPORT_ATTR)
										Exporter.AddColumn(pItem->Attr);

									if ((BitMask & EXPORT_DESC) == EXPORT_DESC)
										Exporter.AddColumn(pItem->ItemDescription);

									if ((BitMask & EXPORT_TYPE) == EXPORT_TYPE)
										Exporter.AddColumn(pItem->Slot);

									if ((BitMask & EXPORT_RACES) == EXPORT_RACES)
										Exporter.AddColumn(pItem->Races);

									if ((BitMask & EXPORT_LEVEL) == EXPORT_LEVEL)
										Exporter.AddColumn(pItem->Level);

									if ((BitMask & EXPORT_JOBS) == EXPORT_JOBS)
										Exporter.AddColumn(pItem->Jobs);

									if ((BitMask & EXPORT_RMKS) == EXPORT_RMKS)
										Exporter.AddColumn(pItem->Remarks);
								}
							}
						}
					}
				}

				Exporter.CloseFile();
				ShellExecute(m_hWnd, _T("open"), Filename, NULL, NULL, SW_SHOW);
			}
		}
		else
		{
			AfxMessageBox(_T("Nothing to export"));
		}
	}
}

void CLootBoxDlg::OnRefreshClose()
{
	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_INVENTORY_TABS);

	if (IsInventoryTab() == false)
	{
		pTabCtrl->DeleteItem(m_SelectedTab--);
		pTabCtrl->SetCurSel(m_SelectedTab);
	}
}

LRESULT CLootBoxDlg::OnSearchClose(WPARAM wParam, LPARAM lParam)
{
	FFXiItemList *pList = (FFXiItemList*)GetDlgItem(IDC_INVENTORY_LIST);
	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_INVENTORY_TABS);
	SearchData *pData;
	int CountTabs = 0;

	if (pTabCtrl && pList)
	{
		CString TabText;

		pData = new SearchData;
		pData->Done = false;

		LoadGlobalMap();

		pData->pParams = m_pSearchDlg->GetSearchParams();

		CountTabs = pTabCtrl->GetItemCount();

		TabText.Format(_T("Search %d"), CountTabs - m_InventoryFiles.GetCount() + 1);
		m_SelectedTab = pTabCtrl->InsertItem(CountTabs, TabText, 3);

		if (m_SelectedTab != -1)
		{
			pList->SetColumnWidth(INVENTORY_LIST_COL_LOCATION, 150);
			pTabCtrl->SetCurSel(m_SelectedTab);
			m_SearchTabs.SetAt(m_SelectedTab, pData);
		}

		UpdateStatus();
	}

	return 0L;
}

void CLootBoxDlg::GetSearchResults(SearchData *pData)
{
	FFXiItemList* pList = (FFXiItemList*)GetDlgItem(IDC_INVENTORY_LIST);
	CStatic *pLabel = (CStatic*)GetDlgItem(IDC_ITEM_COUNT);

	if (pList != NULL && pData != NULL)
	{
		if (pData->Done == false)
		{
			int CharID, FileID, ItemID, ItemCount, ListIndex = 0;
			POSITION GlobalPos, InvPos, ItemPos;			
			SearchHandler Searcher(pData);			
			InventoryMap *pInvMap;
			InventoryItem *pItem;
			ItemArray *pItemArr;

			GlobalPos = m_GlobalMap.GetStartPosition();
			pItemArr = NULL;
			pInvMap = NULL;
	
			ItemCount = GlobalMapCount();
			// Init progress bar
			m_ProgressDlg.Create(IDD_PROGRESS, this);
			m_ProgressDlg.m_Progress.SetRange(0, ItemCount);
			m_ProgressDlg.m_Progress.SetStep(1);
	
			while (GlobalPos != NULL)
			{
				m_GlobalMap.GetNextAssoc(GlobalPos, CharID, pInvMap);
	
				if (pInvMap != NULL)
				{
					InvPos = pInvMap->GetStartPosition();
	
					while (InvPos != NULL)
					{
						pInvMap->GetNextAssoc(InvPos, FileID, pItemArr);
	
						if (pItemArr != NULL)
						{
							ItemPos = pItemArr->GetStartPosition();

							while (ItemPos != NULL)
							{
								pItemArr->GetNextAssoc(ItemPos, ItemID, pItem);

								if (pItem != NULL)
									Searcher.ProcessAll(pItem);
	
								m_ProgressDlg.m_Progress.StepIt();
							}
						}
					}
				}
			}
	
			m_ProgressDlg.DestroyWindow();
			pData->Done = true;
		}
	}
}

afx_msg void CLootBoxDlg::OnOptionsChange(UINT CmdID)
{
	FFXiItemList *pInvList = (FFXiItemList*)GetDlgItem(IDC_INVENTORY_LIST);
	bool CompactListChange = false;
	ItemArray *pItemMap = NULL;
	int PrevLang = m_Language;

	pItemMap = GetItemMap(m_SelectedChar, m_SelectedTab);

	switch(CmdID)
	{
		default:
		case ID_LANGUAGE_JAPANESE:
			m_Language = FFXI_LANG_JP;
		break;
		case ID_LANGUAGE_ENGLISH:
			m_Language = FFXI_LANG_US;
		break;
		case ID_LANGUAGE_FRENCH:
			m_Language = FFXI_LANG_FR;
		break;
		case ID_LANGUAGE_GERMAN:
			m_Language = FFXI_LANG_DE;
		break;
		case ID_VIEW_COMPACTLISTING:
			CompactListChange = true;
			m_CompactList = !pInvList->IsCompact();
			pInvList->SetCompactList(m_CompactList);
		break;
	}

	if (m_Language != PrevLang)
	{
		m_pIni->SetLongValue(INI_FILE_CONFIG_SECTION, INI_FILE_LANGUAGE_KEY, m_Language);
		SetLanguageMenu(PrevLang, false);
		SetLanguageMenu(m_Language);

		LoadGlobalMap(true);
	}
	else if (CompactListChange)
	{
		m_pIni->SetLongValue(INI_FILE_CONFIG_SECTION, INI_FILE_COMPACT_LIST_KEY, m_CompactList ? 1 : 0);
		SetCompactListMenu(m_CompactList);
	}

	UpdateStatus();
}

void CLootBoxDlg::RemoveItemIcons()
{
	for (int i = 0; i < m_pItemIconList->GetImageCount(); i++)
		m_pItemIconList->Remove(i);
}

HBITMAP CLootBoxDlg::GetItemIcon(FFXiIconInfo *pIconInfo, CDC *pDC, int Width, int Height, int ItemID)
{
	if (pIconInfo != NULL)
	{
		HDC hDC = pDC->GetSafeHdc();
		PalettedBitmapInfo BmpInfo;
		HBITMAP hBitmap;
		int Index = 0;
		DWORD *pDst;

		BmpInfo = pIconInfo->ImageInfo;

		hBitmap = ::CreateDIBSection(hDC, (BITMAPINFO*)&BmpInfo, DIB_RGB_COLORS, (void**)&pDst, NULL, 0);

		if (hBitmap != NULL)
		{
			memcpy_s(pDst, 1024, &pIconInfo->ImageInfo.ImageData, 1024);

			if (BmpInfo.bmiHeader.biWidth != Width || BmpInfo.bmiHeader.biHeight != Height)
			{
#ifdef GDIPLUS_IMAGE_RESIZING
				Gdiplus::Bitmap *pResizedImage, *pSrcImage;

				pSrcImage = Gdiplus::Bitmap::FromHBITMAP(hBitmap, NULL);

				if (pSrcImage != NULL)
				{
					pResizedImage = (Gdiplus::Bitmap*)pSrcImage->GetThumbnailImage(Width, Height);

					if (pResizedImage != NULL)
					{
						DeleteObject(hBitmap);
						pResizedImage->GetHBITMAP(Gdiplus::Color::Transparent, &hBitmap);

						if (ItemID >= 0)
						{
							CLSID encoderClsid;
							CString Filename;

							GetEncoderClsid(_T("image/png"), &encoderClsid);

							Filename.Format(_T("img\\item%d.png"), ItemID);
							pResizedImage->Save(Filename, &encoderClsid, NULL);
						}

						delete pResizedImage;
					}

					delete pSrcImage;
				}
#else
				HBITMAP PrevSrcBmp, PrevDstBmp;
				CDC DstMemoryDC, SrcMemoryDC;
				BITMAPINFOHEADER CopyInfo;
				HBITMAP hResizedBitmap;

				SecureZeroMemory(&CopyInfo, sizeof(BITMAPINFOHEADER));
				CopyInfo.biSize = sizeof(BITMAPINFOHEADER);
				CopyInfo.biWidth = Width;
				CopyInfo.biHeight = Height;
				CopyInfo.biPlanes = 1;
				CopyInfo.biBitCount = 32;

				SrcMemoryDC.CreateCompatibleDC(pDC);
				PrevSrcBmp = (HBITMAP)SrcMemoryDC.SelectObject(hBitmap);

				DstMemoryDC.CreateCompatibleDC(pDC);
				hResizedBitmap = ::CreateDIBitmap(hDC, &CopyInfo, DIB_RGB_COLORS, NULL, NULL, 0);
				PrevDstBmp = (HBITMAP)DstMemoryDC.SelectObject(hResizedBitmap);

				DstMemoryDC.SetBkMode(TRANSPARENT);
				DstMemoryDC.SetStretchBltMode(COLORONCOLOR);
				DstMemoryDC.StretchBlt(0, 0 , Width, Height, &SrcMemoryDC, 0, 0, BmpInfo.bmiHeader.biWidth, BmpInfo.bmiHeader.biHeight, SRCCOPY);

				DeleteObject(hBitmap);
				SrcMemoryDC.SelectObject(PrevSrcBmp);
				DstMemoryDC.SelectObject(PrevDstBmp);

				hBitmap = hResizedBitmap;
#endif
			}

			return hBitmap;
		}

		DeleteObject(hDC);
	}

	return NULL;
}

#ifdef GDIPLUS_IMAGE_RESIZING
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
	{
		UINT  num = 0;          // number of image encoders
		UINT  size = 0;         // size of the image encoder array in bytes

		Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

		Gdiplus::GetImageEncodersSize(&num, &size);
		if(size == 0)
			return -1;  // Failure

		pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
		if(pImageCodecInfo == NULL)
			return -1;  // Failure

		Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

		for(UINT j = 0; j < num; ++j)
		{
			if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
			{
				*pClsid = pImageCodecInfo[j].Clsid;
				free(pImageCodecInfo);
				return j;  // Success
			}    
		}

		free(pImageCodecInfo);
		return -1;  // Failure
	}
#endif // GDIPLUS_IMAGE_RESIZING

#ifdef _DEBUG
	void CLootBoxDlg::DumpGlobalMap()
	{
		int CharID, FileID, ItemID, ItemCount = 0;
		POSITION GlobalPos, InvPos;
		InventoryItem *pItem;
		InventoryMap *pInvMap;
		ItemArray *pItemMap;
		
		GlobalPos = m_GlobalMap.GetStartPosition();
		pItemMap = NULL;
		pInvMap = NULL;

		TRACE(_T("=MAP==================================\n\tUSERS\n\t|\n"));

		while (GlobalPos != NULL)
		{
			m_GlobalMap.GetNextAssoc(GlobalPos, CharID, pInvMap);
			TRACE(_T("\t|_ %s (%s)\n"), m_CharacterNames.GetAt(CharID), m_CharacterIDs.GetAt(CharID));
			TRACE(_T("\t|\t|\n"));

			if (pInvMap != NULL)
			{
				InvPos = pInvMap->GetStartPosition();

				while (InvPos != NULL)
				{
					pInvMap->GetNextAssoc(InvPos, FileID, pItemMap);

					TRACE(_T("\t|\t|_ %s (%s)\n"), m_InventoryNames.GetAt(FileID), m_InventoryFiles.GetAt(FileID));
					TRACE(_T("\t|\t|\t|\n"));


					if (pItemMap != NULL)
					{
						POSITION ItemPos = pItemMap->GetStartPosition();

						while (ItemPos != NULL)
						{
							pItemMap->GetNextAssoc(ItemPos, ItemID, pItem);

							if (pItem != NULL)
								TRACE(_T("\t|\t|\t|_ %s (%d)\n"), pItem->ItemName, pItem->RefCount);
						}
					}
				}
			}
		}
	}
#endif // _DEBUG
