#include "StdAfx.h"
#include "FGuimfcWindow.h"

///////////////////////////////////////////////////////////////////////////////////////////
//��Ϣ����
#define WM_QUIT_APP							(WM_USER+8976)

///////////////////////////////////////////////////////////////////////////////////////////
//��Ϣӳ��
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
//���캯��
CFGuiWnd::CFGuiWnd(void)
{
}

//��������
CFGuiWnd::~CFGuiWnd(void)
{
}

//��Ϣ����
LRESULT CFGuiWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if( uMsg == WM_ERASEBKGND ) 
    {
        return true;
    }

    return false;
}

//������Ϣ
LRESULT CFGuiWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//��������
	LRESULT lResult=TRUE;
	BOOL bHandled = FALSE;

	//��������
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:	//�������
		{
			lResult=HandleLButtonDown(uMsg,wParam,lParam,bHandled); 
			if(lResult==TRUE) return FALSE;
			break;
		}
	case WM_LBUTTONDBLCLK:	//���˫��
		{
			lResult=HandleLButtonDbClick(uMsg,wParam,lParam,bHandled); 
			if(lResult==TRUE) return FALSE;
			break;
		}
	case WM_NCHITTEST:		//�������	
		{
			lResult = HandleNcHitTest(uMsg, wParam, lParam, bHandled); 
			break;
		}
	case WM_SIZE:			//�ı��С
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

//������Ϣ
int CFGuiWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
        return -1;

	//���ñ���
    m_PaintManager.Init(m_hWnd);

	m_PaintManager.AddPreMessageFilter(this);
    m_PaintManager.SetPaintNotify(this);	

	//��������
	CDialogBuilder Builder;
	STRINGorID xml(GetSkinFile());

	//�����ؼ�
	CControlUI* pRoot = Builder.Create(xml,0,this,&m_PaintManager);
	if(pRoot==NULL)
	{
		//�������ڵ�
		pRoot = static_cast<CControlUI *>(new CContainerUI());
		pRoot->SetManager(&m_PaintManager,NULL);
	}	
	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);

	//��ʼ�ؼ�
	InitControlUI();

    return 0;
}

//������Ϣ
void CFGuiWnd::OnNcDestroy()
{
	m_PaintManager.RemovePreMessageFilter(this);
	m_PaintManager.RemoveNotifier(this);
	m_PaintManager.RemoveAllTimers();
	m_PaintManager.ReapObjects(m_PaintManager.GetRoot());

	__super::OnNcDestroy();	
}

//��ȡ����
CControlUI * CFGuiWnd::GetControlByName(LPCTSTR pszControlName)
{
	return m_PaintManager.FindControl(pszControlName);
}

//�������
bool CFGuiWnd::DoCaptionHitTest(POINT Point)
{
	//��ȡ����
	RECT rcClient;
	GetClientRect(&rcClient);

	//�ؼ�����
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

//�Զ�����Ϣ
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

//��ť����
LRESULT CFGuiWnd::HandleNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//��������
	POINT pt = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)};
	ScreenToClient(&pt);

	//��ȡ����
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

	//������
	if(DoCaptionHitTest(pt)==true) return HTCAPTION;

	return HTCLIENT;
}

//�������
LRESULT CFGuiWnd::HandleLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//��ȡλ��
	POINT Point = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)}; 

	//�������
	if(DoCaptionHitTest(Point)==false) return FALSE;

	//���ô���
	if ((IsZoomed()==FALSE))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	//���ô���
	bHandled=TRUE;

	return TRUE;
}

//���˫��
LRESULT CFGuiWnd::HandleLButtonDbClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//��ȡλ��
	POINT Point = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)}; 

	//�������
	if(DoCaptionHitTest(Point)==false) return FALSE;

	//���ô���
	bHandled=TRUE;

	return TRUE;
}

//��Ϣת��
BOOL CFGuiWnd::PreTranslateMessage(MSG* pMsg)
{
	if(CPaintManagerUI::TranslateMessage(pMsg)==true)
	{
		return TRUE;
	}
	
	return __super::PreTranslateMessage(pMsg);
}

//���ڹ���
LRESULT CFGuiWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//��������
	LRESULT lResult=HandleMessage(message, wParam, lParam);
	if(lResult==FALSE)
	{
		return __super::WindowProc(message,wParam,lParam);
	}

	return lResult;
}


///////////////////////////////////////////////////////////////////////////////////////////
//���캯��
CFGuiFrameWnd::CFGuiFrameWnd(void)
{
}

