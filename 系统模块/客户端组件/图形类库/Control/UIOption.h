#ifndef __UIOPTION_H__
#define __UIOPTION_H__

#pragma once

/////////////////////////////////////////////////////////////////////////////////////
//

class FGUI_LIB_CLASS COptionUI : public CButtonUI
{
public:
	COptionUI();
	~COptionUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);

	bool Activate();
	void SetEnabled(bool bEnable = true);

	void SetStatusImage(LPCTSTR pStrImage, bool bFixed=true);

	LPCTSTR GetSelectedImage();
	void SetSelectedImage(LPCTSTR pStrImage);

	LPCTSTR GetSelectedRailImage();
	void SetSelectedRailImage(LPCTSTR pStrImage);

	void SetSelectedTextColor(DWORD dwTextColor);
	DWORD GetSelectedTextColor();

	void SetSelectedTextPadding(RECT rc);
	RECT GetSelectedTextPadding() const;

	void SetSelectedFont(int index);
	int GetSelectedFont() const;

	LPCTSTR GetForeImage();
	void SetForeImage(LPCTSTR pStrImage);

	LPCTSTR GetGroup() const;
	void SetGroup(LPCTSTR pStrGroupName = NULL);
	bool IsSelected() const;
	void Selected(bool bSelected);

	void SetPos(RECT rc);

	SIZE EstimateSize(SIZE szAvailable);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void PaintStatusImage(HDC hDC);
	void PaintText(HDC hDC);

	void DoInit();

	//¹¤³§º¯Êý
public:
	static COptionUI * Create(CPaintManagerUI * pManager,CControlUI * pParent,LPCTSTR pszName,bool bInit=true);

protected:
	bool		m_bSelected;
	CStdString	m_sGroupName;
	CStdString	m_sForeImage;
	DWORD		m_dwSelectedTextColor;
	RECT		m_rcSelectedTextPadding;
	CStdString	m_sSelectedImage;
	CStdString	m_sSelectedRailImage;
	SIZE		m_cSelectedRailXYPoint;
	int		    m_iSelectedFont;

};

#endif