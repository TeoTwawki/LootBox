#ifndef __FFXI_HELPER_CLASS__
#define __FFXI_HELPER_CLASS__

//#define _DUMP_DAT_FILES

#define FFXI_REGISTRY_KEY_INSTALL_JP	_T("SOFTWARE\\PlayOnline\\InstallFolder")
#define FFXI_REGISTRY_KEY_INSTALL_US	_T("SOFTWARE\\PlayOnlineUS\\InstallFolder")
#define FFXI_REGISTRY_KEY_INSTALL_EU	_T("SOFTWARE\\PlayOnlineEU\\InstallFolder")
#define REGISTRY_KEY_FFXI_KEYNAME		_T("0001")
#define FFXI_PATH_USER_DATA				_T("USER")
#define FFXI_ITEM_STRING_TABLE_PADDING	(7 * sizeof(DWORD))
#define OFFSET_FILEHEADER	0x02B0
#define DATA_SIZE_INVENTORY 0x0288
#define DATA_SIZE_ITEM_INFO	0x0200
#define DATA_SIZE_ITEM_ICON 0x0A00
#define DATA_SIZE_ITEM		0x0C00
#define RSHIFT_DECRYPT_ITEM 0x0005

#pragma pack(push, 1)

typedef struct _FFXiStringAssoc
{
	_FFXiStringAssoc(int _Key, TCHAR *_pString): Key(_Key), pString(_pString){}
	int Key;
	TCHAR *pString;
} FFXiStringAssoc;

typedef struct _PalettedBitmapInfo
{
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD bmiColors[64];
	DWORD PaddingData[192];
	DWORD ImageData[256];
} PalettedBitmapInfo;

typedef struct _FFXiIconInfo
{
	BYTE ImageType;
	char Category[8];
	char Name[8];
	PalettedBitmapInfo ImageInfo;
} FFXiIconInfo;

/*
000    UINT32 ID
004    UINT16 Flags
006    UINT16 Stack Size
008    UINT16 Type
00A    UINT16 ResourceID
00C    UINT16 ValidTargets
*/
typedef struct _FFXiItemHeader
{
	DWORD	ItemID;
	WORD	Flags;
	WORD	StackSize;
	WORD	Type;
	WORD	ResourceID;
	WORD	ValidTargets;
} FFXiItemHeader;

/*
Offset Type   Description
------ ------ --------------
00E    UINT16 Level
010    UINT16 Slots
012    UINT16 Races
014    UINT32 Jobs
018    UINT16 Shield Size
01A    UINT8  Max Charges
01B    UINT8  Casting Time
01C    UINT16 Use Delay
01E    UINT16 Unknown
020    UINT32 Re-Use Delay
024    UINT32 Unknown (added March 10th 2008) 
*/
typedef struct _FFXiArmorInfo
{
	WORD	Level;
	WORD	Slot;
	WORD	Races;
	DWORD	Jobs;
	WORD	ShieldSize;
	BYTE	MaxCharges;
	BYTE	CastingTime;
	WORD	UseDelay;
	WORD	Defense;
	DWORD	ReuseDelay;
	DWORD	Unknown2;
} FFXiArmorInfo;

/*
Offset Type   Description
------ ------ --------------
00E    UINT16 Element
010    UINT32 Storage Slots 
*/
typedef struct _FFXiObjectInfo
{
	WORD	Element;
	DWORD	Storage;
} FFXiObjectInfo;

/*
Offset Type   Description
------ ------ --------------
00E    UINT16 Puppet Slot
010    UINT32 Element Charge
014    UINT32 Unknown 
*/
typedef struct _FFXiPuppetInfo
{
	WORD	Slot;
	DWORD	Element;
	DWORD	Unknown;
} FFXiPuppetInfo;

/*
Offset Type   Description
------ ------ --------------
00E    UINT16 Activation Time
010    UINT32 Unknown (added March 10th 2008) 
*/
typedef struct _FFXiUsableItemInfo
{
	WORD	ActivationTime;
	DWORD	Unknown;
} FFXiUsableItemInfo;

