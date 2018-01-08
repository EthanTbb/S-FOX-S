#include "StdAfx.h"
#include "FGuimfcWindow.h"

///////////////////////////////////////////////////////////////////////////////////////////
//消息定义
#define WM_QUIT_APP							(WM_USER+8976)

///////////////////////////////////////////////////////////////////////////////////////////
//消息映射
BEGIN_MESSAGE_MAP(CFGuiWnd, CWnd)
 ON_WM_CREATE()
 ON_WM_NCDESTROY()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CFGuiDialog, CDialog)
 ON_WM_CREATE()
 ON_WM_NCDESTROY()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CFGuiFrameWnd, CFrameWnd)
 ON_WM_CREATE()
 ON_WM_NCDESTROY()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////////////////

#define GET_L_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_H_LPARAM(lp)                        ((int)(short)HIWORD(lp))

///////////////////////////////////////////////////////////////////////////////////////////
//构造函数
CFGuiWnd::CFGuiWnd(void)
{
}

//析构函数
CFGuiWnd::~CFGuiWnd(void)
{
}

//消息处理
LRESULT CFGuiWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if( uMsg == WM_ERASEBKGND ) 
    {
        return true;
    }

    return false;
}

//处理消息
LRESULT CFGuiWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//变量定义
	LRESULT lResult=TRUE;
	BOOL bHandled = FALSE;

	//按键测试
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:	//左键按下
		{
			lResult=HandleLButtonDown(uMsg,wParam,lParam,bHandled); 
			if(lResult==TRUE) return FALSE;
			break;
		}
	case WM_LBUTTONDBLCLK:	//左键双击
		{
			lResult=HandleLButtonDbClick(uMsg,wParam,lParam,bHandled); 
			if(lResult==TRUE) return FALSE;
			break;
		}
	case WM_NCHITTEST:		//点击测试	
		{
			lResult = HandleNcHitTest(uMsg, wParam, lParam, bHandled); 
			break;
		}
	case WM_SIZE:			//改变大小
		{
			lResult = HandleOnSizeMessage(uMsg, wParam, lParam, bHandled); 
			break;
		}
	}
	if (bHandled) return lResult;
	lResult = HandleCustomMessage(uMsg, wParam, lParam, bHandled);
	if (bHandled) return lResult;
	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam,lResult)) return lResult;

	return FALSE;
}

//创建消息
int CFGuiWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
        return -1;

	//设置变量
    m_PaintManager.Init(m_hWnd);

	m_PaintManager.AddPreMessageFilter(this);
    m_PaintManager.SetPaintNotify(this);	

	//变量定义
	CDialogBuilder Builder;
	STRINGorID xml(GetSkinFile());

	//创建控件
	CControlUI* pRoot = Builder.Create(xml,0,this,&m_PaintManager);
	if(pRoot==NULL)
	{
		//创建根节点
		pRoot = static_cast<CControlUI *>(new CContainerUI());
		pRoot->SetManager(&m_PaintManager,NULL);
	}	
	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);

	//初始控件
	InitControlUI();

    return 0;
}

//销毁消息
void CFGuiWnd::OnNcDestroy()
{
	m_PaintManager.RemovePreMessageFilter(this);
	m_PaintManager.RemoveNotifier(this);
	m_PaintManager.RemoveAllTimers();
	m_PaintManager.ReapObjects(m_PaintManager.GetRoot());

	__super::OnNcDestroy();	
}

//获取对象
CControlUI * CFGuiWnd::GetControlByName(LPCTSTR pszControlName)
{
	return m_PaintManager.FindControl(pszControlName);
}

