#ifndef __UITABBUTTON_H__
#define __UITABBUTTON_H__

#pragma once


//选择按钮
class FGUI_LIB_CLASS CTabButtonUI : public CButtonUI
{
public:
	CTabButtonUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	UINT GetControlFlags() const;

	void SetCheck(bool bChecked = true);
	bool GetCheck();
	void SetGrouped(bool bGrouped = true);
	bool GetGrouped();
	void SetGroupID(UINT nGroupID);
	UINT GetGroupID();
	void DoEvent(TEventUI& event);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void PaintText(HDC hDC);
	void PaintStatusImage(HDC hDC);
	bool DrawImage(HDC hDC,RECT rcDest,LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);	

	//工厂函数
public:
	static CTabButtonUI * Create(CPaintManagerUI * pManager,CControlUI * pParent,LPCTSTR pszName,bool bInit=true);

protected:
	bool m_bChecked;
	bool m_bGrouped;
	UINT m_nGroupID;
	UINT m_uButtonState;
};

#endif