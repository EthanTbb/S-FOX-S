#ifndef __UIHOTKEY_H__
#define __UIHOTKEY_H__

#pragma once

class FGUI_LIB_CLASS CHotKeyUI : public CEditUI
{
public:
	CHotKeyUI();
	~CHotKeyUI();

	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	bool SetHotKey(WORD wVirtualKeyCode, WORD wModifiers);
	DWORD GetHotKey() const;
	void SetRules(WORD wInvalidComb, WORD wModifiers);
	bool SetHotKeyValues(LPCTSTR pszKeyValues);
	LPCTSTR GetHotKeyValues();
	bool IsHotKeySucc() const;
	void ClearHotKey();
	
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

private:
	void SetText(LPCTSTR pstrText);

private:
	WORD m_wHotKey;
	WORD m_wInvalidComb;
	DWORD m_wRuleModifiers;
	bool m_bHotKeySucc;
	CStdString m_sHotKeyValues;
};

#endif