//点击测试
bool CFGuiWnd::DoCaptionHitTest(POINT Point)
{
	//获取区域
	RECT rcClient;
	GetClientRect(&rcClient);

	//控件过滤
	RECT rcCaption = m_PaintManager.GetCaptionRect();
	if( Point.x >= rcClient.left + rcCaption.left && Point.x < rcClient.right - rcCaption.right \
		&& Point.y >= rcCaption.top && Point.y < rcCaption.bottom ) 
	{
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(Point));
			if( pControl && _tcsicmp(pControl->GetClass(), TEXT("ButtonUI")) != 0 && 
				lstrcmp(pControl->GetClass(), TEXT("TabButtonUI")) != 0 &&
				lstrcmp(pControl->GetClass(), TEXT("CheckButtonUI")) != 0 &&
				lstrcmp(pControl->GetClass(), TEXT("RadioButtonUI")) != 0 &&		
				lstrcmp(pControl->GetClass(), TEXT("OptionUI")) != 0 &&
				lstrcmp(pControl->GetClass(), TEXT("FadeButtonUI")) != 0 &&
				lstrcmp(pControl->GetClass(), TEXT("TextUI")) != 0 )
				return true;
	}

	return false;
}

//自定义消息
LRESULT CFGuiWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 1L;
}

LRESULT CFGuiWnd::HandleOnSizeMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_PaintManager.GetRoundCorner();
#if defined(WIN32) && !defined(UNDER_CE)
	if( !::IsIconic(*this) && (szRoundCorner.cx > 0 || szRoundCorner.cy > 0) ) {
		CRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.OffsetRect(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}
#endif
	bHandled = FALSE;
	return 0L;
}

//按钮测试
LRESULT CFGuiWnd::HandleNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//变量定义
	POINT pt = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)};
	ScreenToClient(&pt);

	//获取区域
	RECT rcClient;
	GetClientRect(&rcClient);

	if( !IsZoomed() ) 
	{
		RECT rcSizeBox = m_PaintManager.GetSizeBox();
		if( pt.y < rcClient.top + rcSizeBox.top ) 
		{
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
			return HTTOP;
		}
		else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) 
		{
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}
		if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
		if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	}

	//标题点击
	if(DoCaptionHitTest(pt)==true) return HTCAPTION;

	return HTCLIENT;
}

//左键按下
LRESULT CFGuiWnd::HandleLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//获取位置
	POINT Point = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)}; 

	//点击测试
	if(DoCaptionHitTest(Point)==false) return FALSE;

	//设置处理
	if ((IsZoomed()==FALSE))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	//设置处理
	bHandled=TRUE;

	return TRUE;
}

//左键双击
LRESULT CFGuiWnd::HandleLButtonDbClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//获取位置
	POINT Point = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)}; 

	//点击测试
	if(DoCaptionHitTest(Point)==false) return FALSE;

	//设置处理
	bHandled=TRUE;

	return TRUE;
}

//消息转换
BOOL CFGuiWnd::PreTranslateMessage(MSG* pMsg)
{
	if(CPaintManagerUI::TranslateMessage(pMsg)==true)
	{
		return TRUE;
	}
	
	return __super::PreTranslateMessage(pMsg);
}

//窗口过程
LRESULT CFGuiWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//变量定义
	LRESULT lResult=HandleMessage(message, wParam, lParam);
	if(lResult==FALSE)
	{
		return __super::WindowProc(message,wParam,lParam);
	}

	return lResult;
}


///////////////////////////////////////////////////////////////////////////////////////////
//构造函数
CFGuiFrameWnd::CFGuiFrameWnd(void)
{
}

//析构函数
CFGuiFrameWnd::~CFGuiFrameWnd(void)
{
}

//消息处理
LRESULT CFGuiFrameWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if( uMsg == WM_ERASEBKGND ) 
    {
        return true;
    }

    return false;
}

//处理消息
LRESULT CFGuiFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//变量定义
	LRESULT lResult=TRUE;
	BOOL bHandled = FALSE;

	//按键测试
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:	//左键按下
		{
			lResult=HandleLButtonDown(uMsg,wParam,lParam,bHandled); 
			if(lResult==TRUE) return FALSE;
			break;
		}
	case WM_LBUTTONDBLCLK:	//左键双击
		{
			lResult=HandleLButtonDbClick(uMsg,wParam,lParam,bHandled); 
			if(lResult==TRUE) return FALSE;
			break;
		}
	case WM_NCHITTEST:		//点击测试	
		{
			lResult = HandleNcHitTest(uMsg, wParam, lParam, bHandled); 
			break;
		}
	case WM_SIZE:			//改变大小
		{
			lResult = HandleOnSizeMessage(uMsg, wParam, lParam, bHandled); 
			break;
		}
	}
	if (bHandled) return lResult;
	lResult = HandleCustomMessage(uMsg, wParam, lParam, bHandled);
	if (bHandled) return lResult;
	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam,lResult)) return lResult;

	return FALSE;
}