/*
Offset Type   Description
------ ------ --------------
00E    UINT16 Level
010    UINT16 Slots
012    UINT16 Races
014    UINT32 Jobs
018    UINT16 Damage
01A    UINT16 Delay
01C    UINT16 DPS
01E    UINT8  Skill
01F    UINT8  Jug Size
020    UINT32 Unknown
024    UINT8  Max Charges
025    UINT8  Casting Time
026    UINT16 Use Delay
028    UINT32 Re-Use Delay
02C    UINT32 Unknown (added March 10th 2008) 
*/
typedef struct _FFXiWeaponInfo
{
	WORD	Level;
	WORD	Slot;
	WORD	Races;
	DWORD	Jobs;
	WORD	Damage;
	WORD	Delay;
	WORD	DPS;
	BYTE	Skill;
	BYTE	JugSize;
	DWORD	Unknown1;
	BYTE	MaxCharges;
	BYTE	CastingTime;
	WORD	UseDelay;
	DWORD	ReuseDelay;
	DWORD	Unknown2;
} FFXiWeaponInfo;

#pragma pack(pop)

typedef struct _ItemLocationInfo
{
	int InvTab;
	int Character;
	int ListIndex;
	int ImageIndex;
	CString Location;	
} ItemLocationInfo;

typedef struct _InventoryItem
{
	int RefCount;
	HBITMAP hBitmap;
	CString ItemName;
	CString LogName;
	CString LogName2;
	CString ItemToolTip;
	CString Attr;
	CString ItemDescription;
	CString Slot;
	CString Races;
	CString Level;
	CString Jobs;
	CString Remarks;
	FFXiItemHeader ItemHdr;
	FFXiIconInfo IconInfo;
	ItemLocationInfo LocationInfo;
	union
	{
		FFXiWeaponInfo WeaponInfo;
		FFXiArmorInfo ArmorInfo;
		FFXiObjectInfo ObjectInfo;
		FFXiUsableItemInfo UsableItemInfo;
		FFXiPuppetInfo PuppetInfo;
	};
} InventoryItem;

/*
00 - Nothing
01 - Item
02 - Quest Item
03 - Fish
04 - Weapon
05 - Armor
06 - Linkshell
07 - Usable Item
08 - Crystal
09 - Unknown
10 - Furnishing
11 - Plant
12 - Flowerpot
13 - Puppet Item
14 - Mannequin
15 - Book 	
*/
enum FFXI_ITEM_OBJECT_TYPE
{
	ITEM_OBJECT_TYPE_NOTHING = 0,
	ITEM_OBJECT_TYPE_ITEM,
	ITEM_OBJECT_TYPE_QUEST_ITEM,
	ITEM_OBJECT_TYPE_FISH,
	ITEM_OBJECT_TYPE_WEAPON,
	ITEM_OBJECT_TYPE_ARMOR,
	ITEM_OBJECT_TYPE_LINKSHELL,
	ITEM_OBJECT_TYPE_USABLE_ITEM,
	ITEM_OBJECT_TYPE_CRYSTAL,
	ITEM_OBJECT_TYPE_UNKNOWN,
	ITEM_OBJECT_TYPE_FURNISHING,
	ITEM_OBJECT_TYPE_PLANT,
	ITEM_OBJECT_TYPE_FLOWERPOT,
	ITEM_OBJECT_TYPE_PUPPET_ITEM,
	ITEM_OBJECT_TYPE_MANNEQUIN,
	ITEM_OBJECT_TYPE_BOOK,
	ITEM_OBJECT_TYPE_COUNT
};

/*
0000 - 0FFF  Objects
1000 - 1FFF  Usable Item
2000 - 2BFF  Puppet Item
2C00 - 3FFF  Armor
4000 - 6FFF  Weapons 
*/
enum FFXI_ITEM_TYPE
{
	ITEM_TYPE_OBJECTS		= 0,
	ITEM_TYPE_USABLE_ITEM,
	ITEM_TYPE_PUPPET_ITEM,
	ITEM_TYPE_ARMOR,
	ITEM_TYPE_WEAPON,
	ITEM_TYPE_COUNT
};

