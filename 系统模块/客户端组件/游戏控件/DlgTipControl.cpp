#include "StdAfx.h"
#include "Resource.h"
#include "DlgTipControl.h"

//////////////////////////////////////////////////////////////////////////////////
//公共定义

//圆角大小
#define ROUND_CX					5									//圆角宽度
#define ROUND_CY					5									//圆角高度

//阴影定义
#define SHADOW_CX					6									//阴影宽度
#define SHADOW_CY					6									//阴影高度

//时钟定义
#define IDI_SHOW_WINDOW				100									//显示窗口
#define IDI_HIDE_WINDOW				101									//隐藏窗口

//////////////////////////////////////////////////////////////////////////////////
//静态变量
CTipControlMap					  CWndTipControl::m_TipControlMap;  

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CWndTipControl::CWndTipControl()
{
	//变量定义
	m_hWnd=NULL;
	m_bTrackMouse=false;
	m_nRelayTime=100;
	m_rcInvalidArea.SetRect(0,0,0,0);

	//句柄变量
	m_hFuncInst=NULL;
	ZeroMemory(&m_Blend,sizeof(m_Blend));

	//设置变量
	m_hInstance=NULL;
	m_szImageGround[0]=0;
}

//析构函数
CWndTipControl::~CWndTipControl()
{
	//销毁窗口
	if(m_hWnd!=NULL) 
	{
		DestroyWindow(m_hWnd);
	}

	//销毁动态库
	if(m_hFuncInst!=NULL)
	{
		FreeLibrary(m_hFuncInst);
	}
}

//显示测试
BOOL CWndTipControl::DetectIsShowWindow()
{
	if(!IsWindow(m_hWnd)) return FALSE;

	//设置定时器
	CPoint ptCursor;
	GetCursorPos(&ptCursor);

	return m_rcInvalidArea.PtInRect(ptCursor);
}

//隐藏探测
BOOL CWndTipControl::DetectIsHideWindow()
{
	//设置定时器
	CPoint ptCursor;
	GetCursorPos(&ptCursor);

	return m_rcInvalidArea.PtInRect(ptCursor)==FALSE; 
}

//绘画消息
VOID CWndTipControl::OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight)
{
	return;
}


//设置延时
VOID CWndTipControl::SetRelayTime(INT nRelayTime)
{
	//设置变量
	m_nRelayTime=nRelayTime;
}

//创建窗口
VOID CWndTipControl::CreateTipControl(INT nWidth,INT nHeight,INT nAlpha)
{
	//判断句柄
	if(IsWindow(m_hWnd)==TRUE) return;	

	//加载动态库
	m_hFuncInst = LoadLibrary(TEXT("User32.DLL")); 	
	if(m_hFuncInst)
	{
		m_pUpdateLayeredWindow=(LPUpdateLayeredWindow)GetProcAddress(m_hFuncInst,"UpdateLayeredWindow");
		if(m_pUpdateLayeredWindow == NULL) return;
	}

	//构造结构
	m_Blend.BlendOp=0;
	m_Blend.BlendFlags=0;
	m_Blend.AlphaFormat=AC_SRC_ALPHA;
	m_Blend.SourceConstantAlpha=nAlpha;

	//变量定义
	CRect rcCreate(0,0,0,0);

	//构造结构
	WNDCLASS wc = { 0 };
	wc.style = 0;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.lpfnWndProc = CWndTipControl::WindowProc;
	wc.hInstance = ::AfxGetInstanceHandle();
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = TEXT("WndTipControl");

	//注册类型
	if(::RegisterClass(&wc)!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS)
	{
		//创建窗口
		m_hWnd=CreateWindowEx(WS_EX_TOOLWINDOW|WS_EX_LAYERED,wc.lpszClassName,TEXT(""),WS_POPUPWINDOW|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,0, 0, 0, 0,NULL,NULL,NULL,NULL);

		if(IsWindow(m_hWnd)==TRUE)
		{
			//设置变量
			m_TipControlMap[m_hWnd]=this;

			//设置大小
			SetWindowPos(m_hWnd,NULL,0,0,nWidth, nHeight,SWP_NOZORDER|SWP_NOMOVE);

			//更新控件
			UpdateTipControl();
		}
	}

	return;
}
//隐藏窗口
VOID CWndTipControl::QuickHidwWindow()
{
	ASSERT(IsWindow(m_hWnd)==TRUE);
	if(IsWindow(m_hWnd)==FALSE) return;

	//设置定时器
	KillTimer(m_hWnd,IDI_HIDE_WINDOW);

	//隐藏窗口
	ShowWindow(m_hWnd,SW_HIDE);
}