//创建消息
int CFGuiFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
        return -1;

	//设置变量
    m_PaintManager.Init(m_hWnd);
    
	m_PaintManager.AddPreMessageFilter(this);
	m_PaintManager.SetPaintNotify(this);

	//变量定义
	CDialogBuilder Builder;
	STRINGorID xml(GetSkinFile());

	//创建控件
	CControlUI* pRoot = Builder.Create(xml,0,this,&m_PaintManager);
	if(pRoot==NULL)
	{
		//创建根节点
		pRoot = static_cast<CControlUI *>(new CContainerUI());
		pRoot->SetManager(&m_PaintManager,NULL);
	}	
	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);

	//初始控件
	InitControlUI();

    return 0;
}

//销毁消息
void CFGuiFrameWnd::OnNcDestroy()
{
	m_PaintManager.RemovePreMessageFilter(this);
	m_PaintManager.RemoveNotifier(this);
	m_PaintManager.RemoveAllTimers();
	m_PaintManager.ReapObjects(m_PaintManager.GetRoot());

	__super::OnNcDestroy();	
}


//点击测试
bool CFGuiFrameWnd::DoCaptionHitTest(POINT Point)
{
	//获取区域
	RECT rcClient;
	GetClientRect(&rcClient);

	//控件过滤
	RECT rcCaption = m_PaintManager.GetCaptionRect();
	if( Point.x >= rcClient.left + rcCaption.left && Point.x < rcClient.right - rcCaption.right \
		&& Point.y >= rcCaption.top && Point.y < rcCaption.bottom ) 
	{
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(Point));
			if( pControl && lstrcmp(pControl->GetClass(), TEXT("ButtonUI")) != 0 &&
				lstrcmp(pControl->GetClass(), TEXT("TabButtonUI")) != 0 &&
				lstrcmp(pControl->GetClass(), TEXT("CheckButtonUI")) != 0 &&
				lstrcmp(pControl->GetClass(), TEXT("RadioButtonUI")) != 0 &&	
				lstrcmp(pControl->GetClass(), TEXT("OptionUI")) != 0 &&
				lstrcmp(pControl->GetClass(), TEXT("FadeButtonUI")) != 0 &&
				lstrcmp(pControl->GetClass(), TEXT("TextUI")) != 0 )
				return true;
	}

	return false;
}

//获取对象
CControlUI * CFGuiFrameWnd::GetControlByName(LPCTSTR pszControlName)
{
	return m_PaintManager.FindControl(pszControlName);
}

//自定义消息
LRESULT CFGuiFrameWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 1L;
}

LRESULT CFGuiFrameWnd::HandleOnSizeMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_PaintManager.GetRoundCorner();
#if defined(WIN32) && !defined(UNDER_CE)
	if( !::IsIconic(*this) && (szRoundCorner.cx > 0 || szRoundCorner.cy > 0) ) {
		CRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.OffsetRect(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}
#endif
	bHandled = FALSE;
	return 0L;
}

//按钮测试
LRESULT CFGuiFrameWnd::HandleNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//变量定义
	POINT pt = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)};
	ScreenToClient(&pt);

	//获取区域
	RECT rcClient;
	GetClientRect(&rcClient);

	if( !IsZoomed() ) 
	{
		RECT rcSizeBox = m_PaintManager.GetSizeBox();
		if( pt.y < rcClient.top + rcSizeBox.top ) 
		{
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
			return HTTOP;
		}
		else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) 
		{
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}
		if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
		if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	}

	//标题点击
	if(DoCaptionHitTest(pt)==true) return HTCAPTION;

	return HTCLIENT;
}

//左键按下
LRESULT CFGuiFrameWnd::HandleLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//获取位置
	POINT Point = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)}; 

	//点击测试
	if(DoCaptionHitTest(Point)==false) return FALSE;

	//设置处理
	if ((IsZoomed()==FALSE))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	//设置处理
	bHandled=TRUE;

	return TRUE;
}