/*
0x0001 = ?
0x0002 = ?
0x0004 = ?
0x0008 = ?
0x0010 = ?
0x0020 = Can be synthesized using guild crystals (i.e. can be inscribed)
0x0040 = Cannot be sold at the AH
0x0080 = Is a scroll
0x0100 = Is a linkshell
0x0200 = Can be used
0x0400 = Can be traded to an NPC
0x0800 = Can be equipped
0x1000 = Cannot be sold to NPC
0x2000 = Cannot be sent to mog house
0x4000 = Cannot be traded to a player (includes bazaar)
0x8000 = Rare 
*/
enum FFXI_ITEM_FLAGS
{
	ITEM_FLAG_UNKNOWN1		= 0x0001,
	ITEM_FLAG_UNKNOWN2		= 0x0002,
	ITEM_FLAG_UNKNOWN3		= 0x0004,
	ITEM_FLAG_UNKNOWN4		= 0x0008,
	ITEM_FLAG_UNKNOWN5		= 0x0010,
	ITEM_FLAG_SIGNABLE		= 0x0020,
	ITEM_FLAG_NO_AUCTION	= 0x0040,
	ITEM_FLAG_SCROLL		= 0x0080,
	ITEM_FLAG_LINKSHELL		= 0x0100,
	ITEM_FLAG_USABLE		= 0x0200,
	ITEM_FLAG_NPC_TRADE		= 0x0400,
	ITEM_FLAG_EQUIPPABLE	= 0x0800,
	ITEM_FLAG_NO_NPC_SALE	= 0x1000,
	ITEM_FLAG_NO_DELIVERY	= 0x2000,
	ITEM_FLAG_NO_TRADE		= 0x4000,
	ITEM_FLAG_RARE			= 0x8000,
	ITEM_FLAG_EXCLUSIVE		= ITEM_FLAG_NO_AUCTION | ITEM_FLAG_NO_DELIVERY | ITEM_FLAG_NO_TRADE,
	ITEM_FLAG_RARE_EX		= ITEM_FLAG_EXCLUSIVE | ITEM_FLAG_RARE,
};

/*
0x0001 = Self
0x0002 = Player
0x0004 = Party Member
0x0008 = Alliance Member
0x0010 = NPC
0x0020 = Enemy
0x0040 = ?
0x0080 = Corpse
*/
enum FFXI_TARGET_FLAGS
{
	TARGET_FLAG_SELF		= 0x0001,
	TARGET_FLAG_PLAYER		= 0x0002,
	TARGET_FLAG_PARTY		= 0x0004,
	TARGET_FLAG_ALLIANCE	= 0x0008,
	TARGET_FLAG_NPC			= 0x0010,
	TARGET_FLAG_ENEMY		= 0x0020,
	TARGET_FLAG_UNKNOWN		= 0x0040,
	TARGET_FLAG_CORPSE		= 0x0080,
};

/*
	0x00 - None (set on beastmaster jugs)
	0xFF - Special (set on pet food)
	// Melee Skills
	0x01 - Hand-To-Hand
	0x02 - Dagger
	0x03 - Sword
	0x04 - Great Sword
	0x05 - Axe
	0x06 - Great Axe
	0x07 - Scythe
	0x08 - PoleArm
	0x09 - Katana
	0x0A - Great Katana
	0x0B - Club
	0x0C - Staff
	// Ranged Skills
	0x19 - Ranged
	0x1A - Marksmanship
	0x1B - Thrown
	// Magic Skills
	0x20 - Divine Magic
	0x21 - Healing Magic
	0x22 - Enhancing Magic
	0x23 - Enfeebling Magic
	0x24 - Elemental Magic
	0x25 - Dark magic
	0x26 - Summoning Magic
	0x27 - Ninjutsu
	0x28 - Singing
	0x29 - String Instrument
	0x2A - Wind Instrument
	// Crafts
	0x30 - Fishing 
*/

enum FFXI_SKILL_TYPE
{
	FFXI_SKILL_TYPE_BST_JUG				= 0x00,
	FFXI_SKILL_TYPE_H2H					= 0x01,
	FFXI_SKILL_TYPE_DAGGER				= 0x02,
	FFXI_SKILL_TYPE_SWORD				= 0x03,
	FFXI_SKILL_TYPE_GREAT_SWORD			= 0x04,
	FFXI_SKILL_TYPE_AXE					= 0x05,
	FFXI_SKILL_TYPE_GREAT_AXE			= 0x06,
	FFXI_SKILL_TYPE_SCYTHE				= 0x07,
	FFXI_SKILL_TYPE_POLEARM				= 0x08,
	FFXI_SKILL_TYPE_KATANA				= 0x09,
	FFXI_SKILL_TYPE_GREAT_KATANA		= 0x0A,
	FFXI_SKILL_TYPE_CLUB				= 0x0B,
	FFXI_SKILL_TYPE_STAFF				= 0x0C,
	FFXI_SKILL_TYPE_RANGED				= 0x19,
	FFXI_SKILL_TYPE_MARKSMANSHIP		= 0x1A,
	FFXI_SKILL_TYPE_THROWING			= 0x1B,
	FFXI_SKILL_TYPE_DIVINE_MAGIC		= 0x20,
	FFXI_SKILL_TYPE_HEALING_MAGIC		= 0x21,
	FFXI_SKILL_TYPE_ENHANCING_MAGIC		= 0x22,
	FFXI_SKILL_TYPE_ENFEEBLING_MAGIC	= 0x23,
	FFXI_SKILL_TYPE_ELEMENTAL_MAGIC		= 0x24,
	FFXI_SKILL_TYPE_DARK_MAGIC			= 0x25,
	FFXI_SKILL_TYPE_SUMMONING_MAGIC		= 0x26,
	FFXI_SKILL_TYPE_NINJUTSU			= 0x27,
	FFXI_SKILL_TYPE_SINGING				= 0x28,
	FFXI_SKILL_TYPE_STRING_INSTRUMENT	= 0x29,
	FFXI_SKILL_TYPE_WIND_INSTRUMENT		= 0x2A,
	FFXI_SKILL_TYPE_FISHING				= 0x30,
	FFXI_SKILL_TYPE_PET_FOOD			= 0xFF,
};

