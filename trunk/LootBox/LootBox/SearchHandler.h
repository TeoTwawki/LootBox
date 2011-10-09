#ifndef __SEARCH_HANDLER_CLASS__
#define __SEARCH_HANDLER_CLASS__

typedef struct _SearchParams
{
	TCHAR *pSearchTerm;
	DWORD JobsBitMask;
	DWORD SlotBitMask;
	DWORD RacesBitMask;
	DWORD Skill;
	int MinLevel;
	int MaxLevel;
	int MinDelay;
	int MaxDelay;
	int MinDmg;
	int MaxDmg;
	int MinDef;
	int MaxDef;
	bool Rare;
	bool Exclusive;
} SearchParams;

typedef struct _SearchResult
{
	CString CharID;
	CString InvFile;
	int ListIndex;
} SearchResult;

typedef struct _SearchData
{
	SearchParams *pParams;
	ItemArray Items;
	bool Done;
} SearchData;

class SearchHandler
{
public:
	SearchHandler(SearchData *pData) { m_pSearchData = pData; }
	virtual ~SearchHandler() { m_pSearchData = NULL; }

	int ProcessSearchTerm(InventoryItem *pItem);
	int ProcessLevelRange(InventoryItem *pItem);
	int ProcessDelayRange(InventoryItem *pItem);
	int ProcessDmgRange(InventoryItem *pItem);
	int ProcessDefRange(InventoryItem *pItem);
	int ProcessJobs(InventoryItem *pItem);
	int ProcessSlot(InventoryItem *pItem);
	int ProcessSkill(InventoryItem *pItem);
	int ProcessRaces(InventoryItem *pItem);
	int ProcessRareEx(InventoryItem *pItem);

	void ProcessAll(InventoryItem *pItem);

	bool VerifyRange(int &Min, int &Max);
	bool CheckRange(int Min, int Max, int Value);
protected:
	SearchData *m_pSearchData;
};

#endif//__SEARCH_HANDLER_CLASS__