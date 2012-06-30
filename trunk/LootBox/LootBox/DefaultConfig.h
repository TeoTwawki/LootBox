#ifndef __DEFAULT_CONFIG_H__
#define __DEFAULT_CONFIG_H__

#define LIST_ICON_SIZE					16

#define INI_FILE_FILENAME				_T("loot.ini")

#define INI_FILE_CHARACTERS_SECTION		_T("Characters")
#define INI_FILE_INVENTORY_SECTION		_T("Inventory")
#define INI_FILE_COLUMNS_SECTION		_T("Columns")
#define INI_FILE_EXPORT_SECTION			_T("Export")
#define INI_FILE_CONFIG_SECTION			_T("Config")

#define INI_FILE_COL_NAME_KEY			_T("Name")
#define INI_FILE_COL_ATTR_KEY			_T("Attr")
#define INI_FILE_COL_DESC_KEY			_T("Description")
#define INI_FILE_COL_TYPE_KEY			_T("Type")
#define INI_FILE_COL_RACES_KEY			_T("Races")
#define INI_FILE_COL_LEVEL_KEY			_T("Level")
#define INI_FILE_COL_JOBS_KEY			_T("Jobs")
#define INI_FILE_COL_REMARKS_KEY		_T("Remarks")

#define INI_FILE_COL_NAME_VALUE			_T("150")
#define INI_FILE_COL_ATTR_VALUE			_T("50")
#define INI_FILE_COL_DESC_VALUE			_T("250")
#define INI_FILE_COL_TYPE_VALUE			_T("50")
#define INI_FILE_COL_RACES_VALUE		_T("50")
#define INI_FILE_COL_LEVEL_VALUE		_T("50")
#define INI_FILE_COL_JOBS_VALUE			_T("100")
#define INI_FILE_COL_REMARKS_VALUE		_T("100")

#define INI_FILE_INVENTORY_VALUE		_T("Inventory")
#define INI_FILE_MOG_SAFE_VALUE			_T("Mog Safe")
#define INI_FILE_STORAGE_VALUE			_T("Storage")
#define INI_FILE_MOG_LOCKER_VALUE		_T("Mog Locker")
#define INI_FILE_MOG_SATCHEL_VALUE		_T("Mog Satchel")
#define INI_FILE_MOG_SACK_VALUE			_T("Mog Sack")

#define INI_FILE_INVENTORY_KEY			_T("is.dat")
#define INI_FILE_MOG_SAFE_KEY			_T("bs.dat")
#define INI_FILE_STORAGE_KEY			_T("cl.dat")
#define INI_FILE_MOG_LOCKER_KEY			_T("mb.dat")
#define INI_FILE_MOG_SATCHEL_KEY		_T("sb.dat")
#define INI_FILE_MOG_SACK_KEY			_T("sk.dat")

#define INI_FILE_WINDOW_WIDTH_KEY		_T("Width")
#define INI_FILE_WINDOW_HEIGHT_KEY		_T("Height")
#define INI_FILE_LAST_CHARACTER_KEY		_T("SelectedChar")
#define INI_FILE_LAST_TAB_KEY			_T("SelectedTab")
#define INI_FILE_GAME_REGION_KEY		_T("Region")
#define INI_FILE_LANGUAGE_KEY			_T("Language")
#define INI_FILE_COMPACT_LIST_KEY		_T("CompactList")
#define INI_FILE_GAME_DIRECTORY			_T("FFXiPath")

#define INI_FILE_WINDOW_WIDTH_VALUE		600L
#define INI_FILE_WINDOW_HEIGHT_VALUE	400L
#define INI_FILE_LAST_CHARACTER_VALUE	0L
#define INI_FILE_LAST_TAB_VALUE			0L
#define INI_FILE_GAME_REGION_VALUE		FFXI_REGION_US
#define INI_FILE_LANGUAGE_VALUE			FFXI_LANG_US
#define INI_FILE_COMPACT_LIST_VALUE		false

#define INI_FILE_GAME_REGION_COMMENT	_T(";1 = JP | 2 = US | 3 = EU")
#define INI_FILE_LANGUAGE_COMMENT		_T(";1 = JP | 2 = US | 3 = FR | 4 = DE")

#endif//__DEFAULT_CONFIG_H__