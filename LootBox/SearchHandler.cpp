#include "stdafx.h"

#include "FFXIHelper.h"
#include "SearchHandler.h"

int SearchHandler::ProcessSearchTerm(InventoryItem *pItem)
{
	if (m_pSearchData != NULL && pItem != NULL &&
		m_pSearchData->pParams->pSearchTerm && 
		_tcslen(m_pSearchData->pParams->pSearchTerm) > 0)
	{
		CString lcString;

		// search in the item name
		lcString = pItem->ItemName;
		lcString = lcString.MakeLower();

		if (lcString.Find(m_pSearchData->pParams->pSearchTerm) != -1)
			return 1;
		// search in the item full name
		lcString = pItem->LogName;
		lcString = lcString.MakeLower();

		if (lcString.Find(m_pSearchData->pParams->pSearchTerm) != -1)
			return 1;
		// search in the item full name
		lcString = pItem->ItemDescription;
		lcString = lcString.MakeLower();

		if (lcString.Find(m_pSearchData->pParams->pSearchTerm) != -1)
			return 1;

		return 0;
	}

	return -1;
}

int SearchHandler::ProcessLevelRange(InventoryItem *pItem)
{
	if (m_pSearchData != NULL && pItem != NULL)
	{
		int MinLevel, MaxLevel;

		MinLevel = m_pSearchData->pParams->MinLevel;
		MaxLevel = m_pSearchData->pParams->MaxLevel;

		if (VerifyRange(MinLevel, MaxLevel))
		{
			if (pItem->ItemHdr.Type == ITEM_OBJECT_TYPE_ARMOR)
				return CheckRange(MinLevel, MaxLevel, pItem->ArmorInfo.Level) ? 1 : 0;
			else if (pItem->ItemHdr.Type == ITEM_OBJECT_TYPE_WEAPON)
				return CheckRange(MinLevel, MaxLevel, pItem->WeaponInfo.Level) ? 1 : 0;
			else
				return 0;
		}
	}

	return -1;
}

int SearchHandler::ProcessDelayRange(InventoryItem *pItem)
{
	if (m_pSearchData != NULL && pItem != NULL)
	{
		int MinDelay, MaxDelay;

		MinDelay = m_pSearchData->pParams->MinDelay;
		MaxDelay = m_pSearchData->pParams->MaxDelay;

		if (VerifyRange(MinDelay, MaxDelay))
			return CheckRange(MinDelay, MaxDelay, pItem->WeaponInfo.Delay) ? 1 : 0;
	}

	return -1;
}

int SearchHandler::ProcessDmgRange(InventoryItem *pItem)
{
	if (m_pSearchData != NULL && pItem != NULL)
	{
		int MinDmg, MaxDmg;

		MinDmg = m_pSearchData->pParams->MinDmg;
		MaxDmg = m_pSearchData->pParams->MaxDmg;

		if (VerifyRange(MinDmg, MaxDmg))
		{
			if (pItem->ItemHdr.Type == ITEM_OBJECT_TYPE_WEAPON)
				return CheckRange(MinDmg, MaxDmg, pItem->WeaponInfo.Damage) ? 1 : 0;
			else
				return 0;
		}
	}

	return -1;
}

int SearchHandler::ProcessDefRange(InventoryItem *pItem)
{
	if (m_pSearchData != NULL && pItem != NULL)
	{
		int MinDef, MaxDef;

		MinDef = m_pSearchData->pParams->MinDef;
		MaxDef = m_pSearchData->pParams->MaxDef;

		if (VerifyRange(MinDef, MaxDef))
		{
			if (pItem->ItemHdr.Type == ITEM_OBJECT_TYPE_ARMOR)
				return CheckRange(MinDef, MaxDef, pItem->ArmorInfo.Defense) ? 1 : 0;
			else
				return 0;
		}
	}

	return -1;
}

int SearchHandler::ProcessJobs(InventoryItem *pItem)
{
	if (m_pSearchData != NULL && pItem != NULL &&
		m_pSearchData->pParams->JobsBitMask != 0)
	{
		DWORD Mask = ~m_pSearchData->pParams->JobsBitMask;

		if (pItem->ItemHdr.Type == ITEM_OBJECT_TYPE_ARMOR)
			return ((pItem->ArmorInfo.Jobs | Mask) == Mask) ? 0 : 1;
		else if (pItem->ItemHdr.Type == ITEM_OBJECT_TYPE_WEAPON)
			return ((pItem->WeaponInfo.Jobs | Mask) == Mask) ? 0 : 1;

		return 0;
	}

	return -1;
}