//左键双击
LRESULT CFGuiFrameWnd::HandleLButtonDbClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//获取位置
	POINT Point = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)}; 

	//点击测试
	if(DoCaptionHitTest(Point)==false) return FALSE;

	//设置处理
	bHandled=TRUE;

	return TRUE;
}

//消息转换
BOOL CFGuiFrameWnd::PreTranslateMessage(MSG* pMsg)
{
	if(CPaintManagerUI::TranslateMessage(pMsg)==true)
	{
		return TRUE;
	}
	
	return __super::PreTranslateMessage(pMsg);
}

//窗口过程
LRESULT CFGuiFrameWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//退出程序
	if(message==WM_QUIT_APP)
	{
		PostQuitMessage(0);

		return TRUE;
	}
	
	if(message == WM_NCPAINT || message == WM_NCACTIVATE) 
	{
		return TRUE;
	}
	//变量定义
	LRESULT lResult=HandleMessage(message, wParam, lParam);
	if(lResult==FALSE)
	{
		return __super::WindowProc(message,wParam,lParam);
	}

	return lResult;
}

///////////////////////////////////////////////////////////////////////////////////////////
//构造函数
CFGuiDialog::CFGuiDialog(void)
{
}

//构造函数
CFGuiDialog::CFGuiDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd):CDialog(lpszTemplateName,pParentWnd)
{
}

//构造函数
CFGuiDialog::CFGuiDialog(UINT nIDTemplate, CWnd* pParentWnd):CDialog(nIDTemplate,pParentWnd)
{
}

//析构函数
CFGuiDialog::~CFGuiDialog(void)
{
}

INT_PTR CFGuiDialog::DoModal()
{
	m_PaintManager.AddCloseCallBack(this, this);
	INT_PTR re = __super::DoModal();
	m_PaintManager.RemoveCloseCallBack(this);
	return re;
}

//消息处理
LRESULT CFGuiDialog::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if(uMsg == WM_CHAR) return HandleMessage(uMsg, wParam, lParam);
	return FALSE;
}

//处理消息
LRESULT CFGuiDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//变量定义
	LRESULT lResult=TRUE;
	BOOL bHandled = FALSE;

	//按键测试
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:	//左键按下
		{
			lResult=HandleLButtonDown(uMsg,wParam,lParam,bHandled); 
			if(lResult==TRUE) return FALSE;
			break;
		}
	case WM_LBUTTONDBLCLK:	//左键双击
		{
			lResult=HandleLButtonDbClick(uMsg,wParam,lParam,bHandled); 
			if(lResult==TRUE) return FALSE;
			break;
		}
	case WM_NCHITTEST:		//点击测试	
		{
			lResult = HandleNcHitTest(uMsg, wParam, lParam, bHandled); 
			break;
		}
	case WM_SIZE:			//改变大小
		{
			lResult = HandleOnSizeMessage(uMsg, wParam, lParam, bHandled); 
			break;
		}
	}
	if (bHandled) return lResult;
	lResult = HandleCustomMessage(uMsg, wParam, lParam, bHandled);
	if (bHandled) return lResult;
	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam,lResult)) return lResult;

	return FALSE;
}

LRESULT CFGuiDialog::CloseWindow()
{
	EndDialog(IDCANCEL);
	return IDCANCEL;
}

//创建消息
int CFGuiDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
        return -1;

	//设置变量
    m_PaintManager.Init(m_hWnd);
    
	m_PaintManager.AddPreMessageFilter(this);
	m_PaintManager.SetPaintNotify(this);

	//变量定义
	CDialogBuilder Builder;
	STRINGorID xml(GetSkinFile());

	//创建控件
	CControlUI* pRoot = Builder.Create(xml,0,this,&m_PaintManager);
	if(pRoot==NULL)
	{
		//创建根节点
		pRoot = static_cast<CControlUI *>(new CContainerUI());
		pRoot->SetManager(&m_PaintManager,NULL);
	}	
	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);

	//初始控件
	InitControlUI();

    return 0;
}

