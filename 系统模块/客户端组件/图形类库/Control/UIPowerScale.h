#ifndef __UIPOWERSCALE_H__
#define __UIPOWERSCALE_H__

#pragma once

class FGUI_LIB_CLASS CPowerScaleUI : public CLabelUI
{
public:
	CPowerScaleUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	void SetLevelState(const UINT level);
	UINT GetLevelState();
	LPCTSTR GetFgImage() const;
	void SetFgImage(LPCTSTR pStrImage);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void PaintStatusImage(HDC hDC);
	void DoEvent(TEventUI& event);
	//¹¤³§º¯Êý
public:
	static CPowerScaleUI * Create(CPaintManagerUI * pManager,CControlUI * pParent,LPCTSTR pszName,bool bInit=true);

protected:
	enum 
	{ 
		TRANSITION_TIMERID = 20,
		LEVEL_SCALE_0 = 0,
		LEVEL_SCALE_1 = 33,
		LEVEL_SCALE_2 = 66,
		LEVEL_SCALE_3 = 100
	};
	UINT m_uLevelState;
	UINT m_uPreLevelState;
	int m_nMax;
	int m_nMin;
	int m_nValue;
	CStdString m_sFgImage;
	CStdString m_sFgImageModify;
};

#endif