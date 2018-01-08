#ifndef UIMANAGER_HEAD_FILE
#define UIMANAGER_HEAD_FILE
#pragma once
#include "Utils/UIShadow.h"
#include <map>
/////////////////////////////////////////////////////////////////////////////////////
//类型声明
class CControlUI;
class CRichEditUI;

/////////////////////////////////////////////////////////////////////////////////////
//

typedef enum EVENTTYPE_UI
{
    UIEVENT__FIRST = 1,
    UIEVENT__KEYBEGIN,
    UIEVENT_KEYDOWN,
    UIEVENT_KEYUP,
    UIEVENT_CHAR,
    UIEVENT_SYSKEY,
    UIEVENT__KEYEND,
    UIEVENT__MOUSEBEGIN,
    UIEVENT_MOUSEMOVE,
    UIEVENT_MOUSELEAVE,
    UIEVENT_MOUSEENTER,
    UIEVENT_MOUSEHOVER,
    UIEVENT_BUTTONDOWN,
    UIEVENT_BUTTONUP,
    UIEVENT_RBUTTONDOWN,
	UIEVENT_RBUTTONUP,
    UIEVENT_DBLCLICK,
    UIEVENT_CONTEXTMENU,
    UIEVENT_SCROLLWHEEL,
    UIEVENT__MOUSEEND,
    UIEVENT_KILLFOCUS,
    UIEVENT_SETFOCUS,
    UIEVENT_WINDOWSIZE,
    UIEVENT_SETCURSOR,
    UIEVENT_TIMER,
    UIEVENT_NOTIFY,
    UIEVENT_COMMAND,
	UIEVENT_IME_STARTCOMPOSITION,
    UIEVENT__LAST,
};

/////////////////////////////////////////////////////////////////////////////////////
//
#define UIFLAG_TABSTOP       0x00000001
#define UIFLAG_SETCURSOR     0x00000002
#define UIFLAG_WANTRETURN    0x00000004

#define UIFIND_ALL           0x00000000
#define UIFIND_VISIBLE       0x00000001
#define UIFIND_ENABLED       0x00000002
#define UIFIND_HITTEST       0x00000004
#define UIFIND_TOP_FIRST     0x00000008
#define UIFIND_ME_FIRST      0x80000000

#define UISTRETCH_NEWGROUP   0x00000001
#define UISTRETCH_NEWLINE    0x00000002
#define UISTRETCH_MOVE_X     0x00000004
#define UISTRETCH_MOVE_Y     0x00000008
#define UISTRETCH_SIZE_X     0x00000010
#define UISTRETCH_SIZE_Y     0x00000020

#define UISTATE_FOCUSED      0x00000001
#define UISTATE_SELECTED     0x00000002
#define UISTATE_DISABLED     0x00000004
#define UISTATE_HOT          0x00000008
#define UISTATE_PUSHED       0x00000010
#define UISTATE_READONLY     0x00000020
#define UISTATE_CAPTURED     0x00000040

#define DT_MULTILINETEST     0x00002000

/////////////////////////////////////////////////////////////////////////////////////
//类型声明

class CPaintResource;
class CPaintManagerUI;

/////////////////////////////////////////////////////////////////////////////////////
//

typedef struct tagTFontInfo
{
    HFONT hFont;
    CStdString sFontName;
    int iSize;
    bool bBold;
    bool bUnderline;
    bool bItalic;
    TEXTMETRIC tm;
} TFontInfo;

typedef struct tagTImageInfo
{
    HBITMAP hBitmap;
    int nX;
    int nY;
    bool alphaChannel;
    CStdString sResType;
    DWORD dwMask;
} TImageInfo;

typedef struct tagTEventUI
{
    int Type;
    CControlUI* pSender;
    DWORD dwTimestamp;
    POINT ptMouse;
    TCHAR chKey;
    WORD wKeyState;
    WPARAM wParam;
    LPARAM lParam;
} TEventUI;

typedef struct tagTNotifyUI 
{
    CStdString sType;
    CControlUI* pSender;
    DWORD dwTimestamp;
    POINT ptMouse;
    WPARAM wParam;
    LPARAM lParam;
} TNotifyUI;