//销毁消息
VOID CFGuiDialog::OnNcDestroy()
{
	m_PaintManager.RemovePreMessageFilter(this);
	m_PaintManager.RemoveNotifier(this);
	m_PaintManager.RemoveAllTimers();
	m_PaintManager.ReapObjects(m_PaintManager.GetRoot());	

	__super::OnNcDestroy();
}

//点击测试
bool CFGuiDialog::DoCaptionHitTest(POINT Point)
{
	//获取区域
	RECT rcClient;
	GetClientRect(&rcClient);

	//控件过滤
	RECT rcCaption = m_PaintManager.GetCaptionRect();
	if( Point.x >= rcClient.left + rcCaption.left && Point.x < rcClient.right - rcCaption.right \
		&& Point.y >= rcCaption.top && Point.y < rcCaption.bottom ) 
	{
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(Point));
			if( pControl && _tcsicmp(pControl->GetClass(), TEXT("ButtonUI")) != 0 && 
				lstrcmp(pControl->GetClass(), TEXT("TabButtonUI")) != 0 &&
				lstrcmp(pControl->GetClass(), TEXT("CheckButtonUI")) != 0 &&
				lstrcmp(pControl->GetClass(), TEXT("RadioButtonUI")) != 0 &&		
				lstrcmp(pControl->GetClass(), TEXT("OptionUI")) != 0 &&
				lstrcmp(pControl->GetClass(), TEXT("FadeButtonUI")) != 0 &&
				lstrcmp(pControl->GetClass(), TEXT("TextUI")) != 0 )
				return true;
	}

	return false;
}

//获取对象
CControlUI * CFGuiDialog::GetControlByName(LPCTSTR pszControlName)
{
	return m_PaintManager.FindControl(pszControlName);
}

//按钮测试
LRESULT CFGuiDialog::HandleNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//变量定义
	POINT pt = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)};
	ScreenToClient(&pt);

	//获取区域
	RECT rcClient;
	GetClientRect(&rcClient);

	if( !IsZoomed() ) 
	{
		RECT rcSizeBox = m_PaintManager.GetSizeBox();
		if( pt.y < rcClient.top + rcSizeBox.top ) 
		{
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
			return HTTOP;
		}
		else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) 
		{
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}
		if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
		if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	}

	//标题点击
	if(DoCaptionHitTest(pt)==true) return HTCAPTION;

	return HTCLIENT;
}

//左键按下
LRESULT CFGuiDialog::HandleLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//获取位置
	POINT Point = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)}; 

	//点击测试
	if(DoCaptionHitTest(Point)==false) return FALSE;

	//设置处理
	if ((IsZoomed()==FALSE))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	//设置处理
	bHandled=TRUE;

	return TRUE;
}

//左键双击
LRESULT CFGuiDialog::HandleLButtonDbClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//获取位置
	POINT Point = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)}; 

	//点击测试
	if(DoCaptionHitTest(Point)==false) return FALSE;

	//设置处理
	bHandled=TRUE;

	return TRUE;
}

//自定义消息
LRESULT CFGuiDialog::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 1L;
}

LRESULT CFGuiDialog::HandleOnSizeMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_PaintManager.GetRoundCorner();
#if defined(WIN32) && !defined(UNDER_CE)
	if( !::IsIconic(*this) && (szRoundCorner.cx > 0 || szRoundCorner.cy > 0) ) {
		CRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.OffsetRect(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}
#endif
	bHandled = FALSE;
	return 0L;
}

//消息转换
BOOL CFGuiDialog::PreTranslateMessage(MSG* pMsg)
{
	if(CPaintManagerUI::TranslateMessage(pMsg)==true)
	{
		return TRUE;
	}
	
	return __super::PreTranslateMessage(pMsg);
}

//窗口过程
LRESULT CFGuiDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//变量定义
	LRESULT lHandled = HandleMessage(message, wParam, lParam);
	if(lHandled==FALSE)
	{
		return __super::WindowProc(message,wParam,lParam);
	}

	return lHandled;
}

///////////////////////////////////////////////////////////////////////////////////////////