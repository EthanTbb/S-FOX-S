#ifndef UICOMBO_HEAD_FILE
#define UICOMBO_HEAD_FILE

#pragma once

/////////////////////////////////////////////////////////////////////////////////////
//

class CComboWnd;

class FGUI_LIB_CLASS CComboUI : public CContainerUI, public IListOwnerUI
{
    friend CComboWnd;
public:
    CComboUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

	virtual CControlUI* GetOwner();
	virtual void SetOwner( CControlUI* pControlUI);

    void DoInit();
    UINT GetControlFlags() const;

    CStdString GetText() const;
    void SetEnabled(bool bEnable = true);

    CStdString GetDropBoxAttributeList();
    void SetDropBoxAttributeList(LPCTSTR pstrList);
    SIZE GetDropBoxSize() const;
    void SetDropBoxSize(SIZE szDropBox);

	void SetDropBoxBkColor(DWORD dwColor);
	void SetDropBoxBorderColor(DWORD dwColor);
	void SetDropBoxBorderSize(DWORD dwSize);
	DWORD GetDropBoxBkColor();
	DWORD GetDropBoxBorderColor();
	DWORD GetDropBoxBorderSize();
	
	void SetDropBoxBkImage(LPCTSTR pstrImage);
	CStdString GetDropBoxBkImage();

    int GetCurSel() const;  
    bool SelectItem(int iIndex, bool bTakeFocus = false);
	bool DeleteItem(int iIndex);

	int GetCurHover() const;
	bool HoverItem(int iIndex);
	bool HoverItem(CControlUI* pControlUI);

    bool SetItemIndex(CControlUI* pControl, int iIndex);
    bool Add(CControlUI* pControl);
    bool AddAt(CControlUI* pControl, int iIndex);
    bool Remove(CControlUI* pControl);
    bool RemoveAt(int iIndex);
    void RemoveAll();

    bool Activate();

    RECT GetTextPadding() const;
    void SetTextPadding(RECT rc);
    LPCTSTR GetNormalImage() const;
    void SetNormalImage(LPCTSTR pStrImage);
    LPCTSTR GetHotImage() const;
    void SetHotImage(LPCTSTR pStrImage);
    LPCTSTR GetPushedImage() const;
    void SetPushedImage(LPCTSTR pStrImage);
    LPCTSTR GetFocusedImage() const;
    void SetFocusedImage(LPCTSTR pStrImage);
    LPCTSTR GetDisabledImage() const;
    void SetDisabledImage(LPCTSTR pStrImage);

    TListInfoUI* GetListInfo();
    void SetItemFont(int index);
    void SetItemTextStyle(UINT uStyle);
	RECT GetItemTextPadding() const;
    void SetItemTextPadding(RECT rc);
	DWORD GetItemTextColor() const;
    void SetItemTextColor(DWORD dwTextColor);
	DWORD GetItemBkColor() const;
    void SetItemBkColor(DWORD dwBkColor);
	LPCTSTR GetItemBkImage() const;
    void SetItemBkImage(LPCTSTR pStrImage);
    bool IsAlternateBk() const;
    void SetAlternateBk(bool bAlternateBk);

	DWORD GetSelectedItemTextColor() const;
    void SetSelectedItemTextColor(DWORD dwTextColor);
	DWORD GetSelectedItemBkColor() const;
    void SetSelectedItemBkColor(DWORD dwBkColor);
	LPCTSTR GetSelectedItemImage() const;
    void SetSelectedItemImage(LPCTSTR pStrImage);

	DWORD GetHotItemTextColor() const;
    void SetHotItemTextColor(DWORD dwTextColor);
	DWORD GetHotItemBkColor() const;
    void SetHotItemBkColor(DWORD dwBkColor);
	LPCTSTR GetHotItemImage() const;
    void SetHotItemImage(LPCTSTR pStrImage);

	DWORD GetDisabledItemTextColor() const;
    void SetDisabledItemTextColor(DWORD dwTextColor);
	DWORD GetDisabledItemBkColor() const;
    void SetDisabledItemBkColor(DWORD dwBkColor);
	LPCTSTR GetDisabledItemImage() const;
    void SetDisabledItemImage(LPCTSTR pStrImage);

	DWORD GetItemLineColor() const;
    void SetItemLineColor(DWORD dwLineColor);

    bool IsItemShowHtml();
    void SetItemShowHtml(bool bShowHtml = true);

	void SetItemChangeLarge(bool bChangeLarge=false);
	bool IsItemChangeLarge();
	void SetItemChangeHeight(const int nChangeHeight);
	int  GetItemChangeHeight();

    SIZE EstimateSize(SIZE szAvailable);
    void SetPos(RECT rc);
    void DoEvent(TEventUI& event);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    
    void DoPaint(HDC hDC, const RECT& rcPaint);
    void PaintText(HDC hDC);
    void PaintStatusImage(HDC hDC);
	void OnFinalMessage();

	//¹¤³§º¯Êý
public:
	static CComboUI * Create(CPaintManagerUI * pManager,CControlUI * pParent,LPCTSTR pszName,bool bInit=true);

protected:
    CComboWnd* m_pWindow;
	CControlUI* m_pOwner;

    int m_iCurSel;
    RECT m_rcTextPadding;
    CStdString m_sDropBoxAttributes;
    SIZE m_szDropBox;
    UINT m_uButtonState;
	int	m_iDeleteSel;

	DWORD m_dwDropBoxBkColor;
	DWORD m_dwDropBoxBorderSize;
	DWORD m_dwDropBoxBorderColor;
	CStdString m_sDropBoxBkImage;

    CStdString m_sNormalImage;
    CStdString m_sHotImage;
    CStdString m_sPushedImage;
    CStdString m_sFocusedImage;
    CStdString m_sDisabledImage;

    TListInfoUI m_ListInfo;
};


#endif