/*
0x0001 - Weapon ("main")
0x0002 - Shield / Offhand Weapon ("sub")
0x0004 - Ranged Weapon ("range")
0x0008 - Ammo / Thrown Weapon ("ammo")
0x0010 - Head ("head")
0x0020 - Body ("body")
0x0040 - Hands ("hands")
0x0080 - Legs ("legs")
0x0100 - Feet ("feet")
0x0200 - Neck ("neck")
0x0400 - Waist ("waist")
0x0800 - Left Ear ("l.ear")
0x1000 - Right Ear ("r.ear")
0x2000 - Left Ring ("l.ring")
0x4000 - Right Ring ("r.ring")
0x8000 - Cloak / Cape ("back") 
*/
enum FFXI_SLOT_FLAG
{
	FFXI_SLOT_FLAG_MAIN		= 0x0001,
	FFXI_SLOT_FLAG_SUB		= 0x0002,
	FFXI_SLOT_FLAG_RANGE	= 0x0004,
	FFXI_SLOT_FLAG_AMMO		= 0x0008,
	FFXI_SLOT_FLAG_HEAD		= 0x0010,
	FFXI_SLOT_FLAG_BODY		= 0x0020,
	FFXI_SLOT_FLAG_HANDS	= 0x0040,
	FFXI_SLOT_FLAG_LEGS		= 0x0080,
	FFXI_SLOT_FLAG_FEET		= 0x0100,
	FFXI_SLOT_FLAG_NECK		= 0x0200,
	FFXI_SLOT_FLAG_WAIST	= 0x0400,
	FFXI_SLOT_FLAG_L_EAR	= 0x0800,
	FFXI_SLOT_FLAG_R_EAR	= 0x1000,
	FFXI_SLOT_FLAG_L_RING	= 0x2000,
	FFXI_SLOT_FLAG_R_RING	= 0x4000,
	FFXI_SLOT_FLAG_BACK		= 0x8000,
};

// WAR|MNK|WHM|BLM|RDM|THF|PLD|DRK|BST|BRD|RNG|SAM|NIN|DRG|SMN|BLU|COR|PUP|DNC|SCH|All Jobs

