#include "stdafx.h"

#include "FFXIHelper.h"

// WAR|MNK|WHM|BLM|RDM|THF|PLD|DRK|BST|BRD|RNG|SAM|NIN|DRG|SMN|BLU|COR|PUP|DNC|SCH|All Jobs
const FFXiStringAssoc FFXiHelper::JobsStringTable[] = {   
	FFXiStringAssoc(FFXI_JOB_FLAG_WAR, _T("WAR")),
	FFXiStringAssoc(FFXI_JOB_FLAG_MNK, _T("MNK")),
	FFXiStringAssoc(FFXI_JOB_FLAG_WHM, _T("WHM")),
	FFXiStringAssoc(FFXI_JOB_FLAG_BLM, _T("BLM")),
	FFXiStringAssoc(FFXI_JOB_FLAG_RDM, _T("RDM")),
	FFXiStringAssoc(FFXI_JOB_FLAG_THF, _T("THF")),
	FFXiStringAssoc(FFXI_JOB_FLAG_PLD, _T("PLD")),
	FFXiStringAssoc(FFXI_JOB_FLAG_DRK, _T("DRK")),
	FFXiStringAssoc(FFXI_JOB_FLAG_BST, _T("BST")),
	FFXiStringAssoc(FFXI_JOB_FLAG_BRD, _T("BRD")),
	FFXiStringAssoc(FFXI_JOB_FLAG_RNG, _T("RNG")),
	FFXiStringAssoc(FFXI_JOB_FLAG_SAM, _T("SAM")),
	FFXiStringAssoc(FFXI_JOB_FLAG_NIN, _T("NIN")),
	FFXiStringAssoc(FFXI_JOB_FLAG_DRG, _T("DRG")),
	FFXiStringAssoc(FFXI_JOB_FLAG_SMN, _T("SMN")),
	FFXiStringAssoc(FFXI_JOB_FLAG_BLU, _T("BLU")),
	FFXiStringAssoc(FFXI_JOB_FLAG_COR, _T("COR")),
	FFXiStringAssoc(FFXI_JOB_FLAG_PUP, _T("PUP")),
	FFXiStringAssoc(FFXI_JOB_FLAG_DNC, _T("DNC")),
	FFXiStringAssoc(FFXI_JOB_FLAG_SCH, _T("SCH")),
	FFXiStringAssoc(-1				 , NULL),
};

// Slots
const FFXiStringAssoc FFXiHelper::SlotStringTable[] = {   
	FFXiStringAssoc(FFXI_SLOT_FLAG_MAIN		, _T("Weapon")),
	FFXiStringAssoc(FFXI_SLOT_FLAG_SUB		, _T("Shield")),
	FFXiStringAssoc(FFXI_SLOT_FLAG_RANGE	, _T("Ranged")),
	FFXiStringAssoc(FFXI_SLOT_FLAG_AMMO		, _T("Ammo")),
	FFXiStringAssoc(FFXI_SLOT_FLAG_HEAD		, _T("Head")),
	FFXiStringAssoc(FFXI_SLOT_FLAG_BODY		, _T("Body")),
	FFXiStringAssoc(FFXI_SLOT_FLAG_HANDS	, _T("Hands")),
	FFXiStringAssoc(FFXI_SLOT_FLAG_LEGS		, _T("Legs")),
	FFXiStringAssoc(FFXI_SLOT_FLAG_FEET		, _T("Feet")),
	FFXiStringAssoc(FFXI_SLOT_FLAG_NECK		, _T("Neck")),
	FFXiStringAssoc(FFXI_SLOT_FLAG_WAIST	, _T("Waist")),
	FFXiStringAssoc(FFXI_SLOT_FLAG_L_EAR	, _T("Earring")),
	FFXiStringAssoc(FFXI_SLOT_FLAG_R_EAR	, _T("Earring")),
	FFXiStringAssoc(FFXI_SLOT_FLAG_L_RING	, _T("Ring")),
	FFXiStringAssoc(FFXI_SLOT_FLAG_R_RING	, _T("Ring")),
	FFXiStringAssoc(FFXI_SLOT_FLAG_BACK		, _T("Back")),
	FFXiStringAssoc(-1						, NULL),
};

// Skills
const FFXiStringAssoc FFXiHelper::SkillStringTable[] = {   
	FFXiStringAssoc(FFXI_SKILL_TYPE_BST_JUG				, _T("Ammo")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_H2H					, _T("Hand-to-Hand")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_DAGGER				, _T("Dagger")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_SWORD				, _T("Sword")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_GREAT_SWORD			, _T("Great Sword")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_AXE					, _T("Axe")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_GREAT_AXE			, _T("Great Axe")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_SCYTHE				, _T("Scythe")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_POLEARM				, _T("Polearm")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_KATANA				, _T("Katana")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_GREAT_KATANA		, _T("Great Katana")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_CLUB				, _T("Club")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_STAFF				, _T("Staff")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_RANGED				, _T("Archery")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_MARKSMANSHIP		, _T("Marskmanship")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_THROWING			, _T("Throwing")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_DIVINE_MAGIC		, _T("Divine Magic")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_HEALING_MAGIC		, _T("Healing Magic")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_ENHANCING_MAGIC		, _T("Enhancing Magic")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_ENFEEBLING_MAGIC	, _T("Enfeebling Magic")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_ELEMENTAL_MAGIC		, _T("Elemental Magic")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_DARK_MAGIC			, _T("Dark Magic")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_SUMMONING_MAGIC		, _T("Summoning Magic")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_NINJUTSU			, _T("Ninjutsu")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_SINGING				, _T("Singing")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_STRING_INSTRUMENT	, _T("String Instrument")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_WIND_INSTRUMENT		, _T("Wind Instrument")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_FISHING				, _T("Fishing")),
	FFXiStringAssoc(FFXI_SKILL_TYPE_PET_FOOD			, _T("Pet Food")),
	FFXiStringAssoc(-1, NULL),
};

