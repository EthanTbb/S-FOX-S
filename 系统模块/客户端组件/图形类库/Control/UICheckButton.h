#ifndef __UICHECKBUTTON_H__
#define __UICHECKBUTTON_H__

#pragma once

//复选控件
class FGUI_LIB_CLASS CCheckButtonUI : public CLabelUI
{
public:
	CCheckButtonUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	UINT GetControlFlags() const;

	bool Activate();
	void SetEnabled(bool bEnable = true);
	void SetCheck(bool bChecked = true);
	bool GetCheck();
	void DoEvent(TEventUI& event);

	LPCTSTR GetNormalImage();
	void SetNormalImage(LPCTSTR pStrImage);
	LPCTSTR GetCheckedImage();
	void SetCheckedImage(LPCTSTR pStrImage);

	LPCTSTR GetStatusNormalImage();
	void SetStatusNormalImage(LPCTSTR pStrImage);
	LPCTSTR GetStatusCheckedImage();
	void SetStatusCheckedImage(LPCTSTR pStrImage);

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
	static CCheckButtonUI * Create(CPaintManagerUI * pManager,CControlUI * pParent,LPCTSTR pszName,bool bInit=true);

protected:
	bool m_bChecked;
	bool m_bStateChecked;
	UINT m_uButtonState;
	int	 m_nCheckWidth;
	int	 m_nCheckHight;
	bool m_bAutoTextPadding;

	CStdString m_sNormalImage;
	CStdString m_sCheckedImage;
	CStdString m_sStatusNormalImage;
	CStdString m_sStatusCheckedImage;
};

#endif