#ifndef __UITABLAYOUT_H__
#define __UITABLAYOUT_H__

#pragma once



/////////////////////////////////////////////////////////////////////////////////////
//

class FGUI_LIB_CLASS CTabLayoutUI : public CContainerUI
{
public:
	CTabLayoutUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	bool Add(CControlUI* pControl);
	bool AddAt(CControlUI* pControl, int iIndex);
	bool Remove(CControlUI* pControl);
	void RemoveAll();
	int GetCurSel() const;
	bool SelectItem(int iIndex);

	void SetPos(RECT rc);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

protected:
	int m_iCurSel;
};


#endif