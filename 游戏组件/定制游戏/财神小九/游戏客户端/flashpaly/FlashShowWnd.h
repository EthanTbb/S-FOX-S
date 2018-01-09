#pragma once
#include "FlashPlayer.h"

#include <map>
using namespace std;


#define TIMER_ID_FLASH_PLAY		0

const DWORD CM_FLASH_PLAY_END	= (WM_USER+190);


typedef struct {
	//char	theFileName[MAX_PATH];	//flash
	TCHAR	theFileName[MAX_PATH];	//flash
	LPRECT	lprcshow;				//为NULL显示区域就为ppwnd对应的屏幕区域
	HWND	ppWnd;					//flash接收消息窗口
	void	*return_param;			//接收消息时的返回参数
	DWORD	msgID;					//消息类型 一般为 CM_FLASH_PLAY_END
	LPCSTR	flashcommand;			//播放时给 flash 传递的参数
	LPCSTR	flashcmdparam;			//
}PlayFlashAnimation;

class CFlashShowWnd2 :
	public CWnd
{
protected:
	FlashPlayer2		*m_pPlayer;
	CSkinImage		m_buffer;
	int				m_cx, m_cy;
	HWND			m_ppWnd;
	DWORD			m_msgID;
	void			*m_returnparam;
	int				m_nCurrentFrame;
	void			DrawFlashToClient();
public:
	void			OnCloseFalsh();

public:
	CFlashShowWnd2(void);
	virtual ~CFlashShowWnd2(void);

	bool	StartAnimation(PlayFlashAnimation *pplayparam,bool bShow);
	void	StopAnimation();
	virtual bool DealFlashCommand(LPCSTR cmd, LPCSTR lparam);
	//绘制
	 bool DrawAnimation(int nFrame);

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};