enum emRelativeSide
{
	RelativeNone=0,
	RelativeLeft,
	RelativeRight
};

typedef struct tagTRelativePosUI
{
	bool bRelative;
	//SIZE szParent;
	emRelativeSide RelativeSide;
	/*int nMoveXOffset;
	int nMoveYOffset;
	int nZoomXPercent;
	int nZoomYPercent;*/
	int nXOffset;
	int nYOffset;

}TRelativePosUI;

class INotifyUI
{
public:
    virtual void Notify(TNotifyUI& msg) = 0;
};

class IMessageFilterUI
{
public:
    virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) = 0;
};

class ITranslateAccelerator
{
public:
	virtual LRESULT TranslateAccelerator(MSG *pMsg) = 0;
};

class IPaintNotifyUI
{
public:
	virtual void OnBeginPaintWindow(HDC hDC)=0;
	virtual void OnEndPaintWindow(HDC hDC)=0;
};


class IPaintResource
{
public:
	virtual DWORD GetDefaultDisabledColor() const=NULL;
    virtual void SetDefaultDisabledColor(DWORD dwColor)=NULL;
    virtual DWORD GetDefaultFontColor() const=NULL;
    virtual void SetDefaultFontColor(DWORD dwColor)=NULL;
    virtual DWORD GetDefaultLinkFontColor() const=NULL;
    virtual void SetDefaultLinkFontColor(DWORD dwColor)=NULL;
    virtual DWORD GetDefaultLinkHoverFontColor() const=NULL;
    virtual void SetDefaultLinkHoverFontColor(DWORD dwColor)=NULL;
    virtual DWORD GetDefaultSelectedBkColor() const=NULL;
    virtual void SetDefaultSelectedBkColor(DWORD dwColor)=NULL;
    virtual TFontInfo* GetDefaultFontInfo(CPaintManagerUI * pManager)=NULL;
    virtual void SetDefaultFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic)=NULL;
    virtual  DWORD GetCustomFontCount() const=NULL;
    virtual HFONT AddFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic)=NULL;
    virtual HFONT AddFontAt(int index, LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic)=NULL;
    virtual HFONT GetFont(int index,CPaintManagerUI * pManager)=NULL;
    virtual HFONT GetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic)=NULL;
    virtual bool FindFont(HFONT hFont)=NULL;
    virtual bool FindFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic)=NULL;
    virtual int GetFontIndex(HFONT hFont)=NULL;
    virtual int GetFontIndex(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic)=NULL;
    virtual bool RemoveFont(HFONT hFont)=NULL;
    virtual bool RemoveFontAt(int index)=NULL;
    virtual TFontInfo* GetFontInfo(int index,CPaintManagerUI * pManager)=NULL;
    virtual TFontInfo* GetFontInfo(HFONT hFont,CPaintManagerUI * pManager)=NULL;

    virtual const TImageInfo* GetImage(LPCTSTR bitmap)=NULL;
	virtual const TImageInfo* GetImageEx(LPCTSTR bitmap)=NULL;
    virtual const TImageInfo* GetImageEx(LPCTSTR bitmap, LPCTSTR type, DWORD mask = 0)=NULL;
    virtual const TImageInfo* AddImage(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0)=NULL;
    virtual const TImageInfo* AddImage(LPCTSTR bitmap, HBITMAP hBitmap, int iWidth, int iHeight, bool bAlpha)=NULL;
    virtual bool RemoveImage(LPCTSTR bitmap)=NULL;

    virtual void AddDefaultAttributeList(LPCTSTR pStrControlName, LPCTSTR pStrControlAttrList)=NULL;
    virtual LPCTSTR GetDefaultAttributeList(LPCTSTR pStrControlName) const=NULL;
    virtual bool RemoveDefaultAttributeList(LPCTSTR pStrControlName)=NULL;
    virtual const CStdStringPtrMap& GetDefaultAttribultes() const=NULL;
};

