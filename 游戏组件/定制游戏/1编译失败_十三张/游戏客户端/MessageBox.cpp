#include "Stdafx.h"
#include "MessageBox.h"
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMessageBox, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
#define IDI_MSG_SHOW_TIME							1
//////////////////////////////////////////////////////////////////////////

//构造函数
CMessageBox::CMessageBox() : CDialog(IDD_MESSAGE_BOX)
{
	m_szMsg = TEXT("");
	m_nTimerCount = 99;

	HINSTANCE hInst = AfxGetInstanceHandle();
	m_ImageBack.LoadFromResource(hInst ,IDB_MESSAGEBOX_BACK);
	
}

//析构函数
CMessageBox::~CMessageBox()
{
}

//控件绑定
void CMessageBox::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btClose);
}

//初始化函数
BOOL CMessageBox::OnInitDialog()
{
	__super::OnInitDialog();

	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//创建区域
	CRgn RgnControl;
	m_ImageBack.CreateImageRegion(RgnControl,RGB(255,0,255));

	//设置窗口
	if (RgnControl.GetSafeHandle()!=NULL)
	{
		//移动窗口
		SetWindowRgn(RgnControl,TRUE);
		SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);
	}
	m_btOK.SetButtonImage(IDB_BT_MSGBOX_OK,AfxGetInstanceHandle(),false,false);
	m_btClose.SetButtonImage(IDB_BT_MSGBOX_CANCEL,AfxGetInstanceHandle(),false,false);

	//调整按钮
	CRect rcClient;
	GetClientRect(&rcClient);
	CRect rcBtClose;
	m_btClose.GetWindowRect(&rcBtClose);
	m_btOK.SetWindowPos(NULL,rcClient.Width()/2-rcBtClose.Width()-10,rcClient.Height()-rcBtClose.Height()-10,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btClose.SetWindowPos(NULL,rcClient.Width()/2+10,rcClient.Height()-rcBtClose.Height()-10,0,0,SWP_NOSIZE|SWP_NOZORDER);


	////设置透明
	//ModifyStyleEx(0,0x00080000);
	//HINSTANCE hInst = LoadLibrary(TEXT("User32.DLL")); 
	//if(hInst) 
	//{ 
	//	typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
	//	MYFUNC fun = NULL;
	//	//取得SetLayeredWindowAttributes（）函数指针 
	//	fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
	//	if(fun)fun(this->GetSafeHwnd(),0,220,2); 
	//	FreeLibrary(hInst); 
	//}

	return TRUE;
}

//设置积分
void CMessageBox::ShowMessage(LPCTSTR pszMsg)
{
	//设置变量
	m_szMsg.Format(TEXT("%s"), pszMsg);

	if (m_szMsg.GetLength() != 0)
	{
		m_nTimerCount = 99;
		SetTimer(IDI_MSG_SHOW_TIME, 1000, NULL);

		Invalidate(FALSE);
		UpdateWindow();

		ShowWindow(SW_SHOW);
	}
	else ShowWindow(SW_HIDE);
	
	return;
}

//确定按钮
void CMessageBox::OnOK()
{
	KillTimer(IDI_MSG_SHOW_TIME);

	CGameFrameView *gameFrameView = CGameFrameView::GetInstance();
	if (gameFrameView != NULL)
		gameFrameView->PostEngineMessage(IDM_MSG_INFO, 1, 0);

	__super::OnOK();
}

//取消按钮
void CMessageBox::OnCancel()
{
	KillTimer(IDI_MSG_SHOW_TIME);

	CGameFrameView *gameFrameView = CGameFrameView::GetInstance();
	if (gameFrameView != NULL)
		gameFrameView->PostEngineMessage(IDM_MSG_INFO, 0, 0);

	__super::OnCancel();
}


//重画函数
void CMessageBox::OnPaint()
{
	CPaintDC dc(this);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建缓冲
	CDC DCBuffer;
	CBitmap ImageBuffer;
	DCBuffer.CreateCompatibleDC(&dc);
	ImageBuffer.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//设置 DC
	DCBuffer.SetBkMode(TRANSPARENT);
	DCBuffer.SelectObject(&ImageBuffer);

	//绘画背景
	if (m_ImageBack.IsNull()==false) m_ImageBack.BitBlt(DCBuffer,0,0);
	
	//文字颜色
	DCBuffer.SetTextColor(RGB(255,255,255));

	//绘画文字
	CRect rcText(20, 45, m_ImageBack.GetWidth()-20, m_ImageBack.GetHeight()-20);

	DrawText(DCBuffer.GetSafeHdc(), m_szMsg, -1, &rcText, DT_LEFT|DT_WORDBREAK|DT_EDITCONTROL);

	//绘画倒计时
	if (m_nTimerCount != 0)
	{
		rcText.left = 150;
		rcText.top = 9;
		rcText.right = 200 ;
		rcText.bottom = 40;
		TCHAR szBuffer[32] = TEXT("");
		_sntprintf(szBuffer, CountArray(szBuffer), TEXT("(%d)"), m_nTimerCount);
		DrawText(DCBuffer.GetSafeHdc(), szBuffer, -1, &rcText, DT_LEFT);
	}

	//绘画界面
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&DCBuffer,0,0,SRCCOPY);

	//清理资源
	DCBuffer.DeleteDC();
	ImageBuffer.DeleteObject();

	return;
}

//鼠标消息
void CMessageBox::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//消息模拟
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

	return;
}

//背景函数
BOOL CMessageBox::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//定时函数
void CMessageBox::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == IDI_MSG_SHOW_TIME)
	{
		m_nTimerCount--;

		if (m_nTimerCount == 0)
		{
			KillTimer(IDI_MSG_SHOW_TIME);
			OnCancel();
		}

		Invalidate(FALSE);
		UpdateWindow();

		return;
	}
}
//////////////////////////////////////////////////////////////////////////
