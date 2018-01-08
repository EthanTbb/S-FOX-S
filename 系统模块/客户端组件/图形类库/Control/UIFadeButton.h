#ifndef __UIFADEBUTTON_H__
#define __UIFADEBUTTON_H__

#include "UIAnimation.h"
#pragma once

class FGUI_LIB_CLASS CFadeButtonUI : public CButtonUI, public CUIAnimation
{
public:
	CFadeButtonUI();
	virtual ~CFadeButtonUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	void SetNormalImage(LPCTSTR pStrImage);

	void DoEvent(TEventUI& event);
	void OnTimer( int nTimerID );
	void PaintStatusImage(HDC hDC);

	virtual void OnAnimationStart(INT nAnimationID, BOOL bFirstLoop) {}
	virtual void OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID);
	virtual void OnAnimationStop(INT nAnimationID) {}

private:
	static CStdString UpdateFadeAttribute(const CStdString& imageAttribute, int fadeAlpha);	
protected:
	CStdString m_sOldImage;
	CStdString m_sNewImage;
	CStdString m_sLastImage;
	BYTE       m_bFadeAlpha;
	BYTE       m_bFadeAlphaAssist;
	BOOL       m_bMouseHove;
	BOOL       m_bMouseLeave;
	enum{
		FADE_IN_ID			= 8,
		FADE_OUT_ID			= 9,

		FADE_ELLAPSE		= 10,
		FADE_FRAME_COUNT	= 30,
	};
};

#endif // __UIFADEBUTTON_H__