int SearchHandler::ProcessSlot(InventoryItem *pItem)
{
	if (m_pSearchData != NULL && pItem != NULL &&
		m_pSearchData->pParams->SlotBitMask != 0)
	{
		DWORD Mask = m_pSearchData->pParams->SlotBitMask;

		if (pItem->ItemHdr.Type == ITEM_OBJECT_TYPE_ARMOR)
		{
			if ((pItem->ArmorInfo.Slot | Mask) == Mask)
				return (Mask != FFXI_SLOT_FLAG_MAIN) ? 1 : 0;
		}
		else if (pItem->ItemHdr.Type == ITEM_OBJECT_TYPE_WEAPON)
		{
			if ((pItem->WeaponInfo.Slot | Mask) == Mask)
				return (Mask != FFXI_SLOT_FLAG_SUB) ? 1 : 0;
		}

		return 0;
	}

	return -1;
}

int SearchHandler::ProcessSkill( InventoryItem *pItem )
{
	if (m_pSearchData != NULL && pItem != NULL &&
		m_pSearchData->pParams->Skill != 0)
	{
		DWORD Mask = m_pSearchData->pParams->Skill;

		if (pItem->ItemHdr.Type == ITEM_OBJECT_TYPE_WEAPON)
			return (pItem->WeaponInfo.Skill == Mask) ? 1 : 0;
		else
			return 0;
	}

	return -1;
}

int SearchHandler::ProcessRaces(InventoryItem *pItem)
{
	if (m_pSearchData != NULL && pItem != NULL &&
		m_pSearchData->pParams->RacesBitMask != 0)
	{
		DWORD Mask = m_pSearchData->pParams->RacesBitMask;

		if (pItem->ItemHdr.Type == ITEM_OBJECT_TYPE_ARMOR)
			return ((pItem->ArmorInfo.Races | Mask) == Mask) ? 1 : 0;
		else if (pItem->ItemHdr.Type == ITEM_OBJECT_TYPE_WEAPON)
			return ((pItem->WeaponInfo.Races | Mask) == Mask) ? 1 : 0;

		return 0;
	}

	return -1;
}

int SearchHandler::ProcessRareEx(InventoryItem *pItem)
{
	if (m_pSearchData != NULL && pItem != NULL)
	{
		DWORD Mask = 0;

		if (m_pSearchData->pParams->Rare && m_pSearchData->pParams->Exclusive)
			Mask = ITEM_FLAG_RARE_EX;
		else if (m_pSearchData->pParams->Rare)
			Mask = ITEM_FLAG_RARE;
		else if (m_pSearchData->pParams->Exclusive)
			Mask = ITEM_FLAG_EXCLUSIVE;

		return ((pItem->ItemHdr.Flags & Mask) == Mask);
	}

	return -1;
}

void SearchHandler::ProcessAll(InventoryItem *pItem)
{
	if (m_pSearchData != NULL)
	{
		bool Result = false;
		const int Size = 10;
		int Res[Size];

		SecureZeroMemory(&Res, Size);

		Res[0] = ProcessSearchTerm(pItem);
		Res[1] = ProcessLevelRange(pItem);
		Res[2] = ProcessDelayRange(pItem);
		Res[3] = ProcessDmgRange(pItem);
		Res[4] = ProcessDefRange(pItem);
		Res[5] = ProcessJobs(pItem);
		Res[6] = ProcessSlot(pItem);
		Res[7] = ProcessSkill(pItem);
		Res[8] = ProcessRaces(pItem);
		Res[9] = ProcessRareEx(pItem);
		
		for (int i = 0; i < Size; i++)
		{
			if (Res[i] == 1)
				Result = true;
			else if (Res[i] == 0)
				return;
		}

		if (Result)
			m_pSearchData->Items.Add(pItem);
	}
}

bool SearchHandler::VerifyRange(int &Min, int &Max)
{
	if (Min == 0 && Max == 0)
		return false;

	if (Max == 0)
		Max = Min;
	else if (Min == 0)
		Min = Max;
	else if (Min > Max)
	{
		int Temp = Max;

		Max = Min;
		Min = Temp;
	}

	return true;
}

bool SearchHandler::CheckRange(int Min, int Max, int Value)
{
	if (Min == Max)
		return (Value == Min);
	else
		return (Value >= Min && Value <= Max);
}
