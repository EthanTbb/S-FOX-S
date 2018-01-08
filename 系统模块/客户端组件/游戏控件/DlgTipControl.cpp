#include "StdAfx.h"
#include "Resource.h"
#include "DlgTipControl.h"

//////////////////////////////////////////////////////////////////////////////////
//��������

//Բ�Ǵ�С
#define ROUND_CX					5									//Բ�ǿ��
#define ROUND_CY					5									//Բ�Ǹ߶�

//��Ӱ����
#define SHADOW_CX					6									//��Ӱ���
#define SHADOW_CY					6									//��Ӱ�߶�

//ʱ�Ӷ���
#define IDI_SHOW_WINDOW				100									//��ʾ����
#define IDI_HIDE_WINDOW				101									//���ش���

//////////////////////////////////////////////////////////////////////////////////
//��̬����
CTipControlMap					  CWndTipControl::m_TipControlMap;  

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CWndTipControl::CWndTipControl()
{
	//��������
	m_hWnd=NULL;
	m_bTrackMouse=false;
	m_nRelayTime=100;
	m_rcInvalidArea.SetRect(0,0,0,0);

	//�������
	m_hFuncInst=NULL;
	ZeroMemory(&m_Blend,sizeof(m_Blend));

	//���ñ���
	m_hInstance=NULL;
	m_szImageGround[0]=0;
}

//��������
CWndTipControl::~CWndTipControl()
{
	//���ٴ���
	if(m_hWnd!=NULL) 
	{
		DestroyWindow(m_hWnd);
	}

	//���ٶ�̬��
	if(m_hFuncInst!=NULL)
	{
		FreeLibrary(m_hFuncInst);
	}
}

//��ʾ����
BOOL CWndTipControl::DetectIsShowWindow()
{
	if(!IsWindow(m_hWnd)) return FALSE;

	//���ö�ʱ��
	CPoint ptCursor;
	GetCursorPos(&ptCursor);

	return m_rcInvalidArea.PtInRect(ptCursor);
}

//����̽��
BOOL CWndTipControl::DetectIsHideWindow()
{
	//���ö�ʱ��
	CPoint ptCursor;
	GetCursorPos(&ptCursor);

	return m_rcInvalidArea.PtInRect(ptCursor)==FALSE; 
}

//�滭��Ϣ
VOID CWndTipControl::OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight)
{
	return;
}


//������ʱ
VOID CWndTipControl::SetRelayTime(INT nRelayTime)
{
	//���ñ���
	m_nRelayTime=nRelayTime;
}

//��������
VOID CWndTipControl::CreateTipControl(INT nWidth,INT nHeight,INT nAlpha)
{
	//�жϾ��
	if(IsWindow(m_hWnd)==TRUE) return;	

	//���ض�̬��
	m_hFuncInst = LoadLibrary(TEXT("User32.DLL")); 	
	if(m_hFuncInst)
	{
		m_pUpdateLayeredWindow=(LPUpdateLayeredWindow)GetProcAddress(m_hFuncInst,"UpdateLayeredWindow");
		if(m_pUpdateLayeredWindow == NULL) return;
	}

	//����ṹ
	m_Blend.BlendOp=0;
	m_Blend.BlendFlags=0;
	m_Blend.AlphaFormat=AC_SRC_ALPHA;
	m_Blend.SourceConstantAlpha=nAlpha;

	//��������
	CRect rcCreate(0,0,0,0);

	//����ṹ
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

	//ע������
	if(::RegisterClass(&wc)!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS)
	{
		//��������
		m_hWnd=CreateWindowEx(WS_EX_TOOLWINDOW|WS_EX_LAYERED,wc.lpszClassName,TEXT(""),WS_POPUPWINDOW|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,0, 0, 0, 0,NULL,NULL,NULL,NULL);

		if(IsWindow(m_hWnd)==TRUE)
		{
			//���ñ���
			m_TipControlMap[m_hWnd]=this;

			//���ô�С
			SetWindowPos(m_hWnd,NULL,0,0,nWidth, nHeight,SWP_NOZORDER|SWP_NOMOVE);

			//���¿ؼ�
			UpdateTipControl();
		}
	}

	return;
}
//���ش���
VOID CWndTipControl::QuickHidwWindow()
{
	ASSERT(IsWindow(m_hWnd)==TRUE);
	if(IsWindow(m_hWnd)==FALSE) return;

	//���ö�ʱ��
	KillTimer(m_hWnd,IDI_HIDE_WINDOW);

	//���ش���
	ShowWindow(m_hWnd,SW_HIDE);
}