// Usable Items
const FFXiStringAssoc FFXiHelper::UsableItemStringTable[] = {  
	FFXiStringAssoc(ITEM_FLAG_SCROLL	, _T("Scroll")),
	FFXiStringAssoc(ITEM_FLAG_LINKSHELL	, _T("Linkshell")),
	FFXiStringAssoc(-1					, NULL),
};

// Races \u2640 : ♀ (female) \u2642 : ♂ (male)
// All Races|All M|All F|H|HM|HF|E|EM|EF|T|TM|TF|M|G
const FFXiStringAssoc FFXiHelper::RacesStringTable[] = {  
	FFXiStringAssoc(FFXI_RACE_FLAG_HUME_MALE		, _T("Hume \u2642")),
	FFXiStringAssoc(FFXI_RACE_FLAG_HUME_FEMALE		, _T("Hume \u2640")),
	FFXiStringAssoc(FFXI_RACE_FLAG_ELVAAN_MALE		, _T("Elvaan \u2642")),
	FFXiStringAssoc(FFXI_RACE_FLAG_ELVAAN_FEMALE	, _T("Elvaan \u2640")),
	FFXiStringAssoc(FFXI_RACE_FLAG_TARUTARU_MALE	, _T("Tarutaru \u2642")),
	FFXiStringAssoc(FFXI_RACE_FLAG_TARUTARU_FEMALE	, _T("Tarutaru \u2640")),
	FFXiStringAssoc(FFXI_RACE_FLAG_MITHRA			, _T("Mithra")),
	FFXiStringAssoc(FFXI_RACE_FLAG_GALKA			, _T("Galka")),
	FFXiStringAssoc(-1								, NULL),
};

inline BYTE FFXiHelper::RotateRight(BYTE Value_in, int BitShift_in)
{
	// make sure we don't shift more than 8 bits
	BitShift_in &= 0x7;

	// rotate the bits right
	return (Value_in >> BitShift_in) | (Value_in << (8 - BitShift_in));
}

inline BYTE FFXiHelper::RotateLeft(BYTE Value_in, int BitShift_in)
{
	// make sure we don't shift more than 8 bits
	BitShift_in &= 0x7;

	// rotate the bits right
	return (Value_in << BitShift_in) | (Value_in >> (8 - BitShift_in));
}

void FFXiHelper::RotateBits(const BYTE *pData_in ,BYTE * pData_out, int DataSize_in, int BitShift_in)
{
	const BYTE *pPos = pData_in;

	if (pData_in != NULL && pData_out != NULL)
	{
		for(int i = 0; i < DataSize_in; i++, pPos++, pData_out++)
			if (*pPos != NULL && *pPos != 0xFF)
				*pData_out = RotateRight(*pPos, BitShift_in);

		//*pData_out = NULL;
	}
}

TCHAR* FFXiHelper::GetInstallPath(int Region)
{
	CRegKey FFXiKey;

	if (m_InstallFolder.IsEmpty() || Region != m_Region)
	{
		LPCTSTR GameRegKey;

		switch(Region)
		{
			default:
			case FFXI_REGION_JP:
				GameRegKey = FFXI_REGISTRY_KEY_INSTALL_JP;
			break;
			case FFXI_REGION_US:
				GameRegKey = FFXI_REGISTRY_KEY_INSTALL_US;
			break;
			case FFXI_REGION_EU:
				GameRegKey = FFXI_REGISTRY_KEY_INSTALL_EU;
			break;
		}

		if (FFXiKey.Open(HKEY_LOCAL_MACHINE, GameRegKey, KEY_READ) == ERROR_SUCCESS)
		{
			ULONG DataRead;
			LRESULT Result;
			LPTSTR pStr;		

			FFXiKey.QueryStringValue(REGISTRY_KEY_FFXI_KEYNAME, NULL, &DataRead);
			pStr = m_InstallFolder.GetBuffer(DataRead);
			Result = FFXiKey.QueryStringValue(REGISTRY_KEY_FFXI_KEYNAME, pStr, &DataRead);
			m_InstallFolder.ReleaseBuffer(DataRead);
		}
	}

	return m_InstallFolder.GetBuffer();
}

bool FFXiHelper::ReadItem(BYTE *pItemData, InventoryItem *pItem, int Language, bool NoConversion)
{
	BYTE *pItemPos, *pIconPos;

	if (pItemData != NULL && pItem != NULL)
	{
		pIconPos = pItemData + DATA_SIZE_ITEM_INFO + 0x84;
		pItemPos = pItemData;

		pItemPos += GetItemHdr(pItemData, pItem->ItemHdr);

		switch(pItem->ItemHdr.Type)
		{
			case ITEM_OBJECT_TYPE_ARMOR:
				pItemPos += GetArmorInfo(pItemPos, pItem->ArmorInfo);
				pItem->Level.Format(_T("%d"), pItem->ArmorInfo.Level);
				GetJobs(pItem->ArmorInfo.Jobs, pItem->Jobs);
				GetSlot(pItem->ArmorInfo.Slot, pItem->Slot);
				GetRaces(pItem->ArmorInfo.Races, pItem->Races);
				GetArmorChargesInfo(pItem->ArmorInfo, pItem->Remarks);
			break;

			case ITEM_OBJECT_TYPE_WEAPON:
				pItemPos += GetWeaponInfo(pItemPos, pItem->WeaponInfo);
				pItem->Level.Format(_T("%d"), pItem->WeaponInfo.Level);
				GetJobs(pItem->WeaponInfo.Jobs, pItem->Jobs);
				GetSkill(pItem->WeaponInfo.Skill, pItem->Slot);
				GetRaces(pItem->WeaponInfo.Races, pItem->Races);
				GetWeaponChargesInfo(pItem->WeaponInfo, pItem->Remarks);

				if (pItem->WeaponInfo.Skill == FFXI_SKILL_TYPE_BST_JUG &&
					pItem->WeaponInfo.Jobs == FFXI_JOB_FLAG_BST)
				{
					pItem->Slot = _T("Jug Pet");
				}
			break;

			case ITEM_OBJECT_TYPE_CRYSTAL:
			case ITEM_OBJECT_TYPE_LINKSHELL:
			case ITEM_OBJECT_TYPE_USABLE_ITEM:
			case ITEM_OBJECT_TYPE_ITEM:
				pItemPos += GetUsableItemInfo(pItemPos, pItem->UsableItemInfo);
				GetUsableItemType(pItem->ItemHdr.Flags, pItem->Slot);
			break;

			default:
				if (pItem->ItemHdr.ItemID <= 0 || pItem->ItemHdr.ItemID > 0x6FFF)
					break;
			case ITEM_OBJECT_TYPE_FURNISHING:
			case ITEM_OBJECT_TYPE_MANNEQUIN:
			case ITEM_OBJECT_TYPE_FLOWERPOT:
				pItemPos += GetObjectInfo(pItemPos, pItem->ObjectInfo);
			break;

			case ITEM_OBJECT_TYPE_NOTHING:
				if (pItem->ItemHdr.ItemID < 0x2000 || pItem->ItemHdr.ItemID > 0x2BFF)
					break;
			case ITEM_OBJECT_TYPE_PUPPET_ITEM:			
				pItemPos += GetPuppetInfo(pItemPos, pItem->PuppetInfo);
			break;
		};

		GetAttr(pItem->ItemHdr.Flags, pItem->Attr);
		GetItemInfo(pItemPos, Language, pItem->ItemName, pItem->LogName,
					pItem->LogName2, pItem->ItemDescription, NoConversion);
		GetIconInfo(pIconPos, pItem->IconInfo);

		if (pItem->Slot == _T("Scroll"))
			GetScrollInfoFromDesc(pItem->ItemDescription, pItem->Jobs);
		if (pItem->ItemHdr.Type == ITEM_OBJECT_TYPE_ARMOR)
			GetDefenseFromDesc(Language, pItem->ItemDescription, pItem->ArmorInfo.Defense);

		pItem->RefCount = 1;

		return true;
	}

	return false;
}

bool FFXiHelper::ParseInventoryFile(const TCHAR* pFile, const ItemLocationInfo &LocationInfo,
									ItemArray *pMap, int Language, bool Update)
{
	if (pFile != NULL && pMap != NULL)
	{
		CFile InvFile;

		if (InvFile.Open(pFile, CFile::modeRead | CFile::shareDenyNone))
		{
			int ItemIndex = 0, ImageIndex = -1;
			BYTE *pItemData, *pFileData = NULL;
			const WORD *pLimit, *pPos;
			CString ItemCount;
			UINT DataRead;

			pFileData = (BYTE*)malloc(DATA_SIZE_INVENTORY+1);
			SecureZeroMemory(pFileData, DATA_SIZE_INVENTORY);

			DataRead = (UINT)InvFile.Seek(OFFSET_FILEHEADER, CFile::begin);
			DataRead = InvFile.Read(pFileData, DATA_SIZE_INVENTORY);
			pLimit = (WORD*)pFileData + DATA_SIZE_INVENTORY / sizeof(WORD);

			InvFile.Close();

			if (DataRead != NULL)
			{
				DWORD ItemID, InvItemID;
				LONG DataOffset;
				CString DATFile;

				pItemData = (BYTE*)malloc(DATA_SIZE_ITEM + 1);
				SecureZeroMemory(pItemData, DATA_SIZE_ITEM+1);

				for (pPos = (WORD*)pFileData + 4; pPos < pLimit; pPos += 4)
				{
					InventoryItem *pItem = NULL;

					InvItemID = ItemID = *pPos;

					if (ItemID > 0 && ItemID <= 0x6FFF)
					{
						GetItemFromID(ItemID, pMap, &pItem);

						if (pItem == NULL || Update)
						{
							GetFileFromItemID(ItemID, DATFile, Language);
							DataOffset = ItemID * DATA_SIZE_ITEM;

							if (DATFile.IsEmpty() == false && InvFile.Open(DATFile, CFile::modeRead | CFile::shareDenyNone))
							{
								if (pItem == NULL)
									pItem = new InventoryItem();

								ClearItemData(pItem);

								pItem->LocationInfo = LocationInfo;
								pItem->LocationInfo.ListIndex = ItemIndex++;

								DataRead = (UINT)InvFile.Seek(DataOffset, CFile::begin);
								DataRead = InvFile.Read(pItemData, DATA_SIZE_ITEM);
								InvFile.Close();

								FFXiHelper::RotateBits(pItemData, pItemData, DATA_SIZE_ITEM, RSHIFT_DECRYPT_ITEM);

								if (ReadItem(pItemData, pItem, Language))
									if (Update == false)
										pMap->Add(pItem);
							}
						}
						else
						{
							pItem->RefCount++;
						}
					}
				}

				if (pItemData != NULL)
				{
					free(pFileData);
					pFileData = NULL;
				}

				if (pItemData != NULL)
				{
					free(pItemData);
					pItemData = NULL;
				}
			}

			return true;
		}
	}

	return false;
}

void FFXiHelper::GetBYTE(BYTE **pData, BYTE &Result, bool MovePtr)
{
	if (*pData != NULL)
	{
		Result = *((BYTE*)*pData);

		if (MovePtr)
			*pData += sizeof(BYTE);
	}
}

int FFXiHelper::GetBYTE(const BYTE *pData, BYTE &Result)
{
	if (pData == NULL)
		return 0;
	
	Result = *pData;
	
	return sizeof(BYTE);
}

void FFXiHelper::GetWORD(BYTE **pData, WORD &Result, bool MovePtr)
{
	if (*pData != NULL)
	{
		Result = *((WORD*)pData);

		if (MovePtr)
			*pData += sizeof(WORD);
	}
}

int FFXiHelper::GetWORD(const BYTE *pData, WORD &Result)
{
	if (pData == NULL)
		return 0;
	
	Result = *((WORD*)pData);
	
	return sizeof(WORD);
}

void FFXiHelper::GetDWORD(BYTE **pData, DWORD &Result, bool MovePtr)
{
	if (*pData != NULL)
	{
		Result = *((DWORD*)*pData);

		if (MovePtr)
			*pData += sizeof(DWORD);
	}
}

