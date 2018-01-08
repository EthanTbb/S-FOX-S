#ifndef __UIDESKLISTLAYOUT_H__
#define __UIDESKLISTLAYOUT_H__

#pragma once

class FGUI_LIB_CLASS CDeskListLayoutUI : public CTileLayoutUI
{
public:
	enum { SCROLL_TIMERID = 10 };

	CDeskListLayoutUI();
	virtual ~CDeskListLayoutUI();
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	void DoEvent(TEventUI& event);
	void SetPos(RECT rc);

private:
	UINT m_uButtonState;
	POINT m_ptLastMouse;
	LONG m_dwDelayDeltaY;
	DWORD m_dwDelayNum;
	DWORD m_dwDelayLeft;
};


#endif