//��ʾ����
VOID CWndTipControl::RelayShowWindow(INT nXPos,INT YPos,CRect rcInvalidArea)
{
	ASSERT(IsWindow(m_hWnd)==TRUE);
	if(IsWindow(m_hWnd)==FALSE) return;

	//���ñ���
	m_rcInvalidArea=rcInvalidArea;

	//�ж�����
	if(IsWindow(m_hWnd) && ::IsWindowVisible(m_hWnd)==FALSE)
	{
		//���ö�ʱ��
		KillTimer(m_hWnd,IDI_SHOW_WINDOW);

		//���ö�ʱ��
		SetTimer(m_hWnd,IDI_SHOW_WINDOW,m_nRelayTime,0);		
	}


	//����λ��
	SetWindowPos(m_hWnd,NULL,nXPos,YPos,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);
}


//������Դ
VOID CWndTipControl::SetControlResource(HINSTANCE hInstanceRes,LPCTSTR pszImageGround)
{
	//���ñ���
	m_hInstance=hInstanceRes;
	lstrcpyn(m_szImageGround,pszImageGround,CountArray(m_szImageGround));

	//���½���
	RedrawWindow(m_hWnd,NULL,NULL,RDW_INVALIDATE|RDW_ERASE);
}

//���¿ؼ�
BOOL CWndTipControl::UpdateTipControl()
{
	//��ȡ����
	CRect rcClient;
	GetWindowRect(m_hWnd,&rcClient);

	//����λͼ
	BITMAPINFO bmi;       
	ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth=rcClient.Width();
	bmi.bmiHeader.biHeight=rcClient.Height();
	bmi.bmiHeader.biPlanes=1;
	bmi.bmiHeader.biBitCount=32; 
	bmi.bmiHeader.biCompression=BI_RGB;
	bmi.bmiHeader.biSizeImage=rcClient.Width() * rcClient.Height() * 4;

	//����λͼ
	BYTE *pvBackBits,*pvFrontBits;         
	HBITMAP hBitmapBack=CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS,(void **)&pvBackBits, NULL, 0);
	HBITMAP hBitmapFront=CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS,(void **)&pvFrontBits, NULL, 0);

	//��ʼ����
	ZeroMemory(pvBackBits, bmi.bmiHeader.biSizeImage);	
	ZeroMemory(pvFrontBits, bmi.bmiHeader.biSizeImage);

	//�����豸
	HDC hDCScreen=GetDC(m_hWnd);
	HDC hDCMemBack=CreateCompatibleDC(hDCScreen);
	HDC hDCMemFront=CreateCompatibleDC(hDCScreen);

	//ѡ���豸
	SelectObject(hDCMemBack,hBitmapBack);	
	SelectObject(hDCMemFront,hBitmapFront);

	//������Դ
	CPngImage ImageGround;
	ImageGround.LoadImage(m_hInstance,m_szImageGround);

	//���Ʊ���
	ImageGround.DrawImage(CDC::FromHandle(hDCMemBack),0,0);

	//��������
	BitBlt(hDCMemFront,0,0,rcClient.Width(),rcClient.Height(),hDCMemBack,0,0,SRCCOPY);

	//�滭��ͼ
	DrawControlView(hDCMemFront,rcClient.Width(),rcClient.Height());		

	//�޸�Alphaͨ��
	int nImageHeaderSize = (int)(bmi.bmiHeader.biSizeImage);
	for(int i = 0; i + 3 < nImageHeaderSize; i += 4)
	{
		if(*(DWORD *)&pvBackBits[i] == *(DWORD *)&pvFrontBits[i]) continue;
		if(pvFrontBits[i+3]==0) pvFrontBits[i+3] = 255;
	}

	//��������
	POINT ptWinSrc={0,0};
	POINT ptWinPos={rcClient.left,rcClient.top};		
	SIZE SizeWindow={rcClient.Width(),rcClient.Height()};

	//���´���
	BOOL bResult=UpdateLayeredWindow(m_hWnd, hDCScreen, &ptWinPos, &SizeWindow, hDCMemFront, &ptWinSrc, 0, &m_Blend, ULW_ALPHA);	
	//�ͷ��豸	
	DeleteDC(hDCMemBack);
	DeleteDC(hDCMemFront);
	DeleteObject(hBitmapBack);
	DeleteObject(hBitmapFront);
	ReleaseDC(m_hWnd,hDCScreen);

	return bResult;
}

