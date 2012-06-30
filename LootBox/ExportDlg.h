#pragma once

class FFXiHelper;

enum eExportColumns
{
	EXPORT_NAME	 = 0x00000001,
	EXPORT_ATTR	 = 0x00000002,
	EXPORT_DESC	 = 0x00000004,
	EXPORT_TYPE	 = 0x00000008,
	EXPORT_RACES = 0x00000010,
	EXPORT_LEVEL = 0x00000020,
	EXPORT_JOBS	 = 0x00000040,
	EXPORT_RMKS	 = 0x00000080
};

class ExportDialog : public CDialog
{
	enum { IDD = IDD_EXPORT };
public:
	ExportDialog(FFXiHelper *pHelper, const CArray<CString, LPCTSTR> &Characters,
				 CSimpleIni *pIni_in, CWnd* pParent = NULL);

	const CArray<bool, bool>& GetExportedChars() { return m_ExportedChars; }
	int GetExportedCharsCount() const { return m_ExportedCharsCount; }
	int GetColumnCount() const { return m_ColumnCount; }
	DWORD_PTR GetBitMask() const { return m_BitMask; }

protected:
	const CArray<CString, LPCTSTR> &m_Characters;
	CArray<bool, bool> m_ExportedChars;
	int m_ExportedCharsCount;
	FFXiHelper *m_pHelper;
	DWORD_PTR m_BitMask;
	CSimpleIni *m_pIni;	
	int m_ColumnCount;	

	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};