//显示窗口
VOID CWndTipControl::RelayShowWindow(INT nXPos,INT YPos,CRect rcInvalidArea)
{
	ASSERT(IsWindow(m_hWnd)==TRUE);
	if(IsWindow(m_hWnd)==FALSE) return;

	//设置变量
	m_rcInvalidArea=rcInvalidArea;

	//判断区域
	if(IsWindow(m_hWnd) && ::IsWindowVisible(m_hWnd)==FALSE)
	{
		//设置定时器
		KillTimer(m_hWnd,IDI_SHOW_WINDOW);

		//设置定时器
		SetTimer(m_hWnd,IDI_SHOW_WINDOW,m_nRelayTime,0);		
	}


	//设置位置
	SetWindowPos(m_hWnd,NULL,nXPos,YPos,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);
}


//设置资源
VOID CWndTipControl::SetControlResource(HINSTANCE hInstanceRes,LPCTSTR pszImageGround)
{
	//设置变量
	m_hInstance=hInstanceRes;
	lstrcpyn(m_szImageGround,pszImageGround,CountArray(m_szImageGround));

	//更新界面
	RedrawWindow(m_hWnd,NULL,NULL,RDW_INVALIDATE|RDW_ERASE);
}

//更新控件
BOOL CWndTipControl::UpdateTipControl()
{
	//获取区域
	CRect rcClient;
	GetWindowRect(m_hWnd,&rcClient);

	//构造位图
	BITMAPINFO bmi;       
	ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth=rcClient.Width();
	bmi.bmiHeader.biHeight=rcClient.Height();
	bmi.bmiHeader.biPlanes=1;
	bmi.bmiHeader.biBitCount=32; 
	bmi.bmiHeader.biCompression=BI_RGB;
	bmi.bmiHeader.biSizeImage=rcClient.Width() * rcClient.Height() * 4;

	//创建位图
	BYTE *pvBackBits,*pvFrontBits;         
	HBITMAP hBitmapBack=CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS,(void **)&pvBackBits, NULL, 0);
	HBITMAP hBitmapFront=CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS,(void **)&pvFrontBits, NULL, 0);

	//初始数据
	ZeroMemory(pvBackBits, bmi.bmiHeader.biSizeImage);	
	ZeroMemory(pvFrontBits, bmi.bmiHeader.biSizeImage);

	//创建设备
	HDC hDCScreen=GetDC(m_hWnd);
	HDC hDCMemBack=CreateCompatibleDC(hDCScreen);
	HDC hDCMemFront=CreateCompatibleDC(hDCScreen);

	//选择设备
	SelectObject(hDCMemBack,hBitmapBack);	
	SelectObject(hDCMemFront,hBitmapFront);

	//加载资源
	CPngImage ImageGround;
	ImageGround.LoadImage(m_hInstance,m_szImageGround);

	//绘制背景
	ImageGround.DrawImage(CDC::FromHandle(hDCMemBack),0,0);

	//拷贝背景
	BitBlt(hDCMemFront,0,0,rcClient.Width(),rcClient.Height(),hDCMemBack,0,0,SRCCOPY);

	//绘画视图
	DrawControlView(hDCMemFront,rcClient.Width(),rcClient.Height());		

	//修改Alpha通道
	int nImageHeaderSize = (int)(bmi.bmiHeader.biSizeImage);
	for(int i = 0; i + 3 < nImageHeaderSize; i += 4)
	{
		if(*(DWORD *)&pvBackBits[i] == *(DWORD *)&pvFrontBits[i]) continue;
		if(pvFrontBits[i+3]==0) pvFrontBits[i+3] = 255;
	}

	//变量定义
	POINT ptWinSrc={0,0};
	POINT ptWinPos={rcClient.left,rcClient.top};		
	SIZE SizeWindow={rcClient.Width(),rcClient.Height()};

	//更新窗口
	BOOL bResult=UpdateLayeredWindow(m_hWnd, hDCScreen, &ptWinPos, &SizeWindow, hDCMemFront, &ptWinSrc, 0, &m_Blend, ULW_ALPHA);	
	//释放设备	
	DeleteDC(hDCMemBack);
	DeleteDC(hDCMemFront);
	DeleteObject(hBitmapBack);
	DeleteObject(hBitmapFront);
	ReleaseDC(m_hWnd,hDCScreen);

	return bResult;
}

