#pragma once


// RegionSelect dialog

class RegionSelect : public CDialog
{
	DECLARE_DYNAMIC(RegionSelect)

public:
	RegionSelect(int Regions, CWnd* pParent = NULL);   // standard constructor
	virtual ~RegionSelect();

	int GetSelectedRegion();

// Dialog Data
	enum { IDD = IDD_REGION_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	int m_Regions;
	int m_Region;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChangeRegionCombo();
};
