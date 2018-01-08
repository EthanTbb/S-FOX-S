
#ifndef __WKE_BROWSER_H__
#define __WKE_BROWSER_H__

#pragma once

///////////////////////////////////////////  
//网页加载状态改变的回调  
class CWkeWebkitLoadCallback  
{  
public:  
	virtual void    OnLoadFailed()=0;  
	virtual void    OnLoadComplete()=0;  
	virtual void    OnDocumentReady()=0;  
};  

///////////////////////////////////////////  
//网页标题、地址改变的回调  
class CWkeWebkitClientCallback  
{  
public:  
	virtual void    OnTitleChange(const CStdString& strTitle)=0;  
	virtual void    OnUrlChange(const CStdString& strUrl)=0;  
}; 

////////////////////////////////////////

class FGUI_LIB_CLASS CWkeBrowserUI : public CControlUI/*,public wkeBufHandler */ 
{  
public:  
	CWkeBrowserUI(void);  
	~CWkeBrowserUI(void);  
	virtual LPCTSTR GetClass()const;  
	virtual LPVOID  GetInterface(LPCTSTR pstrName);  
	virtual void    DoEvent(TEventUI& event);  
	virtual void    DoPaint(HDC hDC, const RECT& rcPaint);  
	virtual void    SetPos(RECT rc);  
	virtual void    DoInit();  
	virtual void    SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);  
	//////////////////////////////////////  
	const   wstring& GetUrl()const ;  
	bool    CanGoBack() const;  
	bool    GoBack();  
	bool    CanGoForward() const;  
	bool    GoForward();  
	void    StopLoad();  
	void    Refresh();  
	wkeWebView  GetWebView();  
	void    SetLoadCallback(CWkeWebkitLoadCallback* pCallback);  
	CWkeWebkitLoadCallback* GetLoadCallback();  
	void    Navigate(LPCTSTR lpUrl);  
	void    LoadFile(LPCTSTR lpFile);  
	void    LoadHtml(LPCTSTR lpHtml);
	wstring RunJS(wstring strValue);
	static void	JSBindFunction(const char* name, jsNativeFunction fn, unsigned int argCount);
	static LPCTSTR JSBindGetArg( jsExecState es,int index);
protected: 
	void    StartCheckThread();  
	void    StopCheckThread(); 
	static  void OnPaintUpdated(wkeWebView webView, void* param, const void* hdc, int x, int y, int cx, int cy);
	static  void OnTitleChange(const struct _wkeClientHandler* clientHandler, const wkeString title);  
	static  void OnUrlChange(const struct _wkeClientHandler* clientHandler, const wkeString url);  
private:  
	static int  m_bWebkitCount;  
	HANDLE      m_hCheckThread;  
	wstring     m_strUrl;  
	wkeWebView  m_pWebView;  
	CWkeWebkitLoadCallback*     m_pLoadCallback;  
	CWkeWebkitClientCallback*   m_pClientCallback;  
}; 

#endif