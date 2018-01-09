#pragma once
#include "FlashPlayer.h"
#include "FlashShowWnd.h"
//#include "..\GameClientDlg.h"

#include <map>
using namespace std;

const WORD	SHOWLOTTERYFLASH_W = 550;
const WORD	SHOWLOTTERYFLASH_H = 400;

const WORD	SHOWLOTTERYFLASH1_W = 200;
const WORD	SHOWLOTTERYFLASH1_H = 150;


class CShowLotteryFlash2:
	public CFlashShowWnd2
{
public:
	CShowLotteryFlash2(void);
	virtual ~CShowLotteryFlash2(void);

	void	ShowFlash(CWnd *pIRoomViewItem, int showx, int showy,bool bShow);
	void	GiveShowInfo(BYTE Card[]);
    void    StopFlash();
	void	MoveFlash(CWnd *pIRoomViewItem, int showx, int showy);

	virtual bool DealFlashCommand(LPCSTR cmd, LPCSTR lparam);

private:
	bool			m_isSending;
	CWnd     	*m_pIRoomViewItem;
	//CSkinMenu		m_SkinMenu;
public:
    int    m_nDesicType;       //»­Íë»¹ÊÇÍ°

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnPlayFlashEnd(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnClickMenuItem(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};