#ifndef __UITEXT_H__
#define __UITEXT_H__

#pragma once

/////////////////////////////////////////////////////////////////////////////////////
//

class FGUI_LIB_CLASS CTextUI : public CLabelUI
{
public:
	CTextUI();
	~CTextUI();

	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	CStdString* GetLinkContent(int iIndex);

	void SetLineSpace(int space);
	int GetLineSpace() const;
	void SetLineHeight(int height);
	int GetLineHeight() const;

	void DoEvent(TEventUI& event);
	SIZE EstimateSize(SIZE szAvailable);

	void PaintText(HDC hDC);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	//¹¤³§º¯Êý
public:
	static CTextUI * Create(CPaintManagerUI * pManager,CControlUI * pParent,LPCTSTR pszName,bool bInit=true);

protected:
	enum { MAX_LINK = 8 };
	int m_nLinks;
	RECT m_rcLinks[MAX_LINK];
	CStdString m_sLinks[MAX_LINK];
	CStdString m_sLinkNames[MAX_LINK];
	int m_nHoverLink;
	int m_nLineSpace;
	int m_nLineHeight;
};

#endif