int FFXiHelper::GetDWORD(const BYTE *pData, DWORD &Result)
{
	if (pData == NULL)
		return 0;
	
	Result = *((DWORD*)pData);

	return sizeof(DWORD);
}

void FFXiHelper::UpperCaseWord(CString &String)
{
	TCHAR *pChar = String.GetBuffer();
	int StrLength, StrPos = 0;

	StrLength = String.GetLength();

	if (StrLength > 0)
	{
		*pChar = _totupper(*pChar);
		StrPos = String.Find(' ', StrPos);

		while(StrPos != -1)
		{
			if (StrPos + 1 < StrLength)
				*(pChar + StrPos + 1) = _totupper(*(pChar + StrPos + 1));

			StrPos = String.Find(' ', StrPos+1);
		}

		String.ReleaseBuffer();
	}
}

void FFXiHelper::ConvertChars(const BYTE *pData, CString &Text, bool ucWord)
{
	if (pData != NULL)
	{
		int Length = (int)_mbslen(pData);
		BYTE Current, Prev;

		Current = NULL;

		for (int i = 0; i < Length; i++)
		{
			Prev = Current;
			Current = *(pData + i);

			if (Current == 0x85)
				Current = *(pData + (++i)) + 33;
			else if (Current == 0x81) {
				Current = '~';
				i++;
			}

			if (ucWord && (i == 0 || Prev == ' ' || Prev == 0x0A))
				Current = toupper(Current);

		
			if (Current == 0xEF)
			{
				Current = *(pData + (++i));

				// 0x1F-0x26 elements (Fire - Ice - Wind - Earth - Lightning - Water - Light - Dark)
				switch(Current)
				{
					case 0x1F:
						Text += _T("(Fire)");
					break;
					case 0x20:
						Text += _T("(Ice)");
					break;
					case 0x21:
						Text += _T("(Wind)");
					break;
					case 0x22:
						Text += _T("(Earth)");
					break;
					case 0x23:
						Text += _T("(Lightning)");
					break;
					case 0x24:
						Text += _T("(Water)");
					break;
					case 0x25:
						Text += _T("(Light)");
					break;
					case 0x26:
						Text += _T("(Dark)");
					break;
				}
			}
			else if (Current == 0x0A)
			{
				if (i < Length - 1)
					Text += _T(" \r\n");
			}
			else
				Text += Current;
		}
	}
}

/*
ITEM_TYPE_OBJECTS		= 0,
ITEM_TYPE_USABLE_ITEM,
ITEM_TYPE_PUPPET_ITEM,
ITEM_TYPE_ARMOR,
ITEM_TYPE_WEAPON,
*/
void FFXiHelper::GetFileFromType(int Type, CString &DATFile, int Language, bool bRelative)
{
	DWORD ItemID;

	switch(Type)
	{
		default:
		case ITEM_TYPE_OBJECTS:
			ItemID = 1;
		break;
		case ITEM_TYPE_USABLE_ITEM:
			ItemID = 0x0FFF;
		break;
		case ITEM_TYPE_PUPPET_ITEM:
			ItemID = 0x1FFF;
		break;
		case ITEM_TYPE_ARMOR:
			ItemID = 0x2BFF;
		break;
		case ITEM_TYPE_WEAPON:
			ItemID = 0x3FFF;
		break;
	}

	GetFileFromItemID(ItemID, DATFile, Language, bRelative);
}

