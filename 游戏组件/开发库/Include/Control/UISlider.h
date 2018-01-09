#ifndef __UISLIDER_H__
#define __UISLIDER_H__

#pragma once

/////////////////////////////////////////////////////////////////////////////////////
//

class FGUI_LIB_CLASS CSliderUI : public CProgressUI
{
public:
	CSliderUI();

	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	void SetEnabled(bool bEnable = true);

	int GetChangeStep();
	void SetChangeStep(int step);
	void SetThumbSize(SIZE szXY);
	RECT GetThumbRect() const;
	LPCTSTR GetThumbImage() const;
	void SetThumbImage(LPCTSTR pStrImage);
	LPCTSTR GetThumbHotImage() const;
	void SetThumbHotImage(LPCTSTR pStrImage);
	LPCTSTR GetThumbPushedImage() const;
	void SetThumbPushedImage(LPCTSTR pStrImage);

	void DoEvent(TEventUI& event);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void PaintStatusImage(HDC hDC);

	//¹¤³§º¯Êý
public:
	static CSliderUI * Create(CPaintManagerUI * pManager,CControlUI * pParent,LPCTSTR pszName,bool bInit=true);

protected:
	SIZE m_szThumb;
	UINT m_uButtonState;
	int m_nStep;

	CStdString m_sThumbImage;
	CStdString m_sThumbHotImage;
	CStdString m_sThumbPushedImage;

	CStdString m_sImageModify;
};

#endif