enum FFXI_JOB_FLAG
{
	FFXI_JOB_FLAG_WAR		= 0x00000002,
	FFXI_JOB_FLAG_MNK		= 0x00000004,
	FFXI_JOB_FLAG_WHM		= 0x00000008,
	FFXI_JOB_FLAG_BLM		= 0x00000010,
	FFXI_JOB_FLAG_RDM		= 0x00000020,
	FFXI_JOB_FLAG_THF		= 0x00000040,
	FFXI_JOB_FLAG_PLD		= 0x00000080,
	FFXI_JOB_FLAG_DRK		= 0x00000100,
	FFXI_JOB_FLAG_BST		= 0x00000200,
	FFXI_JOB_FLAG_BRD		= 0x00000400,
	FFXI_JOB_FLAG_RNG		= 0x00000800,
	FFXI_JOB_FLAG_SAM		= 0x00001000,
	FFXI_JOB_FLAG_NIN		= 0x00002000,
	FFXI_JOB_FLAG_DRG		= 0x00004000,
	FFXI_JOB_FLAG_SMN		= 0x00008000,
	FFXI_JOB_FLAG_BLU		= 0x00010000,
	FFXI_JOB_FLAG_COR		= 0x00020000,
	FFXI_JOB_FLAG_PUP		= 0x00040000,
	FFXI_JOB_FLAG_DNC		= 0x00080000,
	FFXI_JOB_FLAG_SCH		= 0x00100000,
	FFXI_JOB_FLAG_RUN		= 0x00200000,
	FFXI_JOB_FLAG_GEO		= 0x00400000,
/*
	FFXI_JOB_FLAG_FJ4		= 0x00800000,
	FFXI_JOB_FLAG_FJ5		= 0x01000000,
	FFXI_JOB_FLAG_FJ6		= 0x02000000,
	FFXI_JOB_FLAG_FJ7		= 0x04000000,
	FFXI_JOB_FLAG_FJ8		= 0x08000000,
	FFXI_JOB_FLAG_FJ9		= 0x10000000,
	FFXI_JOB_FLAG_FJ10		= 0x20000000,
	FFXI_JOB_FLAG_FJ11		= 0x40000000,
	FFXI_JOB_FLAG_FJ12		= 0x80000000,
*/
	FFXI_JOB_FLAG_ALL		= 0x001FFFFE,
};

// All Races|All M|All F|H|HM|HF|E|EM|EF|T|TM|TF|M|G
enum FFXI_RACE_FLAG
{
	FFXI_RACE_FLAG_HUME_MALE		= 0x002,
	FFXI_RACE_FLAG_HUME_FEMALE		= 0x004,
	FFXI_RACE_FLAG_ELVAAN_MALE		= 0x008,
	FFXI_RACE_FLAG_ELVAAN_FEMALE	= 0x010,
	FFXI_RACE_FLAG_TARUTARU_MALE	= 0x020,
	FFXI_RACE_FLAG_TARUTARU_FEMALE	= 0x040,
	FFXI_RACE_FLAG_MITHRA			= 0x080,
	FFXI_RACE_FLAG_GALKA			= 0x100,
	FFXI_RACE_FLAG_MALE				= FFXI_RACE_FLAG_HUME_MALE | FFXI_RACE_FLAG_ELVAAN_MALE | FFXI_RACE_FLAG_TARUTARU_MALE | FFXI_RACE_FLAG_GALKA,
	FFXI_RACE_FLAG_FEMALE			= FFXI_RACE_FLAG_HUME_FEMALE | FFXI_RACE_FLAG_ELVAAN_FEMALE | FFXI_RACE_FLAG_TARUTARU_FEMALE | FFXI_RACE_FLAG_MITHRA,
	FFXI_RACE_FLAG_ALL				= 0x1FE,
};

enum FFXI_REGION_FLAG
{
	FFXI_REGION_FLAG_JP = 0x01,
	FFXI_REGION_FLAG_US = 0x02,
	FFXI_REGION_FLAG_EU = 0x04
};

enum FFXI_REGION
{
	FFXI_REGION_JP = 1,
	FFXI_REGION_US,
	FFXI_REGION_EU,
	FFXI_REGION_COUNT
};

enum FFXI_LANG
{
	FFXI_LANG_JP = FFXI_REGION_JP,
	FFXI_LANG_US = FFXI_REGION_US,
	FFXI_LANG_FR,
	FFXI_LANG_DE,
	FFXI_LANG_COUNT
};

/*
	USERS					// Root
	|
	|_ <user_1>				// CharacterMap
	|	|
	|	|_ <file_1.dat>		// InventoryMap
	|	|	|
	|	|	|_ <item_1>		// ItemMap
	|	|	|_ ...
	|	|	|_ <item_n>
	|	|_ ...
	|	|_ <file_n.dat>
	|_ ...
	|_ <user_n>
	|
	|_ <file_1.dat>		// InventoryMap
	|	|
	|	|_ <item_1>		// ItemMap
	|	|_ ...
	|	|_ <item_n>
	|_ ...
	|_ <file_n.dat>
*/
typedef CMap<int, int, InventoryItem*, InventoryItem*> ItemArray;
typedef CMap<int, int, ItemArray*, ItemArray*> InventoryMap;
typedef CMap<int, int, InventoryMap*, InventoryMap*> CharacterMap;

class FFXiHelper
{
public:
	FFXiHelper(int Region = FFXI_REGION_US){ m_Region = Region; }
	~FFXiHelper(){}