//�滭��Ӱ
VOID CWndTipControl::DrawControlView(HDC hDC, INT nWdith, INT nHeight)
{
	//��ȡ�豸
	CDC * pDC = CDC::FromHandle(hDC);

	//���û���
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//�滭����
	OnDrawClientArea(pDC,nWdith,nHeight);

	return;
}

//�����Ϣ
VOID CWndTipControl::OnMouseMove(UINT nFlags, CPoint Point)
{
	//��������
	if(m_bTrackMouse==false)
	{
		//����ṹ
		TRACKMOUSEEVENT TrackMouseEvent;
		ZeroMemory(&TrackMouseEvent,sizeof(TrackMouseEvent));

		//ע����Ϣ
		TrackMouseEvent.hwndTrack=m_hWnd;
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);

		//ע���¼�
		_TrackMouseEvent(&TrackMouseEvent);

		//���ñ���
		m_bTrackMouse=true;

		//�رն�ʱ��
		KillTimer(m_hWnd,IDI_SHOW_WINDOW);
		KillTimer(m_hWnd,IDI_HIDE_WINDOW);
	}

	return;
}

//����뿪
LRESULT CWndTipControl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	//�����ж�
	if (m_bTrackMouse==true)
	{
		//���ñ���
		m_bTrackMouse=false;

		//���ö�ʱ��
		SetTimer(m_hWnd,IDI_HIDE_WINDOW,m_nRelayTime,0);
	}

	return 0;
}

//���ڹ���
LRESULT CALLBACK CWndTipControl::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//��������
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

//ʱ����Ϣ
VOID CWndTipControl::OnTimer(UINT_PTR nIDEvent)
{
	//��ʾ����
	if(IDI_SHOW_WINDOW==nIDEvent)
	{
		//�رն�ʱ��
		KillTimer(m_hWnd,nIDEvent);

		//���ö�ʱ��		
		if(DetectIsShowWindow()==TRUE)
		{
			//���¿ؼ�
			UpdateTipControl();

			//��ʾ����
			ShowWindow(m_hWnd,SW_SHOWNOACTIVATE);
			SetForegroundWindow(m_hWnd);

			//���ö�ʱ��
			SetTimer(m_hWnd,IDI_HIDE_WINDOW,m_nRelayTime,0);
		}
	}

	//���ش���
	if(IDI_HIDE_WINDOW==nIDEvent)
	{
		if(DetectIsHideWindow()==TRUE || IsWindowVisible(m_hWnd)==false)
		{
			//���ش���
			ShowWindow(m_hWnd,SW_HIDE);

			//�رն�ʱ��
			KillTimer(m_hWnd,nIDEvent);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////
