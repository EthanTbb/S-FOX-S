#ifndef __UIANIMPAGELAYOUT_H__
#define __UIANIMPAGELAYOUT_H__

#pragma once

/////////////////////////////////////////////////////////////////////////////////////
//
class FGUI_LIB_CLASS CAnimPageLayoutUI : public CPageLayoutUI, public CUIAnimation
{
public:
	CAnimPageLayoutUI();
	virtual ~CAnimPageLayoutUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	virtual void OnAnimationStart(INT nAnimationID, BOOL bFirstLoop) {}
	virtual void OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID);
	virtual void OnAnimationStop(INT nAnimationID) {}

	void SetInterval(const int nInterval);
	int GetInterval();
	void SetFrameCount(const int nFrameCount);
	int GetFrameCount();
	void SetLoop(bool bLoop);
	bool IsLoop();
	void RestartAnimation();
	void SetVisible(bool bVisible = true);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void DoEvent(TEventUI& event);
	void OnTimer( int nTimerID );
	void DoInit();
	
	//¹¤³§º¯Êý
public:
	static CAnimPageLayoutUI * Create(CPaintManagerUI * pManager,CControlUI * pParent,LPCTSTR pszName,bool bInit=true);

protected:
	enum{
		ANIM_TIMER_ID		= 13,
		ANIM_INTERVAL		= 3000,
		ANIM_FRAME_COUNT	= 10
	};
	int		m_nAnimInterval;
	int		m_nAnimFrameCount;
	bool	m_bLoop;		
};

#endif