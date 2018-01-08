#ifndef UIRENDER_HEAD_FILE
#define UIRENDER_HEAD_FILE
#pragma once

/////////////////////////////////////////////////////////////////////////////////////

class FGUI_LIB_CLASS CRenderClip
{
public:
    ~CRenderClip();
    RECT rcItem;
    HDC hDC;
    HRGN hRgn;
    HRGN hOldRgn;

    static void GenerateClip(HDC hDC, RECT rc, CRenderClip& clip);
    static void GenerateRoundClip(HDC hDC, RECT rc, RECT rcItem, int width, int height, CRenderClip& clip);
    static void UseOldClipBegin(HDC hDC, CRenderClip& clip);
    static void UseOldClipEnd(HDC hDC, CRenderClip& clip);
};

/////////////////////////////////////////////////////////////////////////////////////
//

class FGUI_LIB_CLASS CRenderEngine
{
public:
    static DWORD AdjustColor(DWORD dwColor, short H, short S, short L);
	//static WORD LoadFileFormZip(const TCHAR * pszFilePath,LPBYTE & pData);
    static TImageInfo* LoadImage(STRINGorID bitmap, LPCTSTR type = NULL, DWORD mask = 0);
	static void FreeImage(const TImageInfo* bitmap);
    static void DrawImage(HDC hDC, HBITMAP hBitmap, const RECT& rc, const RECT& rcPaint, \
        const RECT& rcBmpPart, const RECT& rcCorners, bool alphaChannel, BYTE uFade = 255, 
        bool hole = false, bool xtiled = false, bool ytiled = false, bool tiled = false);
    static bool DrawImageString(HDC hDC, CPaintManagerUI* pManager, const RECT& rcItem, const RECT& rcPaint, 
        LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);
	static bool DrawImageString(HDC hDC, CPaintManagerUI* pManager, const RECT& rc, const RECT& rcPaint, 
		LPCTSTR pStrImage, LPCTSTR pStrModify, bool bNeedAlpha, BYTE bNewFade);
    static void DrawColor(HDC hDC, const RECT& rc, DWORD color);
    static void DrawGradient(HDC hDC, const RECT& rc, DWORD dwFirst, DWORD dwSecond, bool bVertical, int nSteps);
	//Ð§ÂÊµÍ É÷ÓÃ
	static void DrawText(HDC hDC,CPaintManagerUI* pManager, RECT& rc, LPCTSTR pstrText,DWORD dwTextColor, int iFont, UINT uStyle,BYTE byFade);

    static void DrawLine(HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor);
    static void DrawRect(HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor);
    static void DrawRoundRect(HDC hDC, const RECT& rc, int width, int height, int nSize, DWORD dwPenColor);
    static void DrawText(HDC hDC, CPaintManagerUI* pManager, RECT& rc, LPCTSTR pstrText, \
        DWORD dwTextColor, int iFont, UINT uStyle);
	static void DrawHtmlText(HDC hDC, CPaintManagerUI* pManager, RECT& rc, LPCTSTR pstrText, \
        DWORD dwTextColor, RECT* pLinks, CStdString* sLinks, int& nLinkRects, UINT uStyle, \
		int iFont = -1, int nLineSpace=0, int nLineHeight=0, CStdString* sLinkNames = NULL);
	static void DrawText(HDC hDC, RECT& rc, LPCTSTR pstrText, DWORD dwTextColor, HFONT hFont, UINT uStyle);  
	static HBITMAP GenerateBitmap(CPaintManagerUI* pManager, CControlUI* pControl, RECT rc);
	static bool GetImageWriteStream(LPCTSTR pszResourceName, IStream* pIStream, LPCTSTR type = NULL);
	static TImageInfo* GetTImageInfo(BYTE* pData, DWORD dwSize, DWORD mask);

	//alpha utilities
	static void CheckAlphaColor(DWORD& dwColor);
	static void ClearAlphaPixel(LPBYTE pBits, int bitsWidth, PRECT rc);
	static void RestoreAlphaColor(LPBYTE pBits, int bitsWidth, PRECT rc);
};

#endif