	int DetectGameRegion(int &Regions);
	void SetRegion(int Region) { m_Region = Region; }
	TCHAR* GetInstallPath(int Region = FFXI_REGION_US);
	void SetInstallPath(const TCHAR *pInstallPath) { m_InstallFolder = pInstallPath; }

	bool ReadInventoryFile(const TCHAR *pPlayerID, int InvType, ItemArray &ItemMap);
	int GetItemHdr(const BYTE *pItemData, FFXiItemHeader &ItemHdr);
	int GetArmorInfo(const BYTE *pItemData, FFXiArmorInfo &ArmorInfo);
	int GetObjectInfo(const BYTE *pItemData, FFXiObjectInfo &ObjectInfo);
	int GetPuppetInfo(const BYTE *pItemData, FFXiPuppetInfo &PuppetInfo);
	int GetWeaponInfo(const BYTE *pItemData, FFXiWeaponInfo &WeaponInfo);
	int GetUsableItemInfo(const BYTE *pItemData, FFXiUsableItemInfo &UsableItemInfo);
	void GetFileFromItemID(DWORD &ItemID, CString &DATFile, int Language = FFXI_LANG_US, bool bRelative = false);
	void GetFileFromType(int Type, CString &DATFile, int Language = FFXI_LANG_US, bool bRelative = false);
	void GetItemInfo(const BYTE *pStringTable, int Language, CString &ItemName, CString &LogName,
					 CString &LogName2, CString &ItemDescription, bool NoConversion = false);
	void GetJobs(DWORD JobsBitMask, CString &Jobs);
	void GetSlot(DWORD SlotBitMask, CString &Slot);
	void GetSkill(DWORD SkillBitMask, CString &Skill);
	void GetUsableItemType(DWORD ItemType, CString &Type);
	void GetRaces(DWORD RacesBitMask, CString &Races);
	void GetAttr(DWORD AttrBitMask, CString &Attr);
	void GetIconInfo(BYTE *pIconData, FFXiIconInfo &IconInfo, DWORD BackgroundColor = 0x00FFFFFF);
	void GetArmorChargesInfo(FFXiArmorInfo ArmorInfo, CString &Remarks);
	void GetWeaponChargesInfo(FFXiWeaponInfo WeaponInfo, CString &Remarks);
	void GetScrollInfoFromDesc(const CString &Description, CString &Jobs);
	void GetDefenseFromDesc(int Language, const CString &Description, WORD &Defense);
	bool ParseInventoryFile(const TCHAR* pFile, const ItemLocationInfo &LocationInfo,
							ItemArray *pMap, int Language = FFXI_LANG_US, bool Update = false);
	bool AddItem(int ItemIndex, ItemArray &Map, const InventoryItem *pItem);
	UINT GetItemFromID(DWORD ItemID, ItemArray *pMap, InventoryItem **pItem);
	void ClearItemData(InventoryItem *pItem);
	bool ReadItem(BYTE *pItemData, InventoryItem *pItem, int Language = FFXI_LANG_US, bool NoConversion = false);

	static void GetBYTE(BYTE **pData, BYTE &Result, bool MovePtr = true);
	static void GetWORD(BYTE **pData, WORD &Result, bool MovePtr = true);
	static void GetDWORD(BYTE **pData, DWORD &Result, bool MovePtr = true);

	static int GetBYTE(const BYTE *pData, BYTE &Result);
	static int GetWORD(const BYTE *pData, WORD &Result);
	static int GetDWORD(const BYTE *pData, DWORD &Result);

	static const BYTE *GetStringOffset(const BYTE *pStringTable, int Offset);

	static void ConvertChars(const BYTE *pData, CString &Text, bool ucWord = false);
	static void UpperCaseWord(CString &String);

	static void RotateBits(const BYTE *pData_in ,BYTE *pData_out, int DataSize_in, int BitShift_in);
	static inline BYTE RotateLeft(BYTE Value_in, int BitShift_in);
	static inline BYTE RotateRight(BYTE Value_in, int BitShift_in);

	const static FFXiStringAssoc UsableItemStringTable[];
	const static FFXiStringAssoc RacesStringTable[];	
	const static FFXiStringAssoc SkillStringTable[];
	const static FFXiStringAssoc JobsStringTable[];
	const static FFXiStringAssoc SlotStringTable[];

protected:
	CString m_InstallFolder;
	int m_Region;
};

#endif//__FFXI_HELPER_CLASS__