void FFXiHelper::GetFileFromItemID(DWORD &ItemID, CString &DATFile, int Language, bool bRelative)
{
	DATFile = _T("");

	if (ItemID > 0)
	{
		// 0000 - 0FFF  Objects 
		if (ItemID < 0x0FFF)
		{
			switch(Language)
			{
				default:
				case FFXI_LANG_JP:
					// file #00004 (ROM/0/4.DAT    ) => JP Objects
					if (bRelative)
						DATFile = _T("ROM\\0\\4.DAT");
					else
						DATFile.Format(_T("%sROM\\0\\4.DAT"), m_InstallFolder);
				break;
				case FFXI_LANG_US:
					// file #00073 (ROM/118/106.DAT) => EN Objects
					if (bRelative)
						DATFile = _T("ROM\\118\\106.DAT");
					else
						DATFile.Format(_T("%sROM\\118\\106.DAT"), m_InstallFolder);
				break;
				case FFXI_LANG_FR:
					// file #56235 (ROM/178/40.DAT ) => FR Objects
					if (bRelative)
						DATFile = _T("ROM\\178\\40.DAT");
					else
						DATFile.Format(_T("%sROM\\178\\40.DAT"), m_InstallFolder);
				break;
				// file #55815 (ROM/176/101.DAT) => DE Objects
				case FFXI_LANG_DE:
					if (bRelative)
						DATFile = _T("ROM\\176\\101.DAT");
					else
						DATFile.Format(_T("%sROM\\176\\101.DAT"), m_InstallFolder);
				break;
			}
		}
		// 1000 - 1FFF  Usable Item 
		else if (ItemID < 0x1FFF)
		{
			ItemID -= 0x1000;

			switch(Language)
			{
				default:
				case FFXI_LANG_JP:
					// file #00005 (ROM/0/5.DAT    ) => JP Usable Items
					if (bRelative)
						DATFile = _T("ROM\\0\\5.DAT");
					else
						DATFile.Format(_T("%sROM\\0\\5.DAT"), m_InstallFolder);
				break;
				case FFXI_LANG_US:
					// file #00074 (ROM/118/107.DAT) => EN Usable Items
					if (bRelative)
						DATFile = _T("ROM\\118\\107.DAT");
					else
						DATFile.Format(_T("%sROM\\118\\107.DAT"), m_InstallFolder);
				break;
				case FFXI_LANG_FR:
					// file #56236 (ROM/178/41.DAT ) => FR Usable Items
					if (bRelative)
						DATFile = _T("ROM\\178\\41.DAT");
					else
						DATFile.Format(_T("%sROM\\178\\41.DAT"), m_InstallFolder);
				break;
				case FFXI_LANG_DE:
					// file #55816 (ROM/176/102.DAT) => DE Usable Items
					if (bRelative)
						DATFile = _T("ROM\\176\\102.DAT");
					else
						DATFile.Format(_T("%sROM\\176\\102.DAT"), m_InstallFolder);
				break;
			}
		}
		// 2000 - 27FF  Puppet Item 
		else if (ItemID < 0x27FF)
		{
			ItemID -= 0x2000;
			
			switch(Language)
			{
				default:
				case FFXI_LANG_JP:
					// file #00008 (ROM/0/8.DAT    ) => JP Puppet Items
					if (bRelative)
						DATFile = _T("ROM\\0\\8.DAT");
					else
						DATFile.Format(_T("%sROM\\0\\8.DAT"), m_InstallFolder);
				break;
				case FFXI_LANG_US:
					// file #00077 (ROM/118/110.DAT) => EN Puppet Items
					if (bRelative)
						DATFile = _T("ROM\\118\\110.DAT");
					else
						DATFile.Format(_T("%sROM\\118\\110.DAT"), m_InstallFolder);
				break;
				case FFXI_LANG_FR:
					// file #56239 (ROM/178/44.DAT ) => FR Puppet Items 
					if (bRelative)
						DATFile = _T("ROM\\178\\44.DAT");
					else
						DATFile.Format(_T("%sROM\\178\\44.DAT"), m_InstallFolder);
				break;
				case FFXI_LANG_DE:
					// file #55819 (ROM/176/115.DAT) => DE Puppet Items
					if (bRelative)
						DATFile = _T("ROM\\176\\115.DAT");
					else
						DATFile.Format(_T("%sROM\\176\\115.DAT"), m_InstallFolder);
				break;
			}
		}
		// 2800 - 3FFF  Armor 
		else if (ItemID < 0x3FFF)
		{
			ItemID -= 0x2800;
			
			switch(Language)
			{
				default:
				case FFXI_LANG_JP:
					// file #00007 (ROM/0/7.DAT    ) => JP Armor
					if (bRelative)
						DATFile = _T("ROM\\0\\7.DAT");
					else
						DATFile.Format(_T("%sROM\\0\\7.DAT"), m_InstallFolder);
				break;
				case FFXI_LANG_US:
					// file #00076 (ROM/118/109.DAT) => EN Armor
					if (bRelative)
						DATFile = _T("ROM\\118\\109.DAT");
					else
						DATFile.Format(_T("%sROM\\118\\109.DAT"), m_InstallFolder);
				break;
				case FFXI_LANG_FR:
					// file #56238 (ROM/178/43.DAT ) => FR Armor
					if (bRelative)
						DATFile = _T("ROM\\178\\43.DAT");
					else
						DATFile.Format(_T("%sROM\\178\\43.DAT"), m_InstallFolder);
				break;
				case FFXI_LANG_DE:
					// file #55818 (ROM/176/104.DAT) => DE Armor
					if (bRelative)
						DATFile = _T("ROM\\176\\104.DAT");
					else
						DATFile.Format(_T("%sROM\\176\\104.DAT"), m_InstallFolder);
				break;
			}
		}
		// 4000 - 6FFF  Weapons 
		else if (ItemID < 0x6FFF)
		{
			ItemID -= 0x4000;
			
			switch(Language)
			{
				default:
				case FFXI_LANG_JP:
					// file #00006 (ROM/0/6.DAT    ) => JP Weapons
					if (bRelative)
						DATFile = _T("ROM\\0\\6.DAT");
					else
						DATFile.Format(_T("%sROM\\0\\6.DAT"), m_InstallFolder);
				break;
				case FFXI_LANG_US:
					// file #00075 (ROM/118/108.DAT) => EN Weapons
					if (bRelative)
						DATFile = _T("ROM\\118\\108.DAT");
					else
						DATFile.Format(_T("%sROM\\118\\108.DAT"), m_InstallFolder);
				break;
				case FFXI_LANG_FR:
					// file #56237 (ROM/178/42.DAT ) => FR Weapons
					if (bRelative)
						DATFile = _T("ROM\\178\\42.DAT");
					else
						DATFile.Format(_T("%sROM\\178\\42.DAT"), m_InstallFolder);
				break;
				case FFXI_LANG_DE:
					// file #55817 (ROM/176/103.DAT) => DE Weapons
					if (bRelative)
						DATFile = _T("ROM\\176\\103.DAT");
					else
						DATFile.Format(_T("%sROM\\176\\103.DAT"), m_InstallFolder);
				break;
			}
		}
	}
}

int FFXiHelper::DetectGameRegion(int &Regions)
{
	int RegionFound = 0;
	CRegKey FFXiKey;

	Regions = 0;

	if (FFXiKey.Open(HKEY_LOCAL_MACHINE, FFXI_REGISTRY_KEY_INSTALL_JP, KEY_READ) == ERROR_SUCCESS)
	{
		Regions |= FFXI_REGION_FLAG_JP;
		FFXiKey.Close();
		RegionFound++;
	}
	if (FFXiKey.Open(HKEY_LOCAL_MACHINE, FFXI_REGISTRY_KEY_INSTALL_US, KEY_READ) == ERROR_SUCCESS)
	{
		Regions |= FFXI_REGION_FLAG_US;
		FFXiKey.Close();
		RegionFound++;
	}
	if (FFXiKey.Open(HKEY_LOCAL_MACHINE, FFXI_REGISTRY_KEY_INSTALL_EU, KEY_READ) == ERROR_SUCCESS)
	{
		Regions |= FFXI_REGION_FLAG_EU;
		FFXiKey.Close();
		RegionFound++;
	}
	else
		return 0;

	return RegionFound;
}

