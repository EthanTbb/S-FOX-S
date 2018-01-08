#ifndef __UITILELAYOUT_H__
#define __UITILELAYOUT_H__

#pragma once


/////////////////////////////////////////////////////////////////////////////////////
//

class FGUI_LIB_CLASS CTileLayoutUI : public CContainerUI
{
public:
	CTileLayoutUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	void SetPos(RECT rc);

	SIZE GetItemSize() const;
	void SetItemSize(SIZE szItem);
	int GetColumns() const;
	void SetColumns(int nCols);
	int GetRows() const;
	void SetRows(int nRows);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

protected:
	SIZE m_szItem;
	int m_nColumns;
	int m_nRows;
	bool m_bAutoColumns;
	bool m_bAutoRows;
};


#endif