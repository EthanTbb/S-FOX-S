#ifndef UIEDIT_COMBO_HEAD_FILE
#define UIEDIT_COMBO_HEAD_FILE

#pragma once

class FGUI_LIB_CLASS CEditComboUI : public CEditUI, public IListOwnerUI
{
public:
	CEditComboUI();
	~CEditComboUI();
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	bool Add(CControlUI* pControl);
	int GetCount() const;
	CControlUI* GetItemAt(int iIndex) const;
	bool RemoveAt(int iIndex);
	bool Remove(CControlUI* pControl);

	virtual TListInfoUI* GetListInfo();
	virtual int GetCurSel() const;
	virtual int GetCurHover() const ;
	virtual bool SelectItem(int iIndex, bool bTakeFocus = false);
	virtual bool DeleteItem(int iIndex);
	virtual bool HoverItem(int iIndex);
	virtual bool HoverItem(CControlUI* pControlUI);
	virtual void DoEvent(TEventUI& event);

	bool Activate();

	void SetDropBoxBkImage(LPCTSTR pstrImage);
	CStdString GetDropBoxBkImage();

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

	CComboUI* GetCombo();
	
	void SetPos(RECT rc);
	void PaintStatusImage(HDC hDC);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	CControlUI* FindControl(FINDCONTROLPROC Proc,const CPaintManagerUI * pManager, LPVOID pData, UINT uFlags);
	void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);

protected:
	virtual bool OnEyeEvent(void* param);
	virtual bool OnNotifyCombo(void* param);

protected:
	CComboUI* m_pCombo;
};

#endif