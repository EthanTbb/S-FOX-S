#ifndef UITOAST_HEAD_FILE
#define UITOAST_HEAD_FILE

#pragma once


class FGUI_LIB_CLASS CToastUI : public CControlUI, public CUIAnimation
{
public:
	CToastUI();
	virtual ~CToastUI();
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	virtual void DoInit();
	void SetPos(RECT rc);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	LPCTSTR GetToastText() const;
	void SetToastText(LPCTSTR pstrValue);
	int GetToastFont() const;
	void SetToastFont(int iFont);
	DWORD GetToastTextColor() const;
	void SetToastTextColor(DWORD dwTextColor);
	UINT GetToastTextStyle() const;
	void SetToastTextStyle(UINT uStyle);
	DWORD GetToastBkColor() const;
	void SetToastBkColor(DWORD dwBackColor);

	LPCTSTR GetToastBkImage() const;
	void SetToastBkImage(LPCTSTR pstrValue);

	void Show(const int nTimer = 0);

	void DoEvent(TEventUI& event);
	void OnTimer( int nTimerID );
	virtual void DoPaint(HDC hDC, const RECT& rcPaint);
    void PaintToastBkColor(HDC hDC);
	void PaintToastBkImage(HDC hDC);
	void PaintToastText(HDC hDC);

	virtual VOID OnAnimationStart(int nAnimationID, BOOL bFirstLoop) {};
	virtual VOID OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID);;
	virtual VOID OnAnimationStop(int nAnimationID) {};
private:
	static CStdString UpdateFadeAttribute(const CStdString& imageAttribute, int fadeAlpha);	
private:


	CStdString m_sToastBkImage;
	CStdString m_sToastText;
	DWORD m_dwToastTextColor;
	int m_iToastTextFont;
	UINT m_uToastTextStyle;
	DWORD m_dwToastBackColor;

	int m_nMoveDistance;
	BYTE       m_bFadeAlpha;
	enum{
		FADE_OUT_ID			= 9,
		FADE_ELLAPSE		= 10,
		FADE_FRAME_COUNT	= 100,
	};
};
#endif //