//绘画阴影
VOID CWndTipControl::DrawControlView(HDC hDC, INT nWdith, INT nHeight)
{
	//获取设备
	CDC * pDC = CDC::FromHandle(hDC);

	//设置缓冲
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//绘画界面
	OnDrawClientArea(pDC,nWdith,nHeight);

	return;
}

//鼠标消息
VOID CWndTipControl::OnMouseMove(UINT nFlags, CPoint Point)
{
	//变量定义
	if(m_bTrackMouse==false)
	{
		//构造结构
		TRACKMOUSEEVENT TrackMouseEvent;
		ZeroMemory(&TrackMouseEvent,sizeof(TrackMouseEvent));

		//注册消息
		TrackMouseEvent.hwndTrack=m_hWnd;
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);

		//注册事件
		_TrackMouseEvent(&TrackMouseEvent);

		//设置变量
		m_bTrackMouse=true;

		//关闭定时器
		KillTimer(m_hWnd,IDI_SHOW_WINDOW);
		KillTimer(m_hWnd,IDI_HIDE_WINDOW);
	}

	return;
}

//鼠标离开
LRESULT CWndTipControl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	//进入判断
	if (m_bTrackMouse==true)
	{
		//设置变量
		m_bTrackMouse=false;

		//设置定时器
		SetTimer(m_hWnd,IDI_HIDE_WINDOW,m_nRelayTime,0);
	}

	return 0;
}

//窗口过程
LRESULT CALLBACK CWndTipControl::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//变量定义
	CWndTipControl * pWinTipControl = NULL; 
	m_TipControlMap.Lookup(hwnd,pWinTipControl);

	if(pWinTipControl!=NULL)
	{
		switch(uMsg)
		{
		case WM_MOUSEMOVE:
			{
				CPoint Point;
				Point.x = (int)(short) LOWORD(lParam);
				Point.y = (int)(short) HIWORD(lParam);

				pWinTipControl->OnMouseMove((UINT)wParam,Point);

				break;
			}			
		case WM_MOUSELEAVE:
			{
				pWinTipControl->OnMouseLeave((UINT)wParam,lParam);
				break;
			}
		case WM_TIMER:
			{
				pWinTipControl->OnTimer((UINT_PTR)wParam);
				break;
			}
		case WM_NCDESTROY:
			{
				m_TipControlMap.RemoveKey(hwnd);	
				break;
			}
		}
	}
	return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//时间消息
VOID CWndTipControl::OnTimer(UINT_PTR nIDEvent)
{
	//显示窗口
	if(IDI_SHOW_WINDOW==nIDEvent)
	{
		//关闭定时器
		KillTimer(m_hWnd,nIDEvent);

		//设置定时器		
		if(DetectIsShowWindow()==TRUE)
		{
			//更新控件
			UpdateTipControl();

			//显示窗口
			ShowWindow(m_hWnd,SW_SHOWNOACTIVATE);
			SetForegroundWindow(m_hWnd);

			//设置定时器
			SetTimer(m_hWnd,IDI_HIDE_WINDOW,m_nRelayTime,0);
		}
	}

	//隐藏窗口
	if(IDI_HIDE_WINDOW==nIDEvent)
	{
		if(DetectIsHideWindow()==TRUE || IsWindowVisible(m_hWnd)==false)
		{
			//隐藏窗口
			ShowWindow(m_hWnd,SW_HIDE);

			//关闭定时器
			KillTimer(m_hWnd,nIDEvent);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////
