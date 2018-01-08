#ifndef __UIRADIOBUTTON_H__
#define __UIRADIOBUTTON_H__

#pragma once

//单选按钮
class FGUI_LIB_CLASS CRadioButtonUI : public CLabelUI
{
public:
	CRadioButtonUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	UINT GetControlFlags() const;

	bool Activate();
	void SetEnabled(bool bEnable = true);
	void SetCheck(bool bChecked = true);
	bool GetCheck();
	void SetGrouped(bool bGrouped = true);
	bool GetGrouped();
	void SetGroupID(UINT nGroupID);
	UINT GetGroupID();
	void DoEvent(TEventUI& event);

	LPCTSTR GetNormalImage();
	void SetNormalImage(LPCTSTR pStrImage);
	LPCTSTR GetCheckedImage();
	void SetCheckedImage(LPCTSTR pStrImage);

	LPCTSTR GetNormalStatusImage();
	void SetNormalStatusImage(LPCTSTR pStrImage);
	LPCTSTR GetCheckedStatusImage();
	void SetCheckedStatusImage(LPCTSTR pStrImage);

	void SetAutoTextPadding(bool bAuto);
	bool GetAutoTextPadding();

	SIZE EstimateSize(SIZE szAvailable);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void PaintText(HDC hDC);
	void PaintStatusImage(HDC hDC);
	bool DrawImage(HDC hDC,RECT rcDest,LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);
	bool DrawImageEx(HDC hDC, const RECT& rcDest, LPCTSTR pStrImage, const RECT& rcBmpPart, BYTE uFade = 255);

	//工厂函数
public:
	static CRadioButtonUI * Create(CPaintManagerUI * pManager,CControlUI * pParent,LPCTSTR pszName,bool bInit=true);

protected:
	bool m_bChecked;
	bool m_bGrouped;
	UINT m_nGroupID;
	UINT m_uButtonState;
	UINT m_nCheckWidth;
	UINT m_nCheckHight;
	bool m_bAutoTextPadding;

	CStdString m_sNormalImage;
	CStdString m_sCheckedImage;
	CStdString m_sNormalStatusImage;
	CStdString m_sCheckedStatusImage;
};

#endif