//��������
CFGuiFrameWnd::~CFGuiFrameWnd(void)
{
}

//��Ϣ����
LRESULT CFGuiFrameWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if( uMsg == WM_ERASEBKGND ) 
    {
        return true;
    }

    return false;
}

//������Ϣ
LRESULT CFGuiFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//��������
	LRESULT lResult=TRUE;
	BOOL bHandled = FALSE;

	//��������
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:	//�������
		{
			lResult=HandleLButtonDown(uMsg,wParam,lParam,bHandled); 
			if(lResult==TRUE) return FALSE;
			break;
		}
	case WM_LBUTTONDBLCLK:	//���˫��
		{
			lResult=HandleLButtonDbClick(uMsg,wParam,lParam,bHandled); 
			if(lResult==TRUE) return FALSE;
			break;
		}
	case WM_NCHITTEST:		//�������	
		{
			lResult = HandleNcHitTest(uMsg, wParam, lParam, bHandled); 
			break;
		}
	case WM_SIZE:			//�ı��С
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

//������Ϣ
int CFGuiFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
        return -1;

	//���ñ���
    m_PaintManager.Init(m_hWnd);
    
	m_PaintManager.AddPreMessageFilter(this);
	m_PaintManager.SetPaintNotify(this);

	//��������
	CDialogBuilder Builder;
	STRINGorID xml(GetSkinFile());

	//�����ؼ�
	CControlUI* pRoot = Builder.Create(xml,0,this,&m_PaintManager);
	if(pRoot==NULL)
	{
		//�������ڵ�
		pRoot = static_cast<CControlUI *>(new CContainerUI());
		pRoot->SetManager(&m_PaintManager,NULL);
	}	
	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);

	//��ʼ�ؼ�
	InitControlUI();

    return 0;
}

//������Ϣ
void CFGuiFrameWnd::OnNcDestroy()
{
	m_PaintManager.RemovePreMessageFilter(this);
	m_PaintManager.RemoveNotifier(this);
	m_PaintManager.RemoveAllTimers();
	m_PaintManager.ReapObjects(m_PaintManager.GetRoot());

	__super::OnNcDestroy();	
}


//�������
bool CFGuiFrameWnd::DoCaptionHitTest(POINT Point)
{
	//��ȡ����
	RECT rcClient;
	GetClientRect(&rcClient);

	//�ؼ�����
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

//��ȡ����
CControlUI * CFGuiFrameWnd::GetControlByName(LPCTSTR pszControlName)
{
	return m_PaintManager.FindControl(pszControlName);
}

//�Զ�����Ϣ
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

//��ť����
LRESULT CFGuiFrameWnd::HandleNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//��������
	POINT pt = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)};
	ScreenToClient(&pt);

	//��ȡ����
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

	//������
	if(DoCaptionHitTest(pt)==true) return HTCAPTION;

	return HTCLIENT;
}

//�������
LRESULT CFGuiFrameWnd::HandleLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//��ȡλ��
	POINT Point = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)}; 

	//�������
	if(DoCaptionHitTest(Point)==false) return FALSE;

	//���ô���
	if ((IsZoomed()==FALSE))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	//���ô���
	bHandled=TRUE;

	return TRUE;
}

//���˫��
LRESULT CFGuiFrameWnd::HandleLButtonDbClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//��ȡλ��
	POINT Point = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)}; 

	//�������
	if(DoCaptionHitTest(Point)==false) return FALSE;

	//���ô���
	bHandled=TRUE;

	return TRUE;
}

//��Ϣת��
BOOL CFGuiFrameWnd::PreTranslateMessage(MSG* pMsg)
{
	if(CPaintManagerUI::TranslateMessage(pMsg)==true)
	{
		return TRUE;
	}
	
	return __super::PreTranslateMessage(pMsg);
}

//���ڹ���
LRESULT CFGuiFrameWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//�˳�����
	if(message==WM_QUIT_APP)
	{
		PostQuitMessage(0);

		return TRUE;
	}
	
	if(message == WM_NCPAINT || message == WM_NCACTIVATE) 
	{
		return TRUE;
	}
	//��������
	LRESULT lResult=HandleMessage(message, wParam, lParam);
	if(lResult==FALSE)
	{
		return __super::WindowProc(message,wParam,lParam);
	}

	return lResult;
}

///////////////////////////////////////////////////////////////////////////////////////////
//���캯��
CFGuiDialog::CFGuiDialog(void)
{
}

