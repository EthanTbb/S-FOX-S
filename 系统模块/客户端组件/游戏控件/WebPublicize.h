#ifndef WEB_PUBLICIZE_HEAD_FILE
#define WEB_PUBLICIZE_HEAD_FILE

#pragma once

//#include <mshtml.h>	
//#include "mshtmdid.h" 
#include "WebBrowser.h"
#include "ShareControlHead.h"

//#import <mshtml.tlb>

//////////////////////////////////////////////////////////////////////////////////

//浏览状态
#define BROWSER_STATUS_LOAD			0									//加载状态
#define BROWSER_STATUS_ERROR		1									//错误状态
#define BROWSER_STATUS_FINISH		2									//完成状态

//////////////////////////////////////////////////////////////////////////////////
//事件接口
interface IDocEventHanderSink
{
	//点击事件
	virtual VOID OnEventClicked()=NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//类型声明
class CWebPublicize;
class CDocEventHandler;

//////////////////////////////////////////////////////////////////////////////////
//
////事件处理
//class SHARE_CONTROL_CLASS CDocEventHandler : public CCmdTarget
//{
//	DECLARE_DYNAMIC(CDocEventHandler)
//
//	//接口变量
//protected:
//	IDocEventHanderSink *					m_pIDocEventHanderSink;				//回调接口
//
//	//函数定义
//public:
//	//构造函数
//	CDocEventHandler();
//	//析构函数
//	virtual ~CDocEventHandler();
//
//	//事件函数
//public:
//	//点击事件
//	VOID OnClick(MSHTML::IHTMLEventObjPtr pEvtObj);
//
//	DECLARE_MESSAGE_MAP()
//	DECLARE_DISPATCH_MAP()
//	DECLARE_INTERFACE_MAP()
//};

//////////////////////////////////////////////////////////////////////////////////

//广告控件
class SHARE_CONTROL_CLASS CWebPublicize : public CWnd
{
	//标识变量
protected:
	DWORD							m_dwDocCookie;						//文档甜饼
	COLORREF						m_crBackGround;						//背景颜色

	//状态变量
protected:
	WORD							m_wIndex;							//索引变量
	BYTE							m_cbStatus;							//浏览状态

	//控件变量
protected:
	CWebBrowser						m_WebBrowser;						//广告窗口
	//CDocEventHandler *				m_pEventHandler;					//事件处理

	//函数定义
public:
	//构造函数
	CWebPublicize();
	//析构函数
	virtual ~CWebPublicize();

	//信息接口
public:	
	//获取状态
	BYTE GetBrowserStatus() { return m_cbStatus; }
	//获取窗口
	CWebBrowser * GetWebBrowser() { return &m_WebBrowser; }

	//辅助函数
public:
	//设置颜色
	VOID SetBackGroundColor(COLORREF crBackGround);

	//浏览函数
public:
	//浏览函数
	VOID Navigate(LPCTSTR pszURL);
	//浏览函数
	VOID Navigate(LPCTSTR pszURL, VARIANT * Flags, VARIANT * TargetFrameName, VARIANT * PostData, VARIANT * Headers);

	//重置函数
protected:
	//状态通知
	virtual VOID OnBrowserStatus(BYTE cbStatus);
	//位置函数
	virtual VOID GetWebBrowserRect(CRect&rcWebBrowser);
	//绘画界面
	virtual VOID DrawPublicizeView(CDC * pDC, INT nWidth, INT nHeight, BYTE cbStatus);

	//消息函数
public:
	//销毁消息
	VOID OnDestroy();
	//重画消息
	VOID OnPaint();
	//时间消息
	VOID OnTimer(UINT nIDEvent);	
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);

	//辅助函数
public:
	//装载处理器
	VOID InstallEventHandler();
	//卸载处理器
	VOID UninstallEventHandler();

	//消息函数
protected:
	//导航完成
	VOID OnDocumentCompleteWeb(LPDISPATCH pDisp, VARIANT FAR * URL);
	//导航错误
	VOID OnNavigateErrorWeb(LPDISPATCH pDisp, VARIANT FAR * URL, VARIANT FAR * Frame, VARIANT FAR * StatusCode, BOOL FAR * Cancel);
	//导航开始
	VOID OnBeforeNavigate2Web(LPDISPATCH pDisp, VARIANT FAR * URL, VARIANT FAR * Flags, VARIANT FAR * TargetFrameName, VARIANT FAR * PostData, VARIANT FAR * Headers, BOOL FAR * Cancel);

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif