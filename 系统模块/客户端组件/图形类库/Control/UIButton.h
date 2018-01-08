#ifndef __UIBUTTON_H__
#define __UIBUTTON_H__

#pragma once
class CNumberUI;

class FGUI_LIB_CLASS CButtonUI : public CLabelUI
{
public:
	CButtonUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	UINT GetControlFlags() const;
	void SetWantReturn();
	void SetFadeOut(bool bFadeOut = true);
	bool IsFadeOut();
	void SetEnabelFocused(bool bHaveFocus);
	bool IsEnabelFocused();

	bool Activate();
	void SetEnabled(bool bEnable = true);
	void DoEvent(TEventUI& event);
	void OnTimer( int nTimerID );

	LPCTSTR GetNormalImage();
	virtual void SetNormalImage(LPCTSTR pStrImage);
	LPCTSTR GetHotImage();
	void SetHotImage(LPCTSTR pStrImage);
	LPCTSTR GetPushedImage();
	void SetPushedImage(LPCTSTR pStrImage);
	LPCTSTR GetFocusedImage();
	void SetFocusedImage(LPCTSTR pStrImage);
	LPCTSTR GetDisabledImage();
	void SetDisabledImage(LPCTSTR pStrImage);
	virtual void SetStatusImage(LPCTSTR pStrImage, bool bFixed=true);
	LPCTSTR GetStatusImage();

	void SetHotBkColor(DWORD dwColor);
	DWORD GetHotBkColor() const;

	void SetHotTextColor(DWORD dwColor);
	DWORD GetHotTextColor() const;
	LPCTSTR GetForeImage();
	void SetForeImage(LPCTSTR pStrImage);
	LPCTSTR GetHotForeImage();
	void SetHotForeImage(LPCTSTR pStrImage);
	void SetPushedTextColor(DWORD dwColor);
	DWORD GetPushedTextColor() const;
	void SetFocusedTextColor(DWORD dwColor);
	DWORD GetFocusedTextColor() const;
	SIZE EstimateSize(SIZE szAvailable);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void PaintText(HDC hDC);
	void PaintStatusImage(HDC hDC);
	bool DrawImageEx(HDC hDC, LPCTSTR pStrImage,const RECT& rcBmpPart,BYTE uFade = 255);

	void BindTabIndex(int _BindTabIndex);
	void BindTabLayoutName(LPCTSTR _TabLayoutName);
	void BindTriggerTabSel(int _SetSelectIndex = -1);
	void RemoveBindTabIndex();
	int	 GetBindTabLayoutIndex();
	CStdString GetBindTabLayoutName();

	CStdString GetBindNumberName();
	bool SetBindNumberName(LPCTSTR NumberUIName);
	int  GetBindNumberSetp();
	void SetBindNumberSetp(const int nSetp);

private:
	bool SetpBindNumber(const int nNumberStep);

	//¹¤³§º¯Êý
public:
	static CButtonUI * Create(CPaintManagerUI * pManager,CControlUI * pParent,LPCTSTR pszName,bool bInit=true);

protected:
	UINT m_uButtonState;
	SIZE m_cxyStatus;
	bool m_bFadeout;
	bool m_bEnableFocused;
	bool m_bCanRaiseBindNumber;

	DWORD m_dwHotBkColor;
	DWORD m_dwHotTextColor;
	DWORD m_dwPushedTextColor;
	DWORD m_dwFocusedTextColor;
	
	CStdString	m_sNormalImage;
	CStdString	m_sHotImage;
	CStdString	m_sHotForeImage;
	CStdString	m_sPushedImage;
	CStdString	m_sPushedForeImage;
	CStdString	m_sFocusedImage;
	CStdString	m_sDisabledImage;
	CStdString  m_sStatusImage;
	CStdString m_sForeImage;

	int			m_iBindTabIndex;
	CStdString	m_sBindTabLayoutName;

	CStdString  m_sBindNumberUIName;
	CNumberUI*	m_pBindNumberUI;
	int			m_iBindNumberRaiseTimes;
	int			m_nBindNumberRaiseInterval;
	int			m_nBindNumberStep;
	int			m_nFiexdBindNumberStep;
};

#endif // __UIBUTTON_H__