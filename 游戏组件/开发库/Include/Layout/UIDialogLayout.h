#ifndef __UIDIALOGLAYOUT_H__
#define __UIDIALOGLAYOUT_H__

#pragma once


/////////////////////////////////////////////////////////////////////////////////////
//

class FGUI_LIB_CLASS CDialogLayoutUI : public CContainerUI
{
public:
	CDialogLayoutUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	void SetStretchMode(CControlUI* pControl, UINT uMode);

	void SetPos(RECT rc);
	SIZE EstimateSize(SIZE szAvailable);
	RECT RecalcArea();   

	//¹¤³§º¯Êý
public:
	static CDialogLayoutUI * Create(CPaintManagerUI * pManager,CControlUI * pParent,LPCTSTR pszName,bool bInit=true);

protected:  
	typedef struct 
	{
		CControlUI* pControl;
		UINT uMode;
		RECT rcItem;
	} STRETCHMODE;

	CStdValArray m_aModes;
};



#endif