/////////////////////////////////////////////////////////////////////////////////////

class FGUI_LIB_CLASS CPaintResource : public IPaintResource
{
public:
    DWORD m_dwDefaultDisabledColor;
    DWORD m_dwDefaultFontColor;
    DWORD m_dwDefaultLinkFontColor;
    DWORD m_dwDefaultLinkHoverFontColor;
    DWORD m_dwDefaultSelectedBkColor;
    TFontInfo m_DefaultFontInfo;
    CStdPtrArray m_aCustomFonts;
    CStdStringPtrMap m_mImageHash;
    CStdStringPtrMap m_DefaultAttrHash;

public:
    CPaintResource();
    ~CPaintResource();

    virtual DWORD GetDefaultDisabledColor() const;
    virtual void SetDefaultDisabledColor(DWORD dwColor);
    virtual DWORD GetDefaultFontColor() const;
    virtual void SetDefaultFontColor(DWORD dwColor);
    virtual DWORD GetDefaultLinkFontColor() const;
    virtual void SetDefaultLinkFontColor(DWORD dwColor);
    virtual DWORD GetDefaultLinkHoverFontColor() const;
    virtual void SetDefaultLinkHoverFontColor(DWORD dwColor);
    virtual DWORD GetDefaultSelectedBkColor() const;
    virtual void SetDefaultSelectedBkColor(DWORD dwColor);
    virtual TFontInfo* GetDefaultFontInfo(CPaintManagerUI * pManager);
    virtual void SetDefaultFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    virtual DWORD GetCustomFontCount() const;
    virtual HFONT AddFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    virtual HFONT AddFontAt(int index, LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    virtual HFONT GetFont(int index,CPaintManagerUI * pManager);
    virtual HFONT GetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    virtual bool FindFont(HFONT hFont);
    virtual bool FindFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    virtual int GetFontIndex(HFONT hFont);
    virtual int GetFontIndex(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    virtual bool RemoveFont(HFONT hFont);
    virtual bool RemoveFontAt(int index);
    virtual TFontInfo* GetFontInfo(int index,CPaintManagerUI * pManager);
    virtual TFontInfo* GetFontInfo(HFONT hFont,CPaintManagerUI * pManager);

    virtual const TImageInfo* GetImage(LPCTSTR bitmap);
	virtual const TImageInfo* GetImageEx(LPCTSTR bitmap);
    virtual const TImageInfo* GetImageEx(LPCTSTR bitmap, LPCTSTR type, DWORD mask = 0);
    virtual const TImageInfo* AddImage(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);
    virtual const TImageInfo* AddImage(LPCTSTR bitmap, HBITMAP hBitmap, int iWidth, int iHeight, bool bAlpha);
    virtual bool RemoveImage(LPCTSTR bitmap);

    virtual void AddDefaultAttributeList(LPCTSTR pStrControlName, LPCTSTR pStrControlAttrList);
    virtual LPCTSTR GetDefaultAttributeList(LPCTSTR pStrControlName) const;
    virtual bool RemoveDefaultAttributeList(LPCTSTR pStrControlName);
    virtual const CStdStringPtrMap& GetDefaultAttribultes() const;

public:
	bool ReloadAllImages();

protected:
	void RemoveAllFonts();
	void RemoveAllImages();
	void RemoveAllDefaultAttributeList();
};

/////////////////////////////////////////////////////////////////////////////////////
//
typedef std::map<void*, IWindowCloseCallBack*> MapCloseCallBack;

class FGUI_LIB_CLASS CPaintManagerUI/* : public IPaintResource*/
{
	friend class CPaintResource;

private:
    HWND	 m_hWndPaint;
    HDC		 m_hDcPaint;
    HDC		 m_hDcOffscreen;
    HBITMAP	 m_hbmpOffscreen;
	LPBYTE	 m_pBmpOffscreenBits;
    bool	 m_bShowUpdateRect;
	RECT	 m_rcInvalidate;// 半透明异形窗体中，保存刷新区域
	bool	 m_bIsRestore;

	HWND	 m_hwndTooltip;
	TOOLINFO m_ToolTip;

	//richedit
	RECT m_rtCaret;
	bool m_bCaretActive;
	bool m_bCaretShowing;
	CRichEditUI* m_currentCaretObject;

    //
    CControlUI* m_pRoot;
    CControlUI* m_pFocus;
    CControlUI* m_pEventHover;
    CControlUI* m_pEventClick;
    CControlUI* m_pEventKey;
	CControlUI*	m_pWantReturn;

	//
	ULONG_PTR						m_gdiplusToken;
	Gdiplus::GdiplusStartupInput	*m_pGdiplusStartupInput;
	CShadowUI	m_shadow;
    //
    POINT m_ptLastMousePos;
    SIZE m_szMinWindow;
    SIZE m_szMaxWindow;
    SIZE m_szInitWindowSize;
    RECT m_rcSizeBox;
    SIZE m_szRoundCorner;
    RECT m_rcCaption;
    UINT m_uTimerID;
    bool m_bFirstLayout;
    bool m_bUpdateNeeded;
    bool m_bFocusNeeded;
    bool m_bOffscreenPaint;
    bool m_bAlphaBackground;
    bool m_bMouseTracking;
    bool m_bMouseCapture;
    //
    CStdPtrArray m_aNotifiers;
    CStdPtrArray m_aTimers;
    CStdPtrArray m_aPreMessageFilters;
    CStdPtrArray m_aMessageFilters;
    CStdPtrArray m_aPostPaintControls;
    CStdPtrArray m_aDelayedCleanup;
    CStdPtrArray m_aAsyncNotify;
    CStdPtrArray m_aFoundControls;
    CStdStringPtrMap m_mNameHash;
    CStdStringPtrMap m_mOptionGroup;
	CStdStringPtrMap m_CustomDefaultAttrHash;
	CStdPtrArray m_arrayCustomFonts;
    //

	IPaintNotifyUI * m_pIPaintNotifyUI;

    static HINSTANCE m_hInstance;
    static HINSTANCE m_hResourceInstance;
    static CStdString m_pStrResourcePath;
    static CStdString m_pStrResourceZip;
	static CStdString m_pStrFileZip;
	static bool m_bCachedResourceZip;
	static HANDLE m_hResourceZip;
	static LPBYTE m_lpResourceZIPBuffer;
	static DWORD m_dwResourceZIPBufferSize;

    static short m_H;
    static short m_S;
    static short m_L;
    static CStdPtrArray m_aPreMessages;
	static MapCloseCallBack m_mCloseHash; 
	
public:
	static CPaintResource m_GlobalResource;
	CStdPtrArray m_aTranslateAccelerator;

public:
    CPaintManagerUI();
    ~CPaintManagerUI();

	//接口函数
public:
    virtual DWORD GetDefaultDisabledColor() const;
    virtual void SetDefaultDisabledColor(DWORD dwColor);
    virtual DWORD GetDefaultFontColor() const;
    virtual void SetDefaultFontColor(DWORD dwColor);
    virtual DWORD GetDefaultLinkFontColor() const;
    virtual void SetDefaultLinkFontColor(DWORD dwColor);
    virtual DWORD GetDefaultLinkHoverFontColor() const;
    virtual void SetDefaultLinkHoverFontColor(DWORD dwColor);
    virtual DWORD GetDefaultSelectedBkColor() const;
    virtual void SetDefaultSelectedBkColor(DWORD dwColor);
    virtual TFontInfo* GetDefaultFontInfo();
    virtual void SetDefaultFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    virtual DWORD GetCustomFontCount() const;
    virtual HFONT AddFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    virtual HFONT AddFontAt(int index, LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    virtual HFONT GetFont(int index);
    virtual HFONT GetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    virtual bool FindFont(HFONT hFont);
    virtual bool FindFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    virtual int GetFontIndex(HFONT hFont);
    virtual int GetFontIndex(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    virtual bool RemoveFont(HFONT hFont);
    virtual bool RemoveFontAt(int index);
    virtual TFontInfo* GetFontInfo(int index);
    virtual TFontInfo* GetFontInfo(HFONT hFont);

    virtual const TImageInfo* GetImage(LPCTSTR bitmap);
	virtual const TImageInfo* GetImageEx(LPCTSTR bitmap);
    virtual const TImageInfo* GetImageEx(LPCTSTR bitmap, LPCTSTR type, DWORD mask = 0);
    virtual const TImageInfo* AddImage(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);
    virtual const TImageInfo* AddImage(LPCTSTR bitmap, HBITMAP hBitmap, int iWidth, int iHeight, bool bAlpha);
    virtual bool RemoveImage(LPCTSTR bitmap);

    virtual void AddDefaultAttributeList(LPCTSTR pStrControlName, LPCTSTR pStrControlAttrList);
    virtual LPCTSTR GetDefaultAttributeList(LPCTSTR pStrControlName) const;
    virtual bool RemoveDefaultAttributeList(LPCTSTR pStrControlName);
    virtual const CStdStringPtrMap& GetDefaultAttribultes() const;

public:
	void Init(HWND hWnd);
    void NeedUpdate();
    void Invalidate(RECT& rcItem);

    HDC GetPaintDC() const;
    HWND GetPaintWindow() const;
    HWND GetTooltipWindow() const;

    POINT GetMousePos() const;
    SIZE GetClientSize() const;
    SIZE GetInitSize();
    void SetInitSize(int cx, int cy);
    RECT& GetSizeBox();
    void SetSizeBox(RECT& rcSizeBox);
    RECT& GetCaptionRect();
    void SetCaptionRect(RECT& rcCaption);
    SIZE GetRoundCorner() const;
    void SetRoundCorner(int cx, int cy);
    SIZE GetMinInfo() const;
    void SetMinInfo(int cx, int cy);
    SIZE GetMaxInfo() const;
    void SetMaxInfo(int cx, int cy);
    void SetTransparent(int nOpacity);
    void SetBackgroundTransparent(bool bTrans);
    bool IsShowUpdateRect() const;
    void SetShowUpdateRect(bool show);
	void SetWantReturn(CControlUI * pControlUI);

	bool IsBackgroundTransparent();
	CShadowUI* GetShadow();
	//richedit 
	bool ShowCaret(bool bShow);
	bool SetCaretPos(CRichEditUI* obj, int x, int y);
	CRichEditUI* GetCurrentCaretObject();
	bool CreateCaret(HBITMAP hBmp, int nWidth, int nHeight);
	void DrawCaret(HDC hDC, const RECT& rcPaint);

    static HINSTANCE GetInstance();
    static CStdString GetInstancePath();
    static CStdString GetCurrentPath();
    static HINSTANCE GetResourceDll();
    static const CStdString& GetResourcePath();
    static const CStdString& GetResourceZip();
    static void SetInstance(HINSTANCE hInst);
    static void SetCurrentPath(LPCTSTR pStrPath);
    static void SetResourceDll(HINSTANCE hInst);
    static void SetResourcePath(LPCTSTR pStrPath);
    static void SetFileZip(LPCTSTR pStrZip, bool bCachedResourceZip= false);
	static void SetResourceZip(LPVOID pVoid, unsigned int len);
	static void SetResourceZip(LPCTSTR pStrPath, bool bCachedResourceZip = false);
	static bool IsCachedResourceZip();
	static HANDLE GetResourceZipHandle();
	static void ReleaseZip();

    static void GetHSL(short* H, short* S, short* L);
    static void SetHSL(bool bUseHSL, short H, short S, short L);
    static void ReloadSkin();
	
	static void AddCloseCallBack(void* pObject, IWindowCloseCallBack* pWindowCloseCallBack);
	static void RemoveCloseCallBack(void* pObject);
	static void InvokAllCloseCallBack();

    bool AttachDialog(CControlUI* pControl);
    bool InitControls(CControlUI* pControl, CControlUI* pParent = NULL);
    void ReapObjects(CControlUI* pControl);

    bool AddOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
    CStdPtrArray* GetOptionGroup(LPCTSTR pStrGroupName);
    void RemoveOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
    void RemoveAllOptionGroups();

    CControlUI* GetFocus() const;
    void SetFocus(CControlUI* pControl);
    void SetFocusNeeded(CControlUI* pControl);

    bool SetNextTabControl(bool bForward = true);

    bool SetTimer(CControlUI* pControl, UINT nTimerID, UINT uElapse);
    bool KillTimer(CControlUI* pControl, UINT nTimerID);
    void KillTimer(CControlUI* pControl);
    void RemoveAllTimers();

    void SetCapture();
    void ReleaseCapture();
    bool IsCaptured();

    bool AddNotifier(INotifyUI* pControl);
    bool RemoveNotifier(INotifyUI* pControl);   
    void SendNotify(TNotifyUI& Msg, bool bAsync = false);
    void SendNotify(CControlUI* pControl, LPCTSTR pstrMessage, WPARAM wParam = 0, LPARAM lParam = 0, bool bAsync = false);

    bool AddPreMessageFilter(IMessageFilterUI* pFilter);
    bool RemovePreMessageFilter(IMessageFilterUI* pFilter);

    bool AddMessageFilter(IMessageFilterUI* pFilter);
    bool RemoveMessageFilter(IMessageFilterUI* pFilter);

    int GetPostPaintCount() const;
    bool AddPostPaint(CControlUI* pControl);
    bool RemovePostPaint(CControlUI* pControl);
    bool SetPostPaintIndex(CControlUI* pControl, int iIndex);

    void AddDelayedCleanup(CControlUI* pControl);

	void AddCustomDefaultAttributeList(LPCTSTR pStrControlName, LPCTSTR pStrControlAttrList);
	LPCTSTR GetCustomDefaultAttributeList(LPCTSTR pStrControlName) const;
	bool RemoveCustomDefaultAttributeList(LPCTSTR pStrControlName);
	HFONT AddCustomFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	HFONT AddCustomFontAt(int index, LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	HFONT GetCustomFont(int index,CPaintManagerUI * pManager);
	HFONT GetCustomFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	bool FindCustomFont(HFONT hFont);
	bool FindCustomFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	int GetCustomFontIndex(HFONT hFont);
	int GetCustomFontIndex(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	TFontInfo* GetCustomFontInfo(int index,CPaintManagerUI * pManager);
	TFontInfo* GetCustomFontInfo(HFONT hFont,CPaintManagerUI * pManager);
	bool RemoveCustomFont(HFONT hFont);
	bool RemoveCustomFontAt(int index);
	void RemoveAllCustomFonts();

	bool AddTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator);
	bool RemoveTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator);
	bool TranslateAccelerator(LPMSG pMsg);

    CControlUI* GetRoot() const;
    void RemoveRoot();
    CControlUI* FindControl(POINT pt) const;
    CControlUI* FindControl(LPCTSTR pstrName) const;
    CControlUI* FindSubControlByPoint(CControlUI* pParent, POINT pt) const;
    CControlUI* FindSubControlByName(CControlUI* pParent, LPCTSTR pstrName) const;
    CControlUI* FindSubControlByClass(CControlUI* pParent, LPCTSTR pstrClass, int iIndex = 0);
    CStdPtrArray* FindSubControlsByClass(CControlUI* pParent, LPCTSTR pstrClass);
    CStdPtrArray* GetSubControlsByClass();	

    static void MessageLoop();
    static bool TranslateMessage(const LPMSG pMsg);

	void DoPaint(CDC * pDC);
	void SetPaintNotify(IPaintNotifyUI * pIPaintNotifyUI);
    bool MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
    bool PreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
	void RemoveAll();

private:
    static CControlUI* CALLBACK __FindControlFromNameHash(CControlUI* pThis,LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromCount(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromPoint(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromTab(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromShortcut(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromUpdate(CControlUI* pThis,LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromName(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromClass(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlsFromClass(CControlUI* pThis, LPVOID pData);
};

/////////////////////////////////////////////////////////////////////////////////////
#endif
