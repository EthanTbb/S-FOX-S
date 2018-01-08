#ifndef __UIEDIT_H__
#define __UIEDIT_H__

#pragma once

/////////////////////////////////////////////////////////////////////////////////////
//
class CEditUI;
class CEditWnd : public CWindowWnd
{
public:
	CEditWnd();

	void Init(CEditUI* pOwner);
	RECT CalPos();
	void CreateBkBrush();

	LPCTSTR GetWindowClassName() const;
	LPCTSTR GetSuperClassName() const;
	void OnFinalMessage(HWND hWnd);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEditChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
	CEditUI* m_pOwner;
	HBRUSH m_hBkBrush;
};



class FGUI_LIB_CLASS CEditUI : public CLabelUI
{
	friend CEditWnd;
public:
	CEditUI();
	virtual ~CEditUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	UINT GetControlFlags() const;

	void SetEnabled(bool bEnable = true);
	void SetText(LPCTSTR pstrText);
	void SetMaxChar(UINT uMax);
	UINT GetMaxChar();
	void SetNumberOnly(bool bOnlyNumber=true);
	bool IsNumberOnly();
	void SetIntegerNumber(bool bIntegerNumber=true);
	bool GetIntegerNumber();
	void SetSegNumber(bool bSegNumber=true);
	bool IsSegNumber();
	void SetReadOnly(bool bReadOnly);
	bool IsReadOnly() const;
	void SetPasswordMode(bool bPasswordMode);
	bool IsPasswordMode() const;
	HWND GetEditWindow();
	void SetPasswordChar(TCHAR cPasswordChar);
	TCHAR GetPasswordChar() const;
	void SetSel(int nStartPos,int nEndPos);
	DWORD GetSel();

	LPCTSTR GetNormalImage();
	void SetNormalImage(LPCTSTR pStrImage);
	LPCTSTR GetHotImage();
	void SetHotImage(LPCTSTR pStrImage);
	LPCTSTR GetFocusedImage();
	void SetFocusedImage(LPCTSTR pStrImage);
	LPCTSTR GetDisabledImage();
	void SetDisabledImage(LPCTSTR pStrImage);
	void SetNativeEditBkColor(DWORD dwBkColor);
	DWORD GetNativeEditBkColor() const;

	LPCTSTR GetEyeStatusImage();
	void SetEyeStatusImage(LPCTSTR pStrImage);
	LPCTSTR GetEyeText();
	void SetEyeText(LPCTSTR pstrValue);
	void SetEyeTextColor(DWORD dwTextColor);
	DWORD GetEyeTextColor();
	CButtonUI* GetEyeButton();

	void SetTipValue(LPCTSTR pStrTipValue);
	LPCTSTR GetTipValue();
	void SetTipValueColor(LPCTSTR pStrColor);
	DWORD GetTipValueColor();

	void SetPos(RECT rc);
	void SetVisible(bool bVisible = true);
	void SetInternVisible(bool bVisible = true);
	SIZE EstimateSize(SIZE szAvailable);
	void DoEvent(TEventUI& event);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	CControlUI* FindControl(FINDCONTROLPROC Proc,const CPaintManagerUI * pManager, LPVOID pData, UINT uFlags);

	void DoPaint(HDC hDC, const RECT& rcPaint);
	void PaintStatusImage(HDC hDC);
	void PaintText(HDC hDC);

protected:
	virtual bool OnEyeEvent(void* param);
	tString SegFormatText(LPCTSTR pstrText);

	//¹¤³§º¯Êý
public:
	static CEditUI * Create(CPaintManagerUI * pManager,CControlUI * pParent,LPCTSTR pszName,bool bInit=true);
	
protected:
	CEditWnd* m_pWindow;
	CButtonUI* m_pEyePwd;

	UINT m_uMaxChar;
	bool m_bReadOnly;
	bool m_bOnlyNumber;
	bool m_bIntegerNumber;
	bool m_bSegNumber;
	bool m_bPasswordMode;
	bool m_bPrePasswordMode;
	TCHAR m_cPasswordChar;
	UINT m_uButtonState;
	CStdString m_sNormalImage;
	CStdString m_sHotImage;
	CStdString m_sFocusedImage;
	CStdString m_sDisabledImage;
	CStdString m_sEyeStatusImage;
	CStdString m_sEyeText;
	CStdString m_sTipValue;
	DWORD m_dwTipValueColor;
	DWORD m_dwEyeTextColor;

	DWORD m_dwEditbkColor;
};


#endif