//���캯��
CFGuiDialog::CFGuiDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd):CDialog(lpszTemplateName,pParentWnd)
{
}

//���캯��
CFGuiDialog::CFGuiDialog(UINT nIDTemplate, CWnd* pParentWnd):CDialog(nIDTemplate,pParentWnd)
{
}

//��������
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

//��Ϣ����
LRESULT CFGuiDialog::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if(uMsg == WM_CHAR) return HandleMessage(uMsg, wParam, lParam);
	return FALSE;
}

//������Ϣ
LRESULT CFGuiDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//��������
	LRESULT lResult=TRUE;
	BOOL bHandled = FALSE;

	//��������
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:	//�������
		{
			lResult=HandleLButtonDown(uMsg,wParam,lParam,bHandled); 
			if(lResult==TRUE) return FALSE;
			break;
		}
	case WM_LBUTTONDBLCLK:	//���˫��
		{
			lResult=HandleLButtonDbClick(uMsg,wParam,lParam,bHandled); 
			if(lResult==TRUE) return FALSE;
			break;
		}
	case WM_NCHITTEST:		//�������	
		{
			lResult = HandleNcHitTest(uMsg, wParam, lParam, bHandled); 
			break;
		}
	case WM_SIZE:			//�ı��С
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

//������Ϣ
int CFGuiDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
        return -1;

	//���ñ���
    m_PaintManager.Init(m_hWnd);
    
	m_PaintManager.AddPreMessageFilter(this);
	m_PaintManager.SetPaintNotify(this);

	//��������
	CDialogBuilder Builder;
	STRINGorID xml(GetSkinFile());

	//�����ؼ�
	CControlUI* pRoot = Builder.Create(xml,0,this,&m_PaintManager);
	if(pRoot==NULL)
	{
		//�������ڵ�
		pRoot = static_cast<CControlUI *>(new CContainerUI());
		pRoot->SetManager(&m_PaintManager,NULL);
	}	
	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);

	//��ʼ�ؼ�
	InitControlUI();

    return 0;
}

//������Ϣ
VOID CFGuiDialog::OnNcDestroy()
{
	m_PaintManager.RemovePreMessageFilter(this);
	m_PaintManager.RemoveNotifier(this);
	m_PaintManager.RemoveAllTimers();
	m_PaintManager.ReapObjects(m_PaintManager.GetRoot());	

	__super::OnNcDestroy();
}

//�������
bool CFGuiDialog::DoCaptionHitTest(POINT Point)
{
	//��ȡ����
	RECT rcClient;
	GetClientRect(&rcClient);

	//�ؼ�����
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

//��ȡ����
CControlUI * CFGuiDialog::GetControlByName(LPCTSTR pszControlName)
{
	return m_PaintManager.FindControl(pszControlName);
}

//��ť����
LRESULT CFGuiDialog::HandleNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//��������
	POINT pt = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)};
	ScreenToClient(&pt);

	//��ȡ����
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

	//������
	if(DoCaptionHitTest(pt)==true) return HTCAPTION;

	return HTCLIENT;
}

//�������
LRESULT CFGuiDialog::HandleLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//��ȡλ��
	POINT Point = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)}; 

	//�������
	if(DoCaptionHitTest(Point)==false) return FALSE;

	//���ô���
	if ((IsZoomed()==FALSE))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	//���ô���
	bHandled=TRUE;

	return TRUE;
}

//���˫��
LRESULT CFGuiDialog::HandleLButtonDbClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//��ȡλ��
	POINT Point = {GET_L_LPARAM(lParam),GET_H_LPARAM(lParam)}; 

	//�������
	if(DoCaptionHitTest(Point)==false) return FALSE;

	//���ô���
	bHandled=TRUE;

	return TRUE;
}

//�Զ�����Ϣ
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

//��Ϣת��
BOOL CFGuiDialog::PreTranslateMessage(MSG* pMsg)
{
	if(CPaintManagerUI::TranslateMessage(pMsg)==true)
	{
		return TRUE;
	}
	
	return __super::PreTranslateMessage(pMsg);
}

//���ڹ���
LRESULT CFGuiDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//��������
	LRESULT lHandled = HandleMessage(message, wParam, lParam);
	if(lHandled==FALSE)
	{
		return __super::WindowProc(message,wParam,lParam);
	}

	return lHandled;
}

///////////////////////////////////////////////////////////////////////////////////////////