void FFXiHelper::GetScrollInfoFromDesc(const CString &Description, CString &Jobs)
{
	int SearchPos = -1, Pos = 0;
	CString Token, TmpStr;
	bool Empty = true;

	if (Description.IsEmpty() == false)
	{
		Token = Description.Tokenize(_T("/\r\n"), Pos);

		while (Token != _T(""))
		{
			Token = Token.Trim();
			SearchPos = Token.Find(_T(" Lv."));

			if (SearchPos != -1)
			{
				// retrieve the job
				TmpStr = Token.Left(SearchPos);
				// add it to the result
				if (TmpStr.IsEmpty() == false)
				{
					if (Empty == false)
						Jobs += '/';
					else
						Empty = false;

					Jobs += TmpStr;
				}
				// search for the level
				TmpStr = Token.Tokenize(_T(" "), SearchPos);

				if (TmpStr.IsEmpty() == false)
				{
					TmpStr.Replace(_T("Lv."), _T(""));

					if (TmpStr.IsEmpty() == false)
						Jobs.AppendFormat(_T("(%s)"), TmpStr);
				}
			}

			Token = Description.Tokenize(_T("/"), Pos);
		}
	}
}

void FFXiHelper::GetDefenseFromDesc(int Language, const CString &Description, WORD &Defense)
{
	int PosStart = 0, PosEnd = 0;
	CString DefenseStr;

	if (Language == FFXI_LANG_JP)
	{
		PosStart = Description.Find(_T("\u9632"));

		if (PosStart != -1)
		{
			PosStart++; // strlen("\u9632");
			PosEnd = Description.Find(' ', PosStart);
			DefenseStr = Description.Mid(PosStart, PosEnd - PosStart);
			Defense = _ttoi(DefenseStr);
		}
	}
	else
	{
		PosStart = Description.Find(_T("DEF:"));

		if (PosStart != -1)
		{
			PosStart += 4; // strlen("DEF:");
			PosEnd = Description.Find(' ', PosStart);
			DefenseStr = Description.Mid(PosStart, PosEnd - PosStart);
			Defense = _ttoi(DefenseStr);
		}
	}
}

void FFXiHelper::GetItemInfo(const BYTE *pTablePos, int Language, CString &ItemName, CString &LogName,
							 CString &LogName2, CString &ItemDescription, bool NoConversion)
{
	const BYTE *pItemName, *pLogName, *pLogName2, *pItemDescription;

	if (Language == FFXI_LANG_JP)
	{
		int Length;

		pItemName = GetStringOffset(pTablePos, 0);
		pItemDescription = GetStringOffset(pTablePos, 1);

		if (pItemName)
		{
			Length = (int)_mbslen(pItemName);
			Length = MultiByteToWideChar(932, MB_PRECOMPOSED, (LPCSTR)pItemName, Length, ItemName.GetBuffer(Length), Length);
			ItemName.ReleaseBuffer(Length);
			LogName2 = LogName = ItemName;
		}

		if (pItemDescription)
		{
			Length = (int)_mbslen(pItemDescription);
			Length = MultiByteToWideChar(932, MB_PRECOMPOSED, (LPCSTR)pItemDescription, Length, ItemDescription.GetBuffer(Length), Length);
			ItemDescription.ReleaseBuffer(Length);
		}
	}
	else
	{
		int TooltipOffset, Tooltip2Offset, DescriptionOffset;

		switch(Language)
		{
			case FFXI_LANG_US:
				TooltipOffset = 2;
				Tooltip2Offset = 3;
				DescriptionOffset = 4;
			break;
			case FFXI_LANG_FR:
				TooltipOffset = 3;
				Tooltip2Offset = 4;
				DescriptionOffset = 5;
			break;
			case FFXI_LANG_DE:
				TooltipOffset = 4;
				Tooltip2Offset = 7;
				DescriptionOffset = 8;
			break;
		}

		pItemName = GetStringOffset(pTablePos, 0);
		pLogName = GetStringOffset(pTablePos, TooltipOffset);
		pLogName2 = GetStringOffset(pTablePos, Tooltip2Offset);
		pItemDescription = GetStringOffset(pTablePos, DescriptionOffset);

		if (NoConversion)
		{
			ItemName = pItemName;
			ItemDescription = pItemDescription;
			LogName = pLogName;
			LogName2 = pLogName2;
		}
		else
		{
			ConvertChars(pItemName, ItemName);
			ConvertChars(pItemDescription, ItemDescription);
			ConvertChars(pLogName, LogName, true);
			ConvertChars(pLogName2, LogName2, true);	
		}
	}
}

const BYTE *FFXiHelper::GetStringOffset(const BYTE *pStringTable, int Offset)
{
	int PtrOffset, EntryCount = *(pStringTable);

	if (Offset < EntryCount)
	{
		PtrOffset = *(pStringTable + sizeof(DWORD) + Offset * 2 * sizeof(DWORD));

		return pStringTable + PtrOffset + FFXI_ITEM_STRING_TABLE_PADDING;
	}
	else
		return NULL;
}

int FFXiHelper::GetItemHdr(const BYTE *pItemData, FFXiItemHeader &ItemHdr)
{
	int Offset = 0;

	if (pItemData)
	{
		//	Offset Type   Description
		//	------ ------ --------------
		// 000    UINT32 ID
		Offset += GetDWORD(pItemData + Offset, ItemHdr.ItemID);
		// 004    UINT16 Flags
		Offset += GetWORD(pItemData + Offset, ItemHdr.Flags);
		// 006    UINT16 Stack Size
		Offset += GetWORD(pItemData + Offset, ItemHdr.StackSize);
		// 008    UINT16 Type
		Offset += GetWORD(pItemData + Offset, ItemHdr.Type);
		// 00A    UINT16 ResourceID
		Offset += GetWORD(pItemData + Offset, ItemHdr.ResourceID);
		// 00C    UINT16 ValidTargets
		Offset += GetWORD(pItemData + Offset, ItemHdr.ValidTargets);
	}

	return Offset;
}

int FFXiHelper::GetWeaponInfo(const BYTE *pItemData, FFXiWeaponInfo &WeaponInfo)
{
	int Offset = 0;

	if (pItemData)
	{
		// 00E    UINT16 Level
		Offset += GetWORD(pItemData + Offset, WeaponInfo.Level);
		// 010    UINT16 Slots
		Offset += GetWORD(pItemData + Offset, WeaponInfo.Slot);
		// 012    UINT16 Races
		Offset += GetWORD(pItemData + Offset, WeaponInfo.Races);
		// 014    UINT32 Jobs
		Offset += GetDWORD(pItemData + Offset, WeaponInfo.Jobs);
		// 018    UINT16 Damage
		Offset += GetWORD(pItemData + Offset, WeaponInfo.Damage);
		// 01A    UINT16 Delay
		Offset += GetWORD(pItemData + Offset, WeaponInfo.Delay);
		// 01C    UINT16 DPS
		Offset += GetWORD(pItemData + Offset, WeaponInfo.DPS);
		// 01E    UINT8  Skill
		Offset += GetBYTE(pItemData + Offset, WeaponInfo.Skill);
		// 01F    UINT8  Jug Size
		Offset += GetBYTE(pItemData + Offset, WeaponInfo.JugSize);
		// 020    UINT32 Unknown
		Offset += GetDWORD(pItemData + Offset, WeaponInfo.Unknown1);
		// 024    UINT8  Max Charges
		Offset += GetBYTE(pItemData + Offset, WeaponInfo.MaxCharges);
		// 025    UINT8  Casting Time
		Offset += GetBYTE(pItemData + Offset, WeaponInfo.CastingTime);
		// 026    UINT16 Use Delay
		Offset += GetWORD(pItemData + Offset, WeaponInfo.UseDelay);
		// 028    UINT32 Re-Use Delay
		Offset += GetDWORD(pItemData + Offset, WeaponInfo.ReuseDelay);
		// 02C    UINT32 Unknown (added March 10th 2008) 
		Offset += GetDWORD(pItemData + Offset, WeaponInfo.Unknown2);
	}

	return Offset;
}

int FFXiHelper::GetArmorInfo(const BYTE *pItemData, FFXiArmorInfo &ArmorInfo)
{
	int Offset = 0;

	if (pItemData)
	{
		// 00E    UINT16 Level
		Offset += GetWORD(pItemData + Offset, ArmorInfo.Level);
		// 010    UINT16 Slots
		Offset += GetWORD(pItemData + Offset, ArmorInfo.Slot);
		// 012    UINT16 Races
		Offset += GetWORD(pItemData + Offset, ArmorInfo.Races);
		// 014    UINT32 Jobs
		Offset += GetDWORD(pItemData + Offset, ArmorInfo.Jobs);
		// 018    UINT16 Shield Size
		Offset += GetWORD(pItemData + Offset, ArmorInfo.ShieldSize);
		// 01A    UINT8  Max Charges
		Offset += GetBYTE(pItemData + Offset, ArmorInfo.MaxCharges);
		// 01B    UINT8  Casting Time
		Offset += GetBYTE(pItemData + Offset, ArmorInfo.CastingTime);
		// 01C    UINT16 Use Delay
		Offset += GetWORD(pItemData + Offset, ArmorInfo.UseDelay);
		// 01E    UINT16 Unknown
		Offset += GetWORD(pItemData + Offset, ArmorInfo.Defense);
		// 020    UINT32 Re-Use Delay
		Offset += GetDWORD(pItemData + Offset, ArmorInfo.ReuseDelay);
		// 024    UINT32 Unknown (added March 10th 2008) 
		Offset += GetDWORD(pItemData + Offset, ArmorInfo.Unknown2);
	}

	return Offset;
}

int FFXiHelper::GetPuppetInfo(const BYTE *pItemData, FFXiPuppetInfo &PuppetInfo)
{
	int Offset = 0;

	if (pItemData)
	{
		// 00E    UINT16 Puppet Slot
		Offset += GetWORD(pItemData + Offset, PuppetInfo.Slot);
		// 010    UINT32 Element Charge
		Offset += GetDWORD(pItemData + Offset, PuppetInfo.Element);
		// 014    UINT32 Unknown 
		Offset += GetDWORD(pItemData + Offset, PuppetInfo.Unknown);
	}

	return Offset;
}

int FFXiHelper::GetObjectInfo(const BYTE *pItemData, FFXiObjectInfo &ObjectInfo)
{
	int Offset = 0;

	if (pItemData)
	{
		// 00E    UINT16 Element
		Offset += GetWORD(pItemData + Offset, ObjectInfo.Element);
		// 010    UINT32 Storage Slots 
		Offset += GetDWORD(pItemData + Offset, ObjectInfo.Storage);
	}

	return Offset;
}

int FFXiHelper::GetUsableItemInfo(const BYTE *pItemData, FFXiUsableItemInfo &UsableItemInfo)
{
	int Offset = 0;

	if (pItemData)
	{
		// 00E    UINT16 Activation Time
		Offset += GetWORD(pItemData + Offset, UsableItemInfo.ActivationTime);
		// 010    UINT32 Unknown (added March 10th 2008)
		Offset += GetDWORD(pItemData + Offset, UsableItemInfo.Unknown);
	}

	return Offset;
}

void FFXiHelper::GetRaces(DWORD RacesBitMask, CString &Races)
{
	if ((RacesBitMask & FFXI_RACE_FLAG_ALL) == FFXI_RACE_FLAG_ALL)
	{
		Races = _T("All");
	}
	else if ((RacesBitMask & FFXI_RACE_FLAG_MALE) == FFXI_RACE_FLAG_MALE)
	{
		Races = _T("All \u2642");
	}
	else if ((RacesBitMask & FFXI_RACE_FLAG_FEMALE) == FFXI_RACE_FLAG_FEMALE)
	{
		Races = _T("All \u2640");
	}
	else
	{
		const FFXiStringAssoc *pStringTable = RacesStringTable;   

		while(pStringTable->Key != -1)
		{   
			if ((RacesBitMask & pStringTable->Key) == pStringTable->Key)
			{
				Races = pStringTable->pString;

				break;
			}

			pStringTable++;   
		}   
	}
}

void FFXiHelper::GetJobs(DWORD JobsBitMask, CString &Jobs)
{
	bool Empty = true;

	if ((JobsBitMask & FFXI_JOB_FLAG_ALL) == FFXI_JOB_FLAG_ALL)
	{
		Jobs = _T("All Jobs");
	}
	else
	{
		const FFXiStringAssoc *pStringTable = JobsStringTable;   
 
		while(pStringTable->Key != -1)
		{   
			if ((JobsBitMask & pStringTable->Key) == pStringTable->Key)
			{
				if (Empty == false)
					Jobs += '/';
				else
					Empty = false;

				Jobs += pStringTable->pString;
			}

			pStringTable++;   
		}   
	}
}

void FFXiHelper::GetSlot(DWORD SlotBitMask, CString &Slot)
{
	const FFXiStringAssoc *pStringTable = SlotStringTable;  

	while(pStringTable->Key != -1)
	{   
		if ((SlotBitMask & pStringTable->Key) == pStringTable->Key)
		{
			Slot =  pStringTable->pString;
			
			break;
		}

		pStringTable++;   
	}
}

void FFXiHelper::GetSkill(DWORD SkillBitMask, CString &Skill)
{
	const FFXiStringAssoc *pStringTable = SkillStringTable; 

	while(pStringTable->Key != -1)
	{   
		if (SkillBitMask == pStringTable->Key)
		{
			Skill =  pStringTable->pString;

			break;
		}

		pStringTable++;   
	}
}

void FFXiHelper::GetUsableItemType(DWORD ItemType, CString &Type)
{
	const FFXiStringAssoc *pStringTable = UsableItemStringTable; 

	while(pStringTable->Key != -1)
	{   
		if ((ItemType & pStringTable->Key) == pStringTable->Key)
		{
			Type =  pStringTable->pString;

			break;
		}

		pStringTable++;   
	}
}

void FFXiHelper::GetAttr(DWORD AttrBitMask, CString &Attr)
{
	if ((AttrBitMask & ITEM_FLAG_RARE_EX) == ITEM_FLAG_RARE_EX)
	{
		Attr = _T("Rare Ex");
	}
	else if ((AttrBitMask & ITEM_FLAG_EXCLUSIVE) == ITEM_FLAG_EXCLUSIVE)
	{
		Attr = _T("Ex");
	}
	else if ((AttrBitMask & ITEM_FLAG_RARE) == ITEM_FLAG_RARE)
	{
		Attr = _T("Rare");
	}
}

void FFXiHelper::GetArmorChargesInfo(FFXiArmorInfo ArmorInfo, CString &Remarks)
{
	if (ArmorInfo.MaxCharges != 0)
	{
		int Hours = 0, Minutes = 0;

		if (ArmorInfo.ReuseDelay >= 3600)
			Hours = ArmorInfo.ReuseDelay / 3600;
		else
			Minutes = ArmorInfo.ReuseDelay / 60;

		Remarks.Format(_T("<%d/%d 0:%d/[%02d:%02d:00, 0:%d]>"), ArmorInfo.MaxCharges, ArmorInfo.MaxCharges,
																ArmorInfo.UseDelay, Hours, Minutes, ArmorInfo.UseDelay);
	}
}

void FFXiHelper::GetWeaponChargesInfo(FFXiWeaponInfo WeaponInfo, CString &Remarks)
{
	if (WeaponInfo.MaxCharges != 0)
	{
		int Hours = 0, Minutes = 0;

		if (WeaponInfo.ReuseDelay >= 3600)
			Hours = WeaponInfo.ReuseDelay / 3600;
		else
			Minutes = WeaponInfo.ReuseDelay / 60;

		Remarks.Format(_T("<%d/%d 0:%d/[%02d:%02d:00, 0:%d]>"), WeaponInfo.MaxCharges, WeaponInfo.MaxCharges,
																WeaponInfo.UseDelay, Hours, Minutes, WeaponInfo.UseDelay);
	}
}

UINT FFXiHelper::GetItemFromID(DWORD ItemID, ItemArray *pMap, InventoryItem **pItem)
{
	InventoryItem *pCurrent;
	int ItemCount;

	if (pMap != NULL)
	{
		ItemCount = (int)pMap->GetCount();
		
		for (int i = 0; i < ItemCount; i++)
		{
			pCurrent = pMap->GetAt(i);

			if (pCurrent != NULL && pCurrent->ItemHdr.ItemID == ItemID)
			{
				*pItem = pCurrent;

				return ItemID;
			}
		}
		
	}

	return -1;
}

void FFXiHelper::GetIconInfo(BYTE *pIconData, FFXiIconInfo &IconInfo, DWORD BackgroundColor)
{
	size_t BufferSize = sizeof(FFXiIconInfo);
	RGBQUAD Background;
	int Index = 0;	

	if (pIconData != NULL)
	{
		Background = *((RGBQUAD*)&BackgroundColor);
		memcpy_s(&IconInfo, BufferSize, pIconData, BufferSize);

		while(IconInfo.ImageInfo.bmiColors[Index].rgbRed == 0 &&
			  IconInfo.ImageInfo.bmiColors[Index].rgbBlue == 0 &&
			  IconInfo.ImageInfo.bmiColors[Index].rgbGreen == 0 && Index < 64)
		{
			IconInfo.ImageInfo.bmiColors[Index] = Background;
			Index++;
		}
	}
}

void FFXiHelper::ClearItemData(InventoryItem *pItem)
{
	if (pItem != NULL)
	{
		pItem->RefCount = 0;
		pItem->hBitmap = NULL;
		pItem->LocationInfo.ImageIndex = 0;
		pItem->LocationInfo.ListIndex = 0;
		pItem->LocationInfo.InvTab = 0;
		pItem->LocationInfo.Character = 0;
		pItem->LocationInfo.Location.Empty();

		pItem->Attr.Empty();		
		pItem->Level.Empty();
		pItem->ItemName.Empty();
		pItem->ItemToolTip.Empty();
		pItem->LogName.Empty();
		pItem->LogName2.Empty();
		pItem->ItemDescription.Empty();
		pItem->Jobs.Empty();
		pItem->Races.Empty();
		pItem->Slot.Empty();
		pItem->Remarks.Empty();

		SecureZeroMemory(&pItem->ItemHdr, sizeof(pItem->ItemHdr));
		SecureZeroMemory(&pItem->WeaponInfo, sizeof(pItem->WeaponInfo));
		SecureZeroMemory(&pItem->IconInfo, sizeof(pItem->